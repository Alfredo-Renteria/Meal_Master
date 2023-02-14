#include <fstream>
#include <iostream>
#include <libpq-fe.h>
#include <sstream>
#include <string>
#include <vector>
#include "dbInfo.h"
#include "MealMaster.h"

MealMaster::MealMaster() {
        //Connect to the database
        const std::string logIn = "user=" + dbInfo::USER + " dbname=" + dbInfo::USER;
        
        conn = PQconnectdb(logIn.c_str());
        
        if (PQstatus(conn) != CONNECTION_OK) {
            std::cerr << "MealMaster(): Failed to connect to the database: " << PQerrorMessage(conn) << std::endl;
        }
}

MealMaster::~MealMaster() {
        // Close the connection to the database
        PQfinish(conn);
}

bool MealMaster::buildDatabase() {
        //std::cout << "buildDatabase(): Initiated" << std::endl;
        const std::string fileLoc = "../FILES/Recipes.txt";
        //Open Recipes file
        std::ifstream file (fileLoc.c_str());
        
        //Check for errors
        if (!file.is_open()) {
            std::cerr << "MealMaster(): Failed to open Recipes.txt" << std::endl;
            return false;
        }
        //std::cout << "buildDatabase(): File open successful" << std::endl;
        
        std::string line;
        std::stringstream ss;

        while (std::getline(file, line)) {
            Recipe recipe;
            if (!line.empty()) {
                /* PROCESS RECIPE NAME */
                recipe.name = line;

                /* PROCESS MEAL TYPE */
                std::getline(file, line);
                recipe.mealTypeID = getMealTypeID(line);

                /* PROCESS INGREDIENTS */
                while (std::getline(file, line) && line != "#") {
                    //Clear str. stream data and error flags
                    ss.str(std::string());
                    ss.clear();
                    
                    float quantity;
                    std::string unit, ingredientName;

                    ss.str(line);
                    ss >> quantity >> unit;
                    std::getline(ss, ingredientName);

                    //Add ingredient to db
                    addIngredient(ingredientName);
                    //Get ingredientID from db
                    int ingredientID = getIngredientID(ingredientName);

                    recipe.ingredients.push_back({ingredientID, {quantity, unit}});
                }
                /* PROCESS INSTRUCTIONS */
                std::getline(file, line);

                //Add recipe to db
                addRecipe(recipe.name, recipe.mealTypeID, line);
                //Get recipeID from db
                recipe.recipeID = getRecipeID(recipe.name);

                //Add qty and qty type info for ingredients in recipe to db
                for (auto const& ing : recipe.ingredients) {
                    std::string unit = ing.second.second;
                    //std::cout << recipe.recipeID << " " << ing.first << " " << ing.second.first << " " << unit << std::endl;
                    addRecipeIngredients(recipe.recipeID, ing.first, ing.second.first, unit);
                }
            }
        }

        file.close();
        
        return true;
}

int MealMaster::getMealTypeID(std::string& mealType) {
        const std::string storedFnName = "getMealTypeID";
        const std::string input = mealType;
        const std::string query = queryGen(input, storedFnName);

        PGresult* mealTypeIDres = PQexec(conn, query.c_str());

        if (PQresultStatus(mealTypeIDres) != PGRES_TUPLES_OK) {
            std::cerr << "MealMaster(): Failed to execute getMealTypeID(): " << PQerrorMessage(conn) << std::endl;
            PQclear(mealTypeIDres);
            PQfinish(conn);
        }

        int mealTypeID = std::atoi(PQgetvalue(mealTypeIDres, 0, 0));
        PQclear(mealTypeIDres);

        return mealTypeID;
}

int MealMaster::getIngredientID(std::string& ingredientName) {
        const std::string storedFnName = "getIngredientID";
        const std::string input = ingredientName;
        const std::string query = queryGen(input, storedFnName);

        PGresult* getIngredientRes = PQexec(conn, query.c_str());

        if (PQresultStatus(getIngredientRes) != PGRES_TUPLES_OK) {
            std::cerr << "MealMaster(): Failed to execute getIngredientID(): " << PQerrorMessage(conn) << std::endl;
            PQclear(getIngredientRes);
            PQfinish(conn);
        }

        int ingredientID = std::atoi(PQgetvalue(getIngredientRes, 0, 0));    
        PQclear(getIngredientRes);

        return ingredientID;
}

int MealMaster::getRecipeID(std::string& recipeName) {
        const std::string storedFnName = "getRecipeID";
        const std::string input = recipeName;
        const std::string query = queryGen(input, storedFnName);

        PGresult* getRecipeRes = PQexec(conn, query.c_str());      
        if (PQresultStatus(getRecipeRes) != PGRES_TUPLES_OK) {
            std::cerr << "MealMaster(): Failed to execute getIngredientID(): " << PQerrorMessage(conn) << std::endl;
            PQclear(getRecipeRes);
            PQfinish(conn);
        }

        int recipeID = std::atoi(PQgetvalue(getRecipeRes, 0, 0));    
        PQclear(getRecipeRes);

        return recipeID;
}

void MealMaster::addIngredient(std::string& ingredientName) {
        const std::string storedFnName = "addIngredient";
        const std::string input = ingredientName;
        const std::string query = queryGen(input, storedFnName);
                
        PGresult* addIngredientRes = PQexec(conn, query.c_str());      
        
        if (PQresultStatus(addIngredientRes) != PGRES_TUPLES_OK) {
            std::cerr << "MealMaster(): Failed to execute addIngredient(): " << PQerrorMessage(conn) << std::endl;
            PQclear(addIngredientRes);
            PQfinish(conn);
        }
        PQclear(addIngredientRes);

}

void MealMaster::addRecipe(std::string& recipeName, int mealTypeID, std::string& instructions) {
        const std::string storedFnName = "addRecipe";
        const std::string input = recipeName + "', '" + std::to_string(mealTypeID) + "', '" + instructions;
        const std::string query = queryGen(input, storedFnName);        

        PGresult* addRecipeRes = PQexec(conn, query.c_str());

        if (PQresultStatus(addRecipeRes) != PGRES_TUPLES_OK) {
            std::cerr << "MealMaster(): Failed to execute addRecipe(): " << PQerrorMessage(conn) << std::endl;
            PQclear(addRecipeRes);
            PQfinish(conn);
        }
        PQclear(addRecipeRes);
}

void MealMaster::addRecipeIngredients(int recipeID, int ingredientID, float qty, std::string& unit) {
        const std::string storedFnName = "addRecipeIngredients";
        const std::string input = std::to_string(recipeID) + "', '" + std::to_string(ingredientID) + "', '" + std::to_string(qty) + "', '" + unit;
        const std::string query = queryGen(input, storedFnName);
        
        PGresult* addRecipeIngredientRes = PQexec(conn, query.c_str());      
        
        if (PQresultStatus(addRecipeIngredientRes) != PGRES_TUPLES_OK) {
            std::cerr << "MealMaster(): Failed to execute addRecipeIngredients(): " << PQerrorMessage(conn) << std::endl;
            PQclear(addRecipeIngredientRes);
            PQfinish(conn);
        }

        PQclear(addRecipeIngredientRes);
}

std::string MealMaster::queryGen(std::string input, std::string storedFnName) {
    std::string query = "SELECT " + storedFnName + "(";

    if (input.empty()) {
        query += ");";
        return query;
    }

    query += "'" + input + "');";

    return query;
}