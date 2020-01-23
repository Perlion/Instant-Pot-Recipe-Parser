#include "Recipe.h"
#include <regex>
#include <iomanip>
void Recipe::parseRecipe(string recipeName)
{
	ifstream recipeFile;
	recipeFile.open(recipeName);
	if (recipeFile)
	{
		string line;
		ParseState currentSection;
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

void Recipe::Serialize()
{
	ofstream outFile;
	string outputPath = "C:/Users/UE4Jam/Documents/GitHub/Instant-Pot-Recipe-Parser/InstantPot/InstantPot/Recipes/";
	if (name.empty())
	{
		outputPath += "invalid";
	}
	else
	{
		outputPath += name;
	}
	outputPath += ".txt";
	outFile.open(outputPath);
	outFile << "#NAME#" << endl;
	if(!name.empty())
	outFile << name << endl;

	outFile << "#INGREDIENTS#" << endl;
	for (auto a : Ingredients)
	{
		if (!a.getMeasurement().empty())
			outFile << a.getMeasurement() << " ";
		if (!a.getMeasurementType().empty())
			outFile << a.getMeasurementType() << " ";
		if(!a.getName().empty())
			outFile << a.getName() << endl;
	}
	outFile << "#LINK#" << endl;
	
	if(!recipeURL.empty())
	outFile << recipeURL << endl;
	
	outFile << "#TIME#" << endl;
	if(!recipeURL.empty())
	outFile << cookTime << endl;
}

Ingredient Recipe::getIngredientFromLine(string line)
{
	Ingredient ingredient;
	int firstSpaceIndex = line.find(' ');
	int secondSpaceIndex = line.find(' ', firstSpaceIndex+1);
	if (firstSpaceIndex != string::npos)
	{
		ingredient.setMeasurement(line.substr(0, firstSpaceIndex));
		ingredient.setMeasurementType(line.substr(firstSpaceIndex + 1, (secondSpaceIndex - firstSpaceIndex) - 1));
		ingredient.setIngredientName(line.substr(secondSpaceIndex + 1));
	}
	return ingredient;
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
	else if (line == "#LINK#")
	{
		return E_LINK;
	}
	else if (line == "#TIME#")
	{
		return E_TIME;
	}
}

void Recipe::parseLine(string line, ParseState currentSection)
{
	if (line.empty())
		return;
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
	case E_LINK:
		recipeURL = line;
		break;
	case E_TIME:
		cookTime = line;
		break;
	default:
		break;
	}
}

ostream& operator<<(ostream& out, const Recipe& c)
{
	out << left << setw(20) << c.name << endl;
	out << left << setw(50) << "Amount" << left << setw(50) << "Type" << left << setw(50) << "Name" << endl;
	for (auto a : c.Ingredients)
	{
		out << left << setw(50) << a.getMeasurement();
		out << left << setw(50) << a.getMeasurementType();
		out << left << setw(50) << a.getName() << endl;
	}
	out << left << setw(20) << "Cook Time: " << c.cookTime << " minutes" << endl;
	return out;
}

bool operator< (const Recipe& recipe1, const Recipe& recipe2)
{
	return recipe1.Ingredients.size() < recipe1.Ingredients.size();
}

bool operator> (const Recipe& recipe1, const Recipe& recipe2)
{
	return !(recipe1 < recipe2);
}