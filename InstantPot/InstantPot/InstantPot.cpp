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
namespace fs = std::filesystem;

void iterateThruRecipes();
void downloadRecipes();
Ingredient ExtractSpanTagsIntoIngredient(std::string& ingredientClass);
int main()
{	
	downloadRecipes();
	//iterateThruRecipes();
}

vector<string> getRecipesFromSite(ifstream &fin)
{	
	vector<string> recipes;
	string line;
	while (getline(fin, line))
	{
		int recipeURLIndex = line.find("https://recipes.instantpot.com/recipe");
		if (recipeURLIndex != string::npos)
		{
			int URLEndPoint = line.find(">", recipeURLIndex);
			string siteToDownload = line.substr(recipeURLIndex, URLEndPoint - 22);
			recipes.push_back(siteToDownload);
		}
	}
	return recipes;
}

vector<string> getIngredientsFromSite(ifstream& fin)
{
	return vector<string>{};
}

string downloadSite(string webAddress, string fileName)
{
	HRESULT hr = URLDownloadToFile(NULL, webAddress.c_str(), fileName.c_str(), 0, NULL);
	if (hr == S_OK)
	{
		cout << endl << "Downloaded: " << webAddress << " with file name: " << fileName << endl;
		return fileName;
	}
	else
	{
		cout << "Operation failed with error code: " << hr << " on site " << webAddress << " with file name " << fileName << "\n";
		return "";
	}
}

void downloadRecipes()
{
	string webAddress = "https://recipes.instantpot.com/?coursefilter=&cuisinefilter=&dietfilter=&search=&bsearch=Search&index=";

	for (int i = 1; i < 2; i++)
	{
		//string downloadedSite = downloadSite(webAddress + char(i + '0'), string("Page.html"));
		//ifstream fin(downloadedSite);
		//vector<string> recipeURLS = getRecipesFromSite(fin);
		
		//string recipeSite = downloadSite(recipeURLS[0], "recipe.html");
		string recipeSite = "recipe.html";
		ifstream finRecipe(recipeSite);
		string line;
		vector<Ingredient> IngredientsInRecipe;
		while (getline(finRecipe, line))
		{
			if (line.find("<div class=\"wpurp-responsive-desktop\">") != string::npos) //avoid reading recipe twice
				break;
			int ingredientClassStartIndex = line.find("<li class=\"wpurp-recipe-ingredient\"");
			do {
				int ingredientClassEndIndex = line.find("</li>", ingredientClassStartIndex);
				ingredientClassEndIndex += 5;
				if (ingredientClassStartIndex != string::npos && ingredientClassEndIndex != string::npos)
				{
					string ingredientClass = line.substr(ingredientClassStartIndex, ingredientClassEndIndex - ingredientClassStartIndex);
					IngredientsInRecipe.push_back(ExtractSpanTagsIntoIngredient(ingredientClass));
				}
				ingredientClassStartIndex = line.find("<li class=\"wpurp-recipe-ingredient\"", ingredientClassEndIndex);
			} while (ingredientClassStartIndex != string::npos); // Go through all ingredients
		}
		cout << left << setw(50) << "Amount" << left << setw(50) << "Type" << left << setw(50) << "Name" << endl;
		for (auto a : IngredientsInRecipe)
		{
			cout << left << setw(50) << a.getMeasurement();
			cout << left << setw(50) << a.getMeasurementType();
			cout << left << setw(50) << a.getName() << endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

Ingredient ExtractSpanTagsIntoIngredient(std::string& ingredientClass)
{
	int ingredientClassSpanStart = ingredientClass.find("<span");
	int index = 0;
	Ingredient ingredientToConstruct;
	do {
		int ingredientClassSpanEnd = ingredientClass.find("</span>", ingredientClassSpanStart);
		ingredientClassSpanEnd += 7;
		string spanString = ingredientClass.substr(ingredientClassSpanStart, ingredientClassSpanEnd - ingredientClassSpanStart);
		smatch amount;
		smatch measurementUnit;
		smatch ingredientName;
		if (index == 0)
		{
			regex rg("^<.*data-normalized=\"(.*)?\".*data-fraction=\"(.*)?\".*data-original=\"(.*)\"\\sclass.*!important;\">(.*)?<.*pan>$");

			if (regex_match(spanString, amount, rg))
			{
				ingredientToConstruct.setMeasurement(amount[1]);
			}
			else
			{
				cout << "No Match" << endl;
			}
		}
		else if (index == 1)
		{
			regex units("^<.*!important;\">(.*)?<.*>$");
			if (regex_match(spanString, measurementUnit, units))
			{
				ingredientToConstruct.setMeasurementType(measurementUnit[1]);
			}
			else
			{
				cout << spanString << endl;
			}
		}
		else if (index == 2)
		{
			regex ingredient("^<.*!important;\">(.*)?<.*>$");
			if (regex_match(spanString, ingredientName, ingredient))
			{
				string ing = ingredientName[1];
				if (ing.find("<ucsa>") != string::npos)
				{
					ing = ing.substr(6, ing.find("</") - 6);
				}
				ingredientToConstruct.setIngredientName(ing);
			}
			else
			{
				cout << spanString << endl;
			}
		}
		ingredientClassSpanStart = ingredientClass.find("<span", ingredientClassSpanEnd);
		index++;
	} while (ingredientClassSpanStart != string::npos); // Go through all <span> tags
	return ingredientToConstruct;
}

void iterateThruRecipes()
{

	vector<string> allAvaliableIngredients;
	vector<Recipe> allCompatibleRecipes;
	string line;
	cout << "Please enter your ingredients: \n";
	while (getline(cin, line))
	{
		allAvaliableIngredients.push_back(line);
	}
	string path = "Recipes/";
	for (const auto& entry : fs::directory_iterator(path))
	{
		string recipeName = entry.path().string();
		Recipe recipe;
		recipe.parseRecipe(recipeName);


		int numberOfSimiliarIngredients = 0;
		for (auto a : recipe.getAllIngredients())
		{
			for (auto b : allAvaliableIngredients)
			{
				if (a.getIngredientName().find(b) != string::npos)
				{
					++numberOfSimiliarIngredients;
				}
			}
		}
		recipe.setNumberOfSimiliarIngredients(numberOfSimiliarIngredients);
		allCompatibleRecipes.push_back(recipe);
	}
	for (int i = 0; i < allCompatibleRecipes.size(); i++)
	{
		cout << i + 1 << ". For " << allCompatibleRecipes[i].getRecipeName() << " you have ";
		cout << allCompatibleRecipes[i].getNumberOfSimiliarIngredients() << " / " << allCompatibleRecipes[i].getAllIngredients().size() << " Ingredients. \n";
	}
}