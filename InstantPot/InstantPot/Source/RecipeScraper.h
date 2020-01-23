#pragma once
#include <iostream>
#include <string>
#include <filesystem>
#include <urlmon.h>
#include <regex>
#include <chrono>
#include <iomanip>
#include <thread>
#include "Recipe.h"
using namespace std;
class RecipeScraper
{
public:
	RecipeScraper(){}
	void BeginScrape();
private:
	vector<string> getRecipesFromSite(ifstream& fin);
	string downloadSite(string webAddress, string fileName);
	void downloadRecipes();
	Recipe ExtractRecipeFromSite(std::string& recipeSite);
	string ExtractSpanTag(std::string& line, std::string spanOpener);
	void ExtractIngredients(std::string& line, std::vector<Ingredient>& IngredientsInRecipe);
	Ingredient ExtractSpanTagsIntoIngredient(std::string& ingredientClass);

};

