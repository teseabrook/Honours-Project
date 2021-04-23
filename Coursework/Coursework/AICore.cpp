#include "AICore.h"


//Set appropriate functions, start the random seed and begin generation
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

//Generate a set of parameters that fulfills all rules and has been ran through the algorithm
ParameterSet* AICore::generateParameterSet()
{
	//Generate the first set
	generateInitialGeneration();
	
	//Loop until we've had n generations satisfy the rules
	int n = 0;
	do
	{
		int highestScore = -10000000;
		int winner = -1;

		//Loop through each member
		for (int i = 0; i < numMembers; i++)
		{
			//Check to see if it meets all the rules
			if (evaluateParametersAgainstRules(P[i]))
			{
				//Give it a score based on the criteria, then evaluate to see if it's the highest scoring one
				int pScore = evaluateParametersAgainstCriteria(P[i]);
				if (pScore >= highestScore)
				{
					highestScore = pScore;
					winner = i;
				}
			}
		}

		//If at least one member passed the rules, generate a new generation using it as a base and increment n
		if (winner != -1)
		{
			generateGeneration(P[winner]);
			n++;
		}
		//All members failed, so regenerate all and don't incremement n
		else
		{
			generateInitialGeneration();
		}
	} while (n < numGenerations);


	//Return the member P[n - 1], as that is the winner of the last generation
	return &P[numMembers - 1];
}

//Parse the json to get the rules and criteria for the AI
bool AICore::parseJSON(const char* rPath, const char* cPath)
{
	rapidjson::Document doc, doc2;

	//Open the file
	std::ifstream stream(rPath);
	rapidjson::IStreamWrapper wrapper(stream);

	//Parse the file
	doc.ParseStream(wrapper);

	//Process the data
	bool result = getRulesFromFile(doc);

	//Open the criteria file and do the same as with the rule file
	std::ifstream stream2(cPath);
	rapidjson::IStreamWrapper wrapper2(stream2);
	doc2.ParseStream(wrapper2);

	bool r2 = getCriteriaFromFile(doc2);

	//If either failed, return false
	bool retVal = true;

	if (result == false || r2 == false)
	{
		retVal = false;
	}

	return retVal;
}

//Extract the rules from the file using RapidJSON
bool AICore::getRulesFromFile(rapidjson::Document & doc)
{
	const rapidjson::Value& rulesArr = doc["rules"];

	//Loop through all found rules
	for (int i = 0; i < rulesArr.Size(); i++)
	{
		Rule r;

		//A rule will always have at least 1 parameter and an operator
		r.param1 = rulesArr[i]["parameter1"].GetString();
		r.op = rulesArr[i]["operator"].GetString();

		//If there's a second parameter, save it
		if (rulesArr[i].HasMember("parameter2"))
		{
			r.param2 = rulesArr[i]["parameter2"].GetString();
		}
		else
		{
			r.param2 = "none";
		}

		//If there's a value, save it
		if (rulesArr[i].HasMember("value"))
		{
			r.value = rulesArr[i]["value"].GetFloat();
		}
		else
		{
			r.value = 0.0f;
		}

		//Save the extracted rule
		rules.push_back(r);
	}

	return true;
}

//Extract the criteria from the file
bool AICore::getCriteriaFromFile(rapidjson::Document & doc)
{
	const rapidjson::Value& critArr = doc["criteria"];

	//Loop through the criteria
	for (int i = 0; i < critArr.Size(); i++)
	{
		Criteria c;

		//A criteria always has a parameter and type
		c.param1 = critArr[i]["parameter1"].GetString();
		c.ruleType = critArr[i]["ctype"].GetString();

		//If it has a multiplicitave value, save it
		if (critArr[i].HasMember("multiplier"))
		{
			c.multiplier = critArr[i]["multiplier"].GetFloat();
		}
		else
		{
			c.multiplier = 1.0f;
		}

		if (critArr[i].HasMember("parameter2"))
		{
			c.param2 = critArr[i]["parameter2"].GetString();
		}
		else
		{
			c.param2 = "none";
		}

		//Save the criteria
		criteria.push_back(c);
	}

	return true;
}

//Generate a set of parameters independant of all previos generated ones
void AICore::generateInitialGeneration()
{
	P.clear();

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
		ParameterSet p;

		//Set appropriate parameters to the user input
		if (hasUserInput)
		{
			p = *inputParams;
		}
		
		//Regenerate all statistics
		//User provided statistics will be locked so they won't be regenerated
		eventSystem->addEvent("SYSTEM", "Generating Parameter Set", "AI");
		p.regenerateAll();
		P.push_back(p);

	}

}

//Generate a generation based on the winner of the last generation
void AICore::generateGeneration(ParameterSet arg)
{
	//Clear the container
	P.clear();

	//Loop through the others and regenerate them
	for (int i = 0; i < numMembers - 1; i++)
	{
		ParameterSet set = arg;

		//Mutate a random amount of members
		int mutations = rand() % NUM_PARAMS;
		set.regenerateAmount(mutations);
		P.push_back(set);
	}

	//Set the last member to the winner of the last generation, such that we always have at least 1 member that can pass
	P.push_back(arg);
}

//Test the input against all rules
bool AICore::evaluateParametersAgainstRules(ParameterSet input)
{
	//Rules here
	for (int i = 0; i < rules.size(); i++)
	{
		//The rule is the form x > y, meaning it will fail if x is less than y
		if (rules[i].op == ">")
		{
			if (rules[i].param2 != "none")
			{
				if (getRuleValueAsFloat(rules[i].param1, input) < getRuleValueAsFloat(rules[i].param2, input))
				{
					return false;
				}
			}
			else if (getRuleValueAsFloat(rules[i].param1, input) < rules[i].value)
			{
				return false;
			}
			
		}

		//The rule is the form x == y, meaning it will fail if x != y
		if (rules[i].op == "=")
		{
			if (getRuleValueAsFloat(rules[i].param1, input) != rules[i].value)
			{
				return false;
			}
		}

		//The rule is the form x != y, meaning it will fail if x == y
		if (rules[i].op == "!=")
		{
			if (getRuleValueAsFloat(rules[i].param1, input) == rules[i].value)
			{
				return false;
			}
		}

		//The rule is that this cannot be true while the second part is also true
		if (rules[i].op == "c=")
		{
			if (getRuleValueAsFloat(rules[i].param1, input) == rules[i].value)
			{
				if (getRuleValueAsFloat(rules[i + 1].param1, input) == rules[i + 1].value)
				{
					return false;
				}
			}
		}
	}

	//We passed
	return true;
}

//Go through the criteria and store the score
int AICore::evaluateParametersAgainstCriteria(ParameterSet input)
{
	int score = 0;

	for (int i = 0; i < criteria.size(); i++)
	{
		//Increase the score based on the absolute value of the parameter multiplied by the multiplier
		if (criteria[i].ruleType == "absMult")
		{
			score += (int)(getRuleValueAsFloat(criteria[i].param1, input) * criteria[i].multiplier);
		}
		//The larger the difference between the parameters, the lower the score
		else if (criteria[i].ruleType == "invdiff")
		{
			if (criteria[i].param2 == "none")
			{
				float diff = abs(getRuleValueAsFloat(criteria[i].param1, input) - criteria[i].multiplier);
				score -= diff;
			}
			else
			{
				float diff = abs(getRuleValueAsFloat(criteria[i].param1, input) - getRuleValueAsFloat(criteria[i].param2, input));
				score -= diff;
			}
		}
	}

	return score;
}

//Brute force method to get the value as other methods wouldn't behave
float AICore::getRuleValueAsFloat(std::string param, ParameterSet input)
{
	//It doesnt like strings or const chars being used in switch statements

	//Blade breadth
	if (param == "bBreadth")
	{
		return input.getBBreadth();
	}
	//Blade length
	else if (param == "bLength")
	{
		return input.getBLength();
	}
	//Hilt Style
	else if (param == "hStyle")
	{
		return input.getHStyle();
	}
	//Hilt Wrapping Style
	else if (param == "hWStyle")
	{
		return input.getHWStyle();
	}
	//Pommel Style
	else if (param == "pStyle")
	{
		return input.getPStyle();
	}
	//Crossguard Shape
	else if (param == "cShape")
	{
		return input.getCShape();
	}
	//Crossguard Style
	else if (param == "cStyle")
	{
		return input.getCStyle();
	}
	//Weapon Type
	else if (param == "wType")
	{
		return input.getWType();
	}
	//Crossguard Bend Angle
	else if (param == "cBAngle")
	{
		return input.getCBAngle();
	}
	//Hilt Radius
	else if (param == "hRadius")
	{
		return input.getHRadius();
	}
	//Hilt Length
	else if (param == "hLength")
	{
		return input.getHLength();
	}
	//Hilt Taper Radius
	else if (param == "hTRadius")
	{
		return input.getHTRadius();
	}
	//Hilt Collar Length
	else if (param == "hCLength")
	{
		return input.getHCLength();
	}
	//Number of blades
	else if (param == "nBlades")
	{
		return input.getNBlades();
	}
	else
	{
		//Parameter doesn't exist or isn't a float
		return -1;
	}
}