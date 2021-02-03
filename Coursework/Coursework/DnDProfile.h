#pragma once
#include <string>
#include "ParameterSet.h"

//Container class for making a DnD profile from a generated weapon

class DnDProfile
{
public:
	DnDProfile(ParameterSet* set);
	~DnDProfile();

	std::string getWeaponName() { return type; };
	int getDamageBonus() { return damageBonus; };
	std::string getEffectDice() { return effectDamageDice; };
	std::string getEffectType() { return effectType; };


protected:
	std::string type;
	int damageBonus;
	std::string effectDamageDice;
	std::string effectType;

	void setName(ParameterSet* set);
	void setDamageBonus(ParameterSet* set);
	void setEffect(ParameterSet* set);
};

