#!/bin/bash

# Set search path for current session
psql -U $USER -d $USER -c "SET SEARCH_PATH to MealMaster";

# Create database Tables
psql -U $USER -d $USER -f ../DB/create.sql

# Create database Tables
psql -U $USER -d $USER -f ../DB/createIndex.sql

# Store functions in database
psql -U $USER -d $USER -f ../DB/storedFunctions.pgsql

# Fill MealTypes table in database
psql -U $USER -d $USER -c "SELECT fillMealTypes()";

# Make Meal Master
make

# Run Meal Master
./MealMasterv1

# Make clean
#make clean