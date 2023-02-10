-- Fills MealTypes table with all possible mealTypeNames: 'G' | 'P' | 'R' | 'S'

CREATE OR REPLACE FUNCTION 
fillMealTypes()
RETURNS void AS $$
  BEGIN
    INSERT INTO MealTypes (mealTypeName)
    VALUES ('G'), ('P'), ('R'), ('S');
  END;
$$ LANGUAGE plpgsql;

-- Retrieves ID from MealTypes table given mealTypeName: 'G' | 'P' | 'R' | 'S'

CREATE OR REPLACE FUNCTION
getMealTypeID(mealtype_name_ VARCHAR)
RETURNS INT AS $$
  DECLARE
  mealtype_id_ INT;
  BEGIN
    SELECT mealTypeID INTO mealtype_id_
    FROM MealTypes
    WHERE mealTypeName = mealtype_name_;
  RETURN mealtype_id_;
  END;
$$ LANGUAGE plpgsql;

-- Inserts ingredient into the Ingredients table given ingredientName

CREATE OR REPLACE FUNCTION addIngredient(ingredient_name_ VARCHAR)
RETURNS VOID AS $$
BEGIN
  IF NOT EXISTS (SELECT 1 FROM Ingredients WHERE ingredientName = ingredient_name_) THEN
    INSERT INTO Ingredients (ingredientName)
    VALUES (ingredient_name_);
  END IF;
END;
$$ LANGUAGE plpgsql;

-- Retrieves ingredientID from Ingredients table given ingredientName

CREATE OR REPLACE FUNCTION
getIngredientID(ingredient_name_ VARCHAR)
RETURNS INT AS $$
  DECLARE
  ingredient_id_ INT;
  BEGIN
    SELECT ingredientID INTO ingredient_id_
    FROM Ingredients
    WHERE ingredientName = ingredient_name_;
  RETURN ingredient_id_;
  END;
$$ LANGUAGE plpgsql;

-- Inserts recipe into the Recipes table given recipeName, mealtTypeID, instructions

CREATE OR REPLACE FUNCTION addRecipe(recipe_name_ VARCHAR, mealtype_id_ INT, instructions_ TEXT)
RETURNS VOID AS $$
BEGIN
  IF NOT EXISTS (SELECT 1 FROM Recipes WHERE recipeName = recipe_name_) THEN
    INSERT INTO Recipes (recipeName, mealTypeID, instructions)
    VALUES (recipe_name_, mealtype_id_, instructions_);
  END IF;
END;
$$ LANGUAGE plpgsql;

-- Retrieves recipeID from Recipes table given recipeName

CREATE OR REPLACE FUNCTION
getRecipeID(recipe_name_ VARCHAR)
RETURNS INT AS $$
  DECLARE
  recipe_id_ INT;
  BEGIN
    SELECT recipeID INTO recipe_id_
    FROM Recipes
    WHERE RecipeName = recipe_name_;
  RETURN recipe_id_;
  END;
$$ LANGUAGE plpgsql;


-- Inserts qty and qty type for given ingredient in a recipe

CREATE OR REPLACE FUNCTION addRecipeIngredients(recipe_id_ INT, ingredient_id_ INT, total_qty_ NUMERIC, qty_type_ VARCHAR)
RETURNS VOID AS $$
BEGIN
  IF NOT EXISTS (SELECT 1 FROM RecipeIngredients WHERE ingredientID = ingredient_id_) THEN
    INSERT INTO RecipeIngredients (recipeID, ingredientID, totalQty, QtyType)
    VALUES (recipe_id_, ingredient_id_, total_qty_, qty_type_);
  END IF;
END;
$$ LANGUAGE plpgsql;