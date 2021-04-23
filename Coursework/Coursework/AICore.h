#pragma once

/*
 * AICore.h
 * Core class for the system's AI
 * Deals with loading of rules and parameters into memory from JSON, as well as generation of parameters and training of AI
 */

#include "ParameterSet.h"
#include "DebugEventSystem.h"

#include <vector>
#include <rapidjson\document.h>
#include <rapidjson\istreamwrapper.h>
#include <rapidjson\stringbuffer.h>
#include <rapidjson\writer.h>
#include <fstream>

class AICore
{
public:
	AICore(DebugEventSystem* e, const char* rPath, const char* cPath, int a_numMembers = 10, int a_numGens = 10);
	~AICore();

	struct Rule
	{
		std::string param1;
		std::string param2;
		std::string op;
		float value;
	};

	struct Criteria
	{
		std::string param1;
		std::string param2;
		float multiplier;
		std::string ruleType;
	};

	//Set and lock some of the input parameters to user input
	void setInputParameters(ParameterSet* in) {inputParams = in;};

	//Regenerate the parameter set. Called automatically on class creation
	ParameterSet* generateParameterSet();

protected:
	//Data Structures
	std::vector<ParameterSet> P;
	std::vector<Rule> rules;
	std::vector<Criteria> criteria;

	//Variables
	ParameterSet* inputParams;
	DebugEventSystem* eventSystem;
	int numGenerations, numMembers;

	//AI Control Functions
	bool evaluateParametersAgainstRules(ParameterSet input);
	int evaluateParametersAgainstCriteria(ParameterSet input);

	//Generation Functions
	void generateGeneration(ParameterSet in);
	void generateInitialGeneration();

	//JSON Parsing Functions
	bool parseJSON(const char* rPath, const char* cPath);
	bool getRulesFromFile(rapidjson::Document & doc);
	bool getCriteriaFromFile(rapidjson::Document & doc);
	float getRuleValueAsFloat(std::string value, ParameterSet input);

};

