#pragma once
#include "Ingredient.h"
#include <fstream>
#include <vector>
enum ParseState
{
	E_NAME,
	E_INGREDIENTS,
	E_INSTRUCTIONS,
	E_NOTES
};
class Recipe
{
	public:
		int getNumberOfSimiliarIngredients() { return numberOfSimilarIngredients; }
		void setNumberOfSimiliarIngredients(int a) { numberOfSimilarIngredients = a; }
		vector<Ingredient> getAllIngredients(){ return Ingredients; }
		string getRecipeName() { return name; }
		void parseRecipe(string recipeName);
		Ingredient getIngredientFromLine(string line);
		string printRecipe();
		ParseState getSection(string line);
		void parseLine(string line, ParseState currentSection);
	private:
		int numberOfSimilarIngredients = 0;
		string name;
		vector<Ingredient> Ingredients;
		vector<string> Instructions;
		vector<string> Notes;
};

