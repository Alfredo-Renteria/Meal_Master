#include <fstream>
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include "MealMaster.h"

int main() {

    MealMaster recipe;
    if (recipe.buildDatabase()) {
        std::cout << "The database was built successfully!" << std::endl;
    } else {
        std::cout << "The database failed to build." << std::endl;
    }
}