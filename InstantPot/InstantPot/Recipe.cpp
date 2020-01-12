#include "Recipe.h"

void Recipe::parseRecipe(string recipeName)
{
	ifstream recipeFile;
	recipeFile.open(recipeName);
	if (recipeFile)
	{
		string line;
		ParseState currentSection = E_NAME;
		while (getline(recipeFile, line))
		{
			if (line[0] == '#')
			{
				currentSection = getSection(line);
			}
			else
			{
				parseLine(line, currentSection);
			}
		}
	}
}

Ingredient Recipe::getIngredientFromLine(string line)
{
	Ingredient ingredient;
	ingredient.parseLine(line);
	return ingredient;
}
string Recipe::printRecipe()
{
	string output;
	output += "Name: ";
	name += name;
	name += '\n';
	name += "Ingredients:";
	name += '\n';
	for (int i = 0; i < Ingredients.size(); i++)
	{
		name += Ingredients[i].output();
		name += '\n';
	}
	name += "Instructions:";
	name += '\n';
	for (auto a : Instructions)
	{
		name += a;
		name += '\n';
	}
	return name;
}
ParseState Recipe::getSection(string line)
{
	if (line == "#NAME#")
	{
		return E_NAME;
	}
	else if (line == "#INGREDIENTS#")
	{
		return E_INGREDIENTS;
	}
	else if (line == "#INSTRUCTIONS#")
	{
		return E_INSTRUCTIONS;
	}
	else if (line == "#NOTES#")
	{
		return E_NOTES;
	}
}
void Recipe::parseLine(string line, ParseState currentSection)
{
	switch (currentSection)
	{
	case E_NAME:
		name = line;
		break;
	case E_INGREDIENTS:
		Ingredients.push_back(getIngredientFromLine(line));
		break;
	case E_INSTRUCTIONS:
		Instructions.push_back(line);
		break;
	case E_NOTES:
		Notes.push_back(line);
		break;
	default:
		break;
	}
}