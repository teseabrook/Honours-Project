#pragma once
#include "ParameterSet.h"

class AICore
{
public:
	AICore();
	~AICore();

	void setInputParameters(ParameterSet* in) {inputParams = in;};

	ParameterSet* generateParameterSet();

protected:
	ParameterSet* P[10];
	ParameterSet* inputParams;

	bool evaluateParametersAgainstRules(ParameterSet* input);
	int evaluateParametersAgainstCriteria(ParameterSet* input);

	void generateGeneration(ParameterSet* in);
	void generateInitialGeneration();

};

