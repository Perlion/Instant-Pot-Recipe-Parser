#pragma once
#include "Ingredient.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

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
	Recipe()
	{
		Ingredients.reserve(50);
	}

	friend bool operator< (const Recipe& recipe1, const Recipe& recipe2);
	friend bool operator> (const Recipe& recipe1, const Recipe& recipe2);
	friend ostream& operator<<(ostream& out, const Recipe& c);
	unsigned int numberOfIngredients = 0;
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
	void parseRecipe(fs::path entry);
	ParseState getSection(string_view line);
	void parseLine(const string& line, ParseState& currentSection);
private:
	void getIngredientFromLine(const string& line, Ingredient& ingredient);
	
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

