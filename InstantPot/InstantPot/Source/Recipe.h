#pragma once
#include "Ingredient.h"
#include <fstream>
#include <iostream>
#include <vector>
enum ParseState
{
	E_NAME,
	E_INGREDIENTS,
	E_INSTRUCTIONS,
	E_NOTES,
	E_LINK,
	E_TIME
};
class Recipe
{
public:
	friend bool operator< (const Recipe& recipe1, const Recipe& recipe2);
	friend bool operator> (const Recipe& recipe1, const Recipe& recipe2);
	friend ostream& operator<<(ostream& out, const Recipe& c);
public:
	void setRecipeURL(string URL) { recipeURL = URL; }
	string getRecipeURL() { return recipeURL; }

	void setCookTime(string inTime) { cookTime = inTime; }
	string getCookTime() { return cookTime; }

	void setName(string inName) { name = inName; }
	string getName() { return name; }

	void addIngredient(Ingredient ingrToAdd) { Ingredients.push_back(ingrToAdd); }
	vector<Ingredient> getAllIngredients() { return Ingredients; }

	void setNumberOfSimiliarIngredients(int a) { numberOfSimilarIngredients = a; }
	int getNumberOfSimiliarIngredients() { return numberOfSimilarIngredients; }
	
	string getRecipeName() { return name; }
	
	bool hasLessIngredients(const Recipe& recipe) {return Ingredients.size() < recipe.Ingredients.size();}
	
	void Serialize();
	void parseRecipe(string recipeName);
	ParseState getSection(string line);
	void parseLine(string line, ParseState currentSection);
private:
	Ingredient getIngredientFromLine(string line);
	
private: //Runtime Variables
	int numberOfSimilarIngredients = 0;
private: //Serilization Variables
	string recipeURL;
	string cookTime;
	string name;
	vector<Ingredient> Ingredients;
	vector<string> Instructions;
	vector<string> Notes;
};

