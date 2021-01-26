#include "ParameterSet.h"



ParameterSet::ParameterSet()
{
	srand(time(NULL));
}


ParameterSet::~ParameterSet()
{
}

void ParameterSet::regenerateWType()
{
	wType = rand() % 3;
}

void ParameterSet::regenerateNBlades()
{
	int temp = rand() % 2;
	if (temp == 0)
	{
		nBlades = true;
	}
	else
	{
		nBlades = false;
	}
}

void ParameterSet::regenerateBLength()
{
	bLength = rand() & BLADE_MAX_LENGTH + BLADE_MIN_LENGTH;
}

void ParameterSet::regenerateBBreadth()
{
	bBreadth = rand() & BLADE_MAX_BREADTH + BLADE_MIN_BREADTH;
}

void ParameterSet::regenerateBWidth()
{
	bWidth = rand() & BLADE_MAX_WIDTH + BLADE_MIN_WIDTH;
}

void ParameterSet::regenerateBCurve()
{
	int temp = rand() & BLADE_MAX_CURVE + BLADE_MIN_CURVE;
	float t2 = temp / 10000000000;
	bCurve = t2;
}

void ParameterSet::regenerateHLength()
{
	hLength = rand() & HILT_MAX_LENGTH + HILT_MIN_LENGTH;
}

void ParameterSet::regenerateHRadius()
{
	hRadius = rand() & HILT_MAX_RADIUS + HILT_MIN_RADIUS;
}

void ParameterSet::regenerateHTRadius()
{
	hTRadius = rand() & HILTTAPER_MAX_RADIUS + HILTTAPER_MIN_RADIUS;
}

void ParameterSet::regeneratePRadius()
{
	pRadius = rand() & POMMEL_MAX_RADIUS + POMMEL_MIN_RADIUS;
}

void ParameterSet::regeneratePStyle()
{
	pStyle = rand() % 6;
}

void ParameterSet::regenerateMaterial()
{
	material = rand() % 3;
}

void ParameterSet::regenerateMColour()
{
	mColour = rand() % 10;
}

void ParameterSet::regenerateGColour()
{
	gColour = rand() % 6;
}

void ParameterSet::regenerateCRadius()
{
	cRadius = rand() % CROSSGUARD_MAX_RADIUS + CROSSGUARD_MIN_RADIUS;
}

void ParameterSet::regenerateCHeight()
{
	cHeight = rand() % CROSSGUARD_MAX_HEIGHT + CROSSGUARD_MIN_HEIGHT;
}

void ParameterSet::regenerateCShape()
{
	cShape = rand() % 4;
}

void ParameterSet::regenerateCBAngle()
{
	int temp = rand() % CROSSGUARD_MAX_BEND + CROSSGUARD_MIN_BEND;
	float temp2 = temp / 10000000000;
	cBAngle = temp2;
}

void ParameterSet::regenerateNEmboss()
{
	int temp = rand() % 5;
	if (temp == 0)
	{
		nEmboss = 3;
	}
	else
	{
		nEmboss = temp * 2;
	}
}

void ParameterSet::regenerateCStyle()
{
	cStyle = rand() % 4;
}

void ParameterSet::regeneratePQuality()
{
	pQuality = rand() % 4;
}

void ParameterSet::regenerateWCondition()
{
	wCondition = rand() % 5;
}

void ParameterSet::regenerateHWStyle()
{
	hWStyle = rand() % 3;
}

void ParameterSet::regenerateHStyle()
{
	hStyle = rand() % 4;
}

void ParameterSet::regenerateHCLength()
{
	hCLength = rand() % HILTCOLLAR_MAX_LENGTH + HILTCOLLAR_MIN_LENGTH;
}

void ParameterSet::regenerateHCAngle()
{
	int temp = rand() % HILTCOLLAR_MAX_ANGLE + HILTCOLLAR_MIN_ANGLE;
	float temp2 = temp / 10000000000;
	hCAngle = temp2;
}

void ParameterSet::regenerateWStyle()
{
	wStyle = rand() % 5;
}

void ParameterSet::regenerateCLength()
{
	cLength = rand() % COLLAR_MAX_LENGTH + COLLAR_MIN_LENGTH;
}

void ParameterSet::regenerateCoStyle()
{
	coStyle = rand() % 3;
}

void ParameterSet::regenerateCAmount()
{
	cAmount = rand() % CUTAWAY_MAX_AMOUNT + CUTAWAT_MIN_AMOUNT;
}

void ParameterSet::regenerateFWidth()
{
	fWidth = rand() % FULLER_MAX_WIDTH + FULLER_MIN_WIDTH;
}

void ParameterSet::regenerateFLength()
{
	fLength = rand() % FULLER_MAX_LENGTH + FULLER_MIN_LENGTH;
}

void ParameterSet::regenerateFDepth()
{
	fDepth = rand() & FULLER_MAX_DEPTH + FULLER_MIN_DEPTH;
}

void ParameterSet::regenerateHamon()
{
	int temp = rand() % 2;
	if (temp == 0)
	{
		hamon = true;
	}
	else
	{
		hamon = false;
	}
}

void ParameterSet::regenerateHOffset()
{
	hOffset = rand() % HAMON_MAX_OFFSET + HAMON_MIN_OFFSET;
}

void ParameterSet::regeneratePLength()
{
	pLength = rand() % POINT_MAX_LENGTH + POINT_MIN_LENGTH;
}

void ParameterSet::regenerateEType()
{
	eType = rand() % 8;
}

void ParameterSet::regenerateBStyle()
{
	int temp = rand() % 2;
	if (temp == 0)
	{
		bStyle = true;
	}
	else
	{
		bStyle = false;
	}
}

void ParameterSet::regenerateHGAmount()
{
	hGAmount = rand() % HOLLOWGRIND_MAX + HOLLOWGRIND_MIN;
}

void ParameterSet::regenerateStWidth()
{
	stWidth = rand() % SPEARTIP_MAX_WIDTH + SPEARTIP_MIN_WIDTH;
}

void ParameterSet::regenerateSBHeight()
{
	sBHeight = rand() % SPEARBASE_MAX_HEIGHT + SPEARBASE_MIN_HEIGHT;
}

void ParameterSet::regenerateDamage()
{
	regenerateRLevel();
	regenerateCLevel();
}

void ParameterSet::regenerateRLevel()
{
	rLevel = rand() % 10;
}

void ParameterSet::regenerateCLevel()
{
	cLevel = rand() % 10;
}

