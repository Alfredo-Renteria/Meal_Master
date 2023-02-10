DROP SCHEMA MealMaster CASCADE;
CREATE SCHEMA MealMaster;

CREATE TABLE MealTypes (
    mealTypeID SERIAL,
    mealTypeName VARCHAR(2) NOT NULL,
    PRIMARY KEY(mealTypeID),
    UNIQUE(mealTypeName)
);

CREATE TABLE Ingredients (
    ingredientID SERIAL,
    ingredientName VARCHAR(100) NOT NULL,
    PRIMARY KEY(ingredientID)
);

CREATE TABLE Recipes (
    recipeID SERIAL,
    recipeName VARCHAR(100) NOT NULL,
    mealTypeID INT,
    instructions TEXT NOT NULL,
    PRIMARY KEY(recipeID),
    FOREIGN KEY(mealTypeID) REFERENCES MealTypes(mealTypeID)
);

CREATE TABLE RecipeIngredients (
    recipeIngredientsID SERIAL,
    recipeID INT,
    ingredientID INT,
    TotalQty NUMERIC NOT NULL,
    QtyType VARCHAR(20) NOT NULL,
    PRIMARY KEY(recipeIngredientsID),
    FOREIGN KEY(recipeID) REFERENCES Recipes(recipeID),
    FOREIGN KEY(ingredientID) REFERENCES Ingredients(ingredientID)
);
