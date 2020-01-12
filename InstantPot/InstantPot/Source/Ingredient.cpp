#include "Ingredient.h"

string Ingredient::output()
{
	if (noAmountFound)
	{
		return errorString;
	}
	else
	{
		return parseError;
	}
}

void Ingredient::parseLine(string line)
{
	findMeasurement(line);
	parseError = measurementAmountString;
	parseError += "|";
	parseError += measuremeantType;
	parseError += "|";
	parseError += ingredientName;
}

void Ingredient::findMeasurement(string line)
{
	for (int i = 0; i < measurementTypes->size(); i++)
	{
		int index = 0;
		index = line.find(measurementTypes[i]);
		if (index != string::npos)
		{
			noAmountFound = false;
			measurement = (EMeasurementType)i;
			measurementAmountString = line.substr(0, index-1);
			measuremeantType = measurementTypes[i];
			ingredientName = line.substr(line.find(" ", index) + 1);
			return;
		}
		else
		{
			noAmountFound = true;
			errorString = line;
		}
	}
}