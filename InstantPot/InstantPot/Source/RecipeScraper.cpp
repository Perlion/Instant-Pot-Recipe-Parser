#include "RecipeScraper.h"

namespace fs = std::filesystem;

void RecipeScraper::BeginScrape()
{
	downloadRecipes();
}

vector<string> RecipeScraper::getRecipesFromSite(ifstream& fin)
{
	vector<string> recipes;
	string line;
	while (getline(fin, line))
	{
		int recipeURLIndex = line.find("https://recipes.instantpot.com/recipe");
		if (recipeURLIndex != string::npos)
		{
			int URLEndPoint = line.find("\">", recipeURLIndex);
			string siteToDownload = line.substr(recipeURLIndex, URLEndPoint - recipeURLIndex);
			recipes.push_back(siteToDownload);
		}
	}
	return recipes;
}

string RecipeScraper::downloadSite(string webAddress, string fileName)
{
	HRESULT hr = URLDownloadToFile(NULL, webAddress.c_str(), fileName.c_str(), 0, NULL);
	if (hr == S_OK)
	{
		//cout << endl << "Downloaded: " << webAddress << " with file name: " << fileName << endl;
		return fileName;
	}
	else
	{
		//cout << "Operation failed with error code: " << hr << " on site " << webAddress << " with file name " << fileName << "\n";
		return "";
	}
}

void RecipeScraper::downloadRecipes()	
{
	string webAddress = "https://recipes.instantpot.com/?coursefilter=&cuisinefilter=&dietfilter=&search=&bsearch=Search&index=";
	string path = "Recipes/";

	for (int i = 1; i < 39; i++)//38 pages
	{
		string indexDownloadName = "C:/Users/UE4Jam/Documents/GitHub/Instant-Pot-Recipe-Parser/InstantPot/InstantPot/Sites/Indexs/Index";
		indexDownloadName += to_string(i);
		indexDownloadName += ".html";
		string searchAddress = webAddress + to_string(i);
		string downloadedSite = downloadSite(searchAddress, indexDownloadName);
		
		ifstream fin(downloadedSite);
		vector<string> recipeURLS = getRecipesFromSite(fin);
		
		
		for (int j = 0; j < recipeURLS.size(); j++) // Individual Recipe Download
		{
			cout << "Downloading page: " << i << " with recipe URL: " << recipeURLS[j] << endl;
			string siteDownloadName = "C:/Users/UE4Jam/Documents/GitHub/Instant-Pot-Recipe-Parser/InstantPot/InstantPot/Sites/Recipies/recipe";
			siteDownloadName += to_string(i);
			siteDownloadName += to_string(j);
			siteDownloadName += ".html";
			string recipeSite = downloadSite(recipeURLS[j], siteDownloadName);
			Recipe recipeToAdd = ExtractRecipeFromSite(recipeSite);
			recipeToAdd.setRecipeURL(recipeURLS[j]);
			recipeToAdd.Serialize();
			//std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Delay as to not send 1100 requests to their server over a span of 5 minutes
	}
}

Recipe RecipeScraper::ExtractRecipeFromSite(std::string& recipeSite)
{
	ifstream finRecipe(recipeSite);
	string line;
	vector<Ingredient> IngredientsInRecipe;
	Recipe recipeToConstruct;
	while (getline(finRecipe, line))
	{
		if (line.find("<div class=\"wpurp-responsive-desktop\">") != string::npos) //avoid reading recipe twice
			break;
		if (recipeToConstruct.getName().empty())
		{
			recipeToConstruct.setName(ExtractSpanTag(line, "<span class=\"wpurp-recipe-title"));
		}
		if (recipeToConstruct.getCookTime().empty())
		{
			recipeToConstruct.setCookTime(ExtractSpanTag(line, "<span class=\"wpurp-recipe-cook-time"));
		}
		ExtractIngredients(line, IngredientsInRecipe); // Go through all ingredients
	}
	for (auto a : IngredientsInRecipe)
	{
		recipeToConstruct.addIngredient(a);
	}
	return recipeToConstruct;
}

string RecipeScraper::ExtractSpanTag(std::string& line, std::string spanOpener)
{
	int recipeTitleStartIndex = line.find(spanOpener);
	if (recipeTitleStartIndex != string::npos)
	{
		int recipeTitleEndIndex = line.find("</span>");
		recipeTitleEndIndex += 7;
		smatch name;
		regex units("^<.*!important;\">(.*)?<.*>$");
		string nameSpan = line.substr(recipeTitleStartIndex, recipeTitleEndIndex - recipeTitleStartIndex);
		if (regex_match(nameSpan, name, units))
		{
			return name[1];
		}
		else
		{
			cout << "No Match For Name: " << nameSpan << endl;
		}
	}
	return "";
}

void RecipeScraper::ExtractIngredients(std::string& line, std::vector<Ingredient>& IngredientsInRecipe)
{
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
	} while (ingredientClassStartIndex != string::npos);
}

Ingredient RecipeScraper::ExtractSpanTagsIntoIngredient(std::string& ingredientClass)
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
				regex noDigits("^<.*!important;\">(.*)?<.*>$");
				if (regex_match(spanString, amount, noDigits))
				{
					ingredientToConstruct.setMeasurement(amount[1]);
				}
				else
				{
					cout << "No Match For Measurement: " << spanString << endl;
				}
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
				cout << "No Match For Unit: " << spanString << endl;
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
				cout << "No Match For Ingredient: " << spanString << endl;
			}
		}
		ingredientClassSpanStart = ingredientClass.find("<span", ingredientClassSpanEnd);
		index++;
	} while (ingredientClassSpanStart != string::npos); // Go through all <span> tags
	return ingredientToConstruct;
}