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
	void iterateThruRecipes();
	void downloadRecipes();
	Ingredient ExtractSpanTagsIntoIngredient(std::string& ingredientClass);
};

