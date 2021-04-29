#include "DnDProfile.h"



DnDProfile::DnDProfile(ParameterSet* set)
{
	setName(set);
	setDamageBonus(set);
	setEffect(set);
}


DnDProfile::~DnDProfile()
{
}

void DnDProfile::setName(ParameterSet* set)
{
	std::string n = "";
	if (set->getEType() != 7)
	{
		n += "Magical ";
	}

	switch (set->getWType())
	{
		case 0:
			//Swords
			//Blade is between 0 and 4000 with 4 categories
			//If handle is long, it's a glaive

			if (set->getHLength() > 3000)
			{
				n += "Glaive";
			}
			else
			{
				if (set->getBLength() < 1000)
				{
					n += "Dagger";
				}
				else if (set->getBLength() < 2000)
				{
					n += "Shortsword";
				}
				else if (set->getBLength() < 3000)
				{
					n += "Longsword";
				}
				else
				{
					n  += "Greatsword";
				}
			}

			break;

		case 1:
			//Axes/Halberds
			if (set->getHLength() > 1000)
			{
				n += "Halberd";
			}
			else
			{
				if (set->getBLength() < 667)
				{
					n += "Handaxe";
				}
				else if (set->getBLength() < 1333)
				{
					n += "Battleaxe";
				}
				else
				{
					n += "Greataxe";
				}
			}
			break;

		case 2:
			//Spears and stuff
			if (set->getHLength() > 2000)
			{
				n += "Spear";
			}
			else
			{
				n += "Javelin";
			}
			break;

		default:
			//How'd you get here?
			n += "ERROR";
			break;
	}

	type = n;
}

void DnDProfile::setDamageBonus(ParameterSet* set)
{
	int bonus = 0;

	//Get the quality
	bonus += set->getPQuality();

	//Get the condition
	if (set->getWCondition() == 0)
	{
		bonus++;
	}
	else
	{
		bonus -= (set->getWCondition() - 1);
	}

	damageBonus = bonus;
}

void DnDProfile::setEffect(ParameterSet* set)
{
	//Damage dice
	switch (set->getPQuality())
	{
		case 0:
			effectDamageDice = "1";
			break;

		case 1:
			effectDamageDice = "d4";
			break;

		case 2:
			effectDamageDice = "2d4";
			break;

		case 3:
			effectDamageDice = "1d10";
			break;
	}

	//Effect type
	switch (set->getEType())
	{
		//0: Magic, 1: Fire, 2: Ice, 3: Wind, 4: Toxic, 5: Electric, 6: Holy, 7: None
		case 0:
			effectType = "Psychic";
			break;

		case 1:
			effectType = "Fire";
			break;

		case 2:
			effectType = "Cold";
			break;

		case 3:
			effectType = "Force";
			break;

		case 4:
			effectType = "Poison";
			break;

		case 5:
			effectType = "Lightning";
			break;

		case 6:
			effectType = "Radiant";
			break;

		default:
			effectType = "None";
			break;
	}
}
