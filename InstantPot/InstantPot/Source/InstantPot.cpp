#include "RecipeScraper.h"
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;
void iterateThruRecipes();
vector<Recipe> GetAllRecipesInFileSystem();

static uint32_t s_AllocCount = 0;

void* operator new(size_t size)
{
	s_AllocCount++;
	return malloc(size);
}

vector<Recipe> bubbleSort(vector<Recipe> unsortedRecipies)
{
	bool unsorted = true;
	while(unsorted)
	{
		unsorted = false;
		for (int i = 0; i < unsortedRecipies.size()-1; i++)
		{
			if (unsortedRecipies[i + 1].hasLessIngredients(unsortedRecipies[i]))
			{
				Recipe temp = unsortedRecipies[i+1];
				unsortedRecipies[i+1] = unsortedRecipies[i];
				unsortedRecipies[i] = temp;
				unsorted = true;
			}
		}
	}
	return unsortedRecipies;
}

using namespace std::chrono;


int main()
{	
	auto start = high_resolution_clock::now();

	RecipeScraper a;
	//a.BeginScrape();
	vector<Recipe> AlphabeticallySortedRecipies;
	AlphabeticallySortedRecipies = GetAllRecipesInFileSystem();
	
	sort(AlphabeticallySortedRecipies.begin(), AlphabeticallySortedRecipies.end(), [](Recipe &a, Recipe &b)
	{
		return a.numberOfIngredients > b.numberOfIngredients;
	});

	for (auto& a : AlphabeticallySortedRecipies)
	{
		cout << a.getName() << " has: " << a.getAllIngredients().size() << " ingredients." << endl;
	}
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<milliseconds>(stop - start);
	cout << duration.count() << endl;
	cout << "Allocations: " << s_AllocCount << endl;
}

vector<Recipe> GetAllRecipesInFileSystem()
{
	vector<Recipe> AllRecipies;
	AllRecipies.reserve(1000);
	string path = "Recipes/";
	
	for (const auto& entry : fs::directory_iterator(path))
	{
		Recipe recipe;
		recipe.parseRecipe(entry);
		AllRecipies.push_back(recipe);
	}
	return AllRecipies;
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
	
	for (int i = 0; i < allCompatibleRecipes.size(); i++)
	{
		cout << i + 1 << ". For " << allCompatibleRecipes[i].getRecipeName() << " you have ";
		cout << allCompatibleRecipes[i].getNumberOfSimiliarIngredients() << " / " << allCompatibleRecipes[i].getAllIngredients().size() << " Ingredients. \n";
	}
}