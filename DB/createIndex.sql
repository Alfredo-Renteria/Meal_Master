CREATE INDEX FindMealTypeRecipes
    ON Recipes(MealTypeID);

CREATE INDEX FindRecipeIngredients
    ON RecipeIngredients(recipeID, ingredientID);