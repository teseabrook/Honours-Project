#pragma once
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

	void setInputParameters(ParameterSet* in) {inputParams = in;};

	ParameterSet* generateParameterSet();

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
		float multiplier;
		std::string ruleType;
	};

protected:
	std::vector<ParameterSet> P;
	ParameterSet* inputParams;

	std::vector<Rule> rules;
	std::vector<Criteria> criteria;

	bool evaluateParametersAgainstRules(ParameterSet input);
	int evaluateParametersAgainstCriteria(ParameterSet input);

	void generateGeneration(ParameterSet in);
	void generateInitialGeneration();

	bool parseJSON(const char* rPath, const char* cPath);
	bool getRulesFromFile(rapidjson::Document & doc);
	bool getCriteriaFromFile(rapidjson::Document & doc);

	float getRuleValueAsFloat(std::string value, ParameterSet input);

	DebugEventSystem* eventSystem;

	int numGenerations, numMembers;

};

