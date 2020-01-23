#pragma once
#include <string>
using namespace std;
enum EMeasurementType
{
	E_TABLESPOON,
	E_TEASPOON,
	E_CUP,
	E_POUND,
	E_OUNCE,
	E_STRIP
};
class Ingredient
{
public:
	void setMeasurement(string measurementInput) { measurementAmountString = measurementInput; }
	void setIngredientName(string ingreName) { ingredientName = ingreName; }
	void setMeasurementType(string measurementTypeInput) { measuremeantType = measurementTypeInput; }
	string getName() { return ingredientName; }
	string getMeasurementType() { return measuremeantType; }
	string getMeasurement() { return measurementAmountString; }
	string getIngredientName() { return ingredientName; }
	string output();
	void parseLine(string line);
	void findMeasurement(string line);
private:
	string measurementTypes[6] = { "tablespoon", "teaspoon", "cup", "pound", "ounce", "strip"};
	
private:
	bool noAmountFound = false;
	string errorString;
	float amount;
private:
	string measuremeantType;
	string ingredientName;
	string measurementAmountString;
private:
	EMeasurementType measurement;
	string parseError;
};