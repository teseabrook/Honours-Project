#include "AICore.h"



AICore::AICore()
{
	srand(time(NULL));
}


AICore::~AICore()
{
}

void AICore::generateInitialGeneration()
{
	//First, check to see if there's user input
	bool hasUserInput = false;
	if (inputParams)
	{
		hasUserInput = true;
	}

	//Now, loop 10 times and generate statistics
	for (int i = 0; i < 10; i++)
	{
		if (hasUserInput)
		{
			P[i] = inputParams;
		}
		else
		{
			P[i] = new ParameterSet();
		}
		
		//Regenerate all statistics
		//User provided statistics will be locked so they won't be regenerated
		P[i]->regenerateAll();
	}

}

void AICore::generateGeneration(ParameterSet* arg)
{
	//Set the last P to the winner of the generation
	P[9] = arg;

	//Loop through the others and regenerate them
	for (int i = 0; i < 9; i++)
	{
		//Set the parameter to the winner of the generation
		P[i] = arg;

		//Mutate a random number of parameters
		int mutations = rand() % NUM_PARAMS;
		P[i]->regenerateAmount(mutations);

	}
}
