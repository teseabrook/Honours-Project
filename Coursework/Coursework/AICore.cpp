#include "AICore.h"



AICore::AICore(DebugEventSystem* e, const char* rPath, const char* cPath, int a_numMembers, int a_numGens)
{
	numGenerations = a_numGens;
	numMembers = a_numMembers;
	eventSystem = e;
	srand(time(NULL));
	parseJSON(rPath, cPath);
}


AICore::~AICore()
{
}

ParameterSet* AICore::generateParameterSet()
{
	generateInitialGeneration();
	int n = 0;

	do
	{

		int highestScore = 0;
		int winner = -1;

		for (int i = 0; i < numMembers; i++)
		{
			if (evaluateParametersAgainstRules(P[i]))
			{
				int pScore = evaluateParametersAgainstCriteria(P[i]);
				if (pScore > highestScore)
				{
					highestScore = pScore;
					winner = i;
				}
			}
		}

		if (winner != -1)
		{
			generateGeneration(P[winner]);
		}
		else
		{
			//All parameters failed, regenerate them all
			generateInitialGeneration();
		}
		n++;
	} while (n < numGenerations);



	return P[numMembers - 1];
}

bool AICore::parseJSON(const char* rPath, const char* cPath)
{
	//Parse the json to get the rules and criteria for the AI
	rapidjson::Document doc, doc2;

	//Open the file
	std::ifstream stream(rPath);
	rapidjson::IStreamWrapper wrapper(stream);

	//Parse the file
	doc.ParseStream(wrapper);

	//Process the data
	bool result = getRulesFromFile(doc);

	//Open the criteria file
	std::ifstream stream2(cPath);
	rapidjson::IStreamWrapper wrapper2(stream2);
	doc2.ParseStream(wrapper2);

	bool r2 = getCriteriaFromFile(doc2);

	bool retVal = true;

	if (result == false || r2 == false)
	{
		retVal = false;
	}

	return retVal;
}

bool AICore::getRulesFromFile(rapidjson::Document & doc)
{
	const rapidjson::Value& rulesArr = doc["rules"];

	for (int i = 0; i < rulesArr.Size(); i++)
	{
		Rule r;
		r.param1 = rulesArr[i]["parameter1"].GetString();
		r.op = rulesArr[i]["operator"].GetString();

		if (rulesArr[i].HasMember("parameter2"))
		{
			r.param2 = rulesArr[i]["parameter2"].GetString();
		}
		else
		{
			r.param2 = "none";
		}

		if (rulesArr[i].HasMember("value"))
		{
			r.value = rulesArr[i]["value"].GetFloat();
		}
		else
		{
			r.value = 0.0f;
		}

		rules.push_back(r);
	}

	return true;
}

bool AICore::getCriteriaFromFile(rapidjson::Document & doc)
{
	const rapidjson::Value& critArr = doc["criteria"];

	for (int i = 0; i < critArr.Size(); i++)
	{
		Criteria c;

		c.param1 = critArr[i]["parameter1"].GetString();
		c.ruleType = critArr[i]["ruletype"].GetString();

		if (critArr[i].HasMember("multiplier"))
		{
			c.multiplier = critArr[i]["multiplier"].GetFloat();
		}
		else
		{
			c.multiplier = 1.0f;
		}

		criteria.push_back(c);
	}

	return true;
}

void AICore::generateInitialGeneration()
{
	eventSystem->addEvent("SYSTEM", "Generating Initial Parameters", "AICore");
	//First, check to see if there's user input
	bool hasUserInput = false;
	if (inputParams)
	{
		hasUserInput = true;
	}

	//Now, loop 10 times and generate statistics
	for (int i = 0; i < numMembers; i++)
	{
		ParameterSet* p = new ParameterSet;

		if (hasUserInput)
		{
			p = inputParams;
		}
		else
		{
			p = new ParameterSet();
		}
		
		//Regenerate all statistics
		//User provided statistics will be locked so they won't be regenerated

		eventSystem->addEvent("SYSTEM", "Generating Parameter Set", "AI");
		p->regenerateAll();
		P.push_back(p);

	}

}

void AICore::generateGeneration(ParameterSet* arg)
{
	//Set the last P to the winner of the generation
	P[numMembers - 1] = arg;

	//Loop through the others and regenerate them
	for (int i = 0; i < numMembers - 1; i++)
	{
		//Set the parameter to the winner of the generation
		P[i] = arg;

		//Mutate a random number of parameters
		int mutations = rand() % NUM_PARAMS;
		P[i]->regenerateAmount(mutations);

	}
}

bool AICore::evaluateParametersAgainstRules(ParameterSet* input)
{
	//Rules here
	for (int i = 0; i < rules.size(); i++)
	{
		if (rules[i].op == ">")
		{
			if (rules[i].param2 != "none")
			{
				if (getRuleValueAsFloat(rules[i].param1, input) > getRuleValueAsFloat(rules[i].param2, input))
				{
					return false;
				}
				else if (getRuleValueAsFloat(rules[i].param1, input) > rules[i].value)
				{
					return false;
				}
			}
		}
	}

	//We passed
	return true;
}

int AICore::evaluateParametersAgainstCriteria(ParameterSet* input)
{
	int score = 0;

	//Arbritary test criteria. The longer the blade is, the higher the score.
	score += input->getBLength() / 100;

	return score;
}

float AICore::getRuleValueAsFloat(std::string param, ParameterSet* input)
{
	//It doesnt like strings or const chars being used in switch statements
	if (param == "bBreadth")
	{
		return input->getBBreadth();
	}
	else if (param == "bLength")
	{
		return input->getBLength();
	}
	else
	{
		//Parameter doesn't exist or is a float
		return -1;
	}
}