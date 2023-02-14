#ifndef MEALMASTER_H
#define MEALMASTER_H

#include <fstream>
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <utility>
#include <vector>

class MealMaster {
public:
    //Constructor
    MealMaster();
    //Destructor
    ~MealMaster();
    //Builds database from Recipes.txt
    bool buildDatabase();

private:
    //Retrieves mealTypeID from mealTypes table
    int getMealTypeID(std::string&);
    //Retrieves ingredientID from Ingredients table
    int getIngredientID(std::string&);
    //Retrieves recipeID from Recipes table
    int getRecipeID(std::string&);
    //Adds ingredient to Ingredients table
    void addIngredient(std::string&);    
    //Adds recipe to Recipes table
    void addRecipe(std::string&, int, std::string&);
    //Adds qty and qty type info for ingredient in a recipe to RecipeIngredients table
    void addRecipeIngredients(int, int, float, std::string&);
    //Generates SQL queries
    std::string queryGen(std::string, std::string);
    
    //Connection handle to the PostgreSQL database
    PGconn* conn;

    //Struct for holding recipe information
    struct Recipe {
        int recipeID;
        int mealTypeID;
        std::string name;
        //Holds ingredientID, qty, and unit
        std::vector<std::pair<int, std::pair<float, std::string>>> ingredients;
    };
};

#endif