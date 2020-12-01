#include "ParameterSet.h"



ParameterSet::ParameterSet()
{
	srand(time(NULL));
	regenerateAll();
}


ParameterSet::~ParameterSet()
{
}

void ParameterSet::regenerateWType()
{
	if (!lockedParameters[0])
	{
		wType = rand() % 3;
	}
}

void ParameterSet::regenerateNBlades()
{
	if (!lockedParameters[1])
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
}

void ParameterSet::regenerateBLength()
{
	if (!lockedParameters[2])
	{
		bLength = rand() & BLADE_MAX_LENGTH + BLADE_MIN_LENGTH;
	}
}

void ParameterSet::regenerateBBreadth()
{
	if (!lockedParameters[3])
	{
		bBreadth = rand() & BLADE_MAX_BREADTH + BLADE_MIN_BREADTH;
	}
}

void ParameterSet::regenerateBWidth()
{
	if (!lockedParameters[4])
	{
		bWidth = rand() & BLADE_MAX_WIDTH + BLADE_MIN_WIDTH;
	}
}

void ParameterSet::regenerateBCurve()
{
	if (!lockedParameters[5])
	{
		int temp = rand() & BLADE_MAX_CURVE + BLADE_MIN_CURVE;
		float t2 = temp / 10000000000;
		bCurve = t2;
	}
}

void ParameterSet::regenerateHLength()
{
	if (!lockedParameters[6])
	{
		hLength = rand() & HILT_MAX_LENGTH + HILT_MIN_LENGTH;
	}
}

void ParameterSet::regenerateHRadius()
{
	if (!lockedParameters[7])
	{
		hRadius = rand() & HILT_MAX_RADIUS + HILT_MIN_RADIUS;
	}
}

void ParameterSet::regenerateHTRadius()
{
	if (!lockedParameters[8])
	{
		hTRadius = rand() & HILTTAPER_MAX_RADIUS + HILTTAPER_MIN_RADIUS;
	}
}

void ParameterSet::regeneratePRadius()
{
	if (!lockedParameters[9])
	{
		pRadius = rand() & POMMEL_MAX_RADIUS + POMMEL_MIN_RADIUS;
	}
}

void ParameterSet::regeneratePStyle()
{
	if (!lockedParameters[10])
	{
		pStyle = rand() % 6;
	}
}

void ParameterSet::regenerateMaterial()
{
	if (!lockedParameters[11])
	{
		material = rand() % 3;
	}
}

void ParameterSet::regenerateMColour()
{
	if (!lockedParameters[12])
	{
		mColour = rand() % 10;
	}
}

void ParameterSet::regenerateGColour()
{
	if (!lockedParameters[13])
	{
		gColour = rand() % 6;
	}
}

void ParameterSet::regenerateCRadius()
{
	if (!lockedParameters[14])
	{
		cRadius = rand() % CROSSGUARD_MAX_RADIUS + CROSSGUARD_MIN_RADIUS;
	}
}

void ParameterSet::regenerateCHeight()
{
	if (!lockedParameters[15])
	{
		cHeight = rand() % CROSSGUARD_MAX_HEIGHT + CROSSGUARD_MIN_HEIGHT;
	}
}

void ParameterSet::regenerateCShape()
{
	if (!lockedParameters[16])
	{
		cShape = rand() % 4;
	}
}

void ParameterSet::regenerateCBAngle()
{
	if (!lockedParameters[17])
	{
		int temp = rand() % CROSSGUARD_MAX_BEND + CROSSGUARD_MIN_BEND;
		float temp2 = temp / 10000000000;
		cBAngle = temp2;
	}
}

void ParameterSet::regenerateNEmboss()
{
	if (!lockedParameters[18])
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
}

void ParameterSet::regenerateCStyle()
{
	if (!lockedParameters[19])
	{
		cStyle = rand() % 4;
	}
}

void ParameterSet::regeneratePQuality()
{
	if (!lockedParameters[20])
	{
		pQuality = rand() % 4;
	}
}

void ParameterSet::regenerateWCondition()
{
	if (!lockedParameters[21])
	{
		wCondition = rand() % 5;
	}
}

void ParameterSet::regenerateHWStyle()
{
	if (!lockedParameters[22])
	{
		hWStyle = rand() % 3;
	}
}

void ParameterSet::regenerateHStyle()
{
	if (!lockedParameters[23])
	{
		hStyle = rand() % 4;
	}
}

void ParameterSet::regenerateHCLength()
{
	if (!lockedParameters[24])
	{
		hCLength = rand() % HILTCOLLAR_MAX_LENGTH + HILTCOLLAR_MIN_LENGTH;
	}
}

void ParameterSet::regenerateHCAngle()
{
	if (!lockedParameters[25])
	{
		int temp = rand() % HILTCOLLAR_MAX_ANGLE + HILTCOLLAR_MIN_ANGLE;
		float temp2 = temp / 10000000000;
		hCAngle = temp2;
	}
}

void ParameterSet::regenerateWStyle()
{
	if (!lockedParameters[26])
	{
		wStyle = rand() % 5;
	}
}

void ParameterSet::regenerateCLength()
{
	if (!lockedParameters[27])
	{
		cLength = rand() % COLLAR_MAX_LENGTH + COLLAR_MIN_LENGTH;
	}
}

void ParameterSet::regenerateCoStyle()
{
	if (!lockedParameters[28])
	{
		coStyle = rand() % 3;
	}
}

void ParameterSet::regenerateCAmount()
{
	if (!lockedParameters[29])
	{
		cAmount = rand() % CUTAWAY_MAX_AMOUNT + CUTAWAT_MIN_AMOUNT;
	}
}

void ParameterSet::regenerateFWidth()
{
	if (!lockedParameters[30])
	{
		fWidth = rand() % FULLER_MAX_WIDTH + FULLER_MIN_WIDTH;
	}
}

void ParameterSet::regenerateFLength()
{
	if (!lockedParameters[31])
	{
		fLength = rand() % FULLER_MAX_LENGTH + FULLER_MIN_LENGTH;
	}
}

void ParameterSet::regenerateFDepth()
{
	if (!lockedParameters[32])
	{
		fDepth = rand() & FULLER_MAX_DEPTH + FULLER_MIN_DEPTH;
	}
}

void ParameterSet::regenerateHamon()
{
	if (!lockedParameters[33])
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
}

void ParameterSet::regenerateHOffset()
{
	if (!lockedParameters[34])
	{
		hOffset = rand() % HAMON_MAX_OFFSET + HAMON_MIN_OFFSET;
	}
}

void ParameterSet::regeneratePLength()
{
	if (!lockedParameters[35])
	{
		pLength = rand() % POINT_MAX_LENGTH + POINT_MIN_LENGTH;
	}
}

void ParameterSet::regenerateEType()
{
	if (!lockedParameters[36])
	{
		eType = rand() % 8;
	}
}

void ParameterSet::regenerateBStyle()
{
	if (!lockedParameters[37])
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
}

void ParameterSet::regenerateHGAmount()
{
	if (!lockedParameters[38])
	{
		hGAmount = rand() % HOLLOWGRIND_MAX + HOLLOWGRIND_MIN;
	}
}

void ParameterSet::regenerateStWidth()
{
	if (!lockedParameters[39])
	{
		stWidth = rand() % SPEARTIP_MAX_WIDTH + SPEARTIP_MIN_WIDTH;
	}
}

void ParameterSet::regenerateSBHeight()
{
	if (!lockedParameters[40])
	{
		sBHeight = rand() % SPEARBASE_MAX_HEIGHT + SPEARBASE_MIN_HEIGHT;
	}
}

void ParameterSet::regenerateDamage()
{
		regenerateRLevel();
		regenerateCLevel();
}

void ParameterSet::regenerateRLevel()
{
	if (!lockedParameters[41])
	{
		rLevel = rand() % 10;
	}
}

void ParameterSet::regenerateCLevel()
{
	if (!lockedParameters[42])
	{
		cLevel = rand() % 10;
	}
}

void ParameterSet::regenerateAll()
{
	regenerateWType();
	regenerateNBlades();
	regenerateBLength();
	regenerateBBreadth();
	regenerateBWidth();
	regenerateBCurve();
	regenerateHLength();
	regenerateHRadius();
	regenerateHTRadius();
	regeneratePRadius();
	regeneratePStyle();
	regenerateMaterial();
	regenerateMColour();
	regenerateGColour();
	regenerateCRadius();
	regenerateCHeight();
	regenerateCShape();
	regenerateCBAngle();
	regenerateNEmboss();
	regenerateCStyle();
	regeneratePQuality();
	regenerateWCondition();
	regenerateHWStyle();
	regenerateHStyle();
	regenerateHCLength();
	regenerateHCAngle();
	regenerateWStyle();
	regenerateCLength();
	regenerateCoStyle();
	regenerateCAmount();
	regenerateFWidth();
	regenerateFLength();
	regenerateFDepth();
	regenerateHamon();
	regenerateHOffset();
	regeneratePLength();
	regenerateEType();
	regenerateBStyle();
	regenerateHGAmount();
	regenerateStWidth();
	regenerateSBHeight();
	regenerateDamage();
}

void ParameterSet::regenerateAmount(int a)
{
	//Regenerates a Random parameters
	do
	{
		int identifier = rand() % 43;

		switch (identifier)
		{
			case 0:
				regenerateWType();
				break;

			case 1:
				regenerateNBlades();
				break;

			case 2:
				regenerateBLength();
				break;

			case 3:
				regenerateBBreadth();
				break;

			case 4:
				regenerateBWidth();
				break;

			case 5:
				regenerateBCurve();
				break;

			case 6:
				regenerateHLength();
				break;

			case 7:
				regenerateHRadius();
				break;

			case 8:
				regenerateHTRadius();
				break;

			case 9:
				regeneratePRadius();
				break;

			case 10:
				regeneratePStyle();

			case 11:
				regenerateMaterial();
				break;

			case 12:
				regenerateMColour();
				break;

			case 13:
				regenerateGColour();
				break;

			case 14:
				regenerateCRadius();
				break;

			case 15:
				regenerateCHeight();
				break;

			case 16:
				regenerateCShape();
				break;

			case 17:
				regenerateCBAngle();
				break;

			case 18:
				regenerateNEmboss();
				break;

			case 19:
				regenerateCStyle();
				break;

			case 20:
				regeneratePQuality();
				break;

			case 21:
				regenerateWCondition();
				break;

			case 22:
				regenerateHWStyle();
				break;

			case 23:
				regenerateHStyle();
				break;

			case 24:
				regenerateHCLength();
				break;

			case 25:
				regenerateHCAngle();
				break;

			case 26:
				regenerateWStyle();
				break;

			case 27:
				regenerateCLength();
				break;

			case 28:
				regenerateCoStyle();
				break;

			case 29:
				regenerateCAmount();
				break;

			case 30:
				regenerateFWidth();
				break;

			case 31:
				regenerateFLength();
				break;

			case 32:
				regenerateFDepth();
				break;

			case 33:
				regenerateHamon();
				break;

			case 34:
				regenerateHOffset();
				break;

			case 35:
				regeneratePLength();
				break;

			case 36:
				regenerateEType();
				break;

			case 37:
				regenerateBStyle();
				break;

			case 38:
				regenerateHGAmount();
				break;

			case 39:
				regenerateStWidth();
				break;

			case 40:
				regenerateSBHeight();
				break;

			case 41:
				regenerateRLevel();
				break;

			case 42:
				regenerateCLevel();
				break;

			default:
				break;

		}

		a--;
	} while (a > 0);
}

bool ParameterSet::getLockState(int param)
{
	if (param < NUM_PARAMS)
	{
		return lockedParameters[param];
	}
}

void ParameterSet::setWType(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 2)
	{
		wType = arg;
		lockedParameters[0] = lock;
	}
}

void ParameterSet::setNBlades(bool arg, bool lock = true)
{
	nBlades = arg;
	lockedParameters[1] = lock;
}

void ParameterSet::setBLength(float arg, bool lock = true)
{
	bLength = arg;
	lockedParameters[2] = lock;
}

void ParameterSet::setBBreadth(float arg, bool lock = true)
{
	bBreadth = arg;
	lockedParameters[3] = lock;
}

void ParameterSet::setBWidth(float arg, bool lock = true)
{
	bWidth = arg;
	lockedParameters[4] = lock;
}

void ParameterSet::setBCurve(float arg, bool lock = true)
{
	bCurve = arg;
	lockedParameters[5] = lock;
}

void ParameterSet::setHLength(float arg, bool lock = true)
{
	hLength = arg;
	lockedParameters[6] = lock;
}

void ParameterSet::setHRadius(float arg, bool lock = true)
{
	hRadius = arg;
	lockedParameters[7] = lock;
}

void ParameterSet::setHTRadius(float arg, bool lock = true)
{
	hTRadius = arg;
	lockedParameters[8] = lock;
}

void ParameterSet::setPRadius(float arg, bool lock = true)
{
	pRadius = arg;
	lockedParameters[9] = lock;
}

void ParameterSet::setPStyle(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 5)
	{
		pStyle = arg;
		lockedParameters[10] = lock;
	}
}

void ParameterSet::setMaterial(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 2)
	{
		material = arg;
		lockedParameters[11] = lock;
	}
}

void ParameterSet::setMColour(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 9)
	{
		mColour = arg;
		lockedParameters[12] = lock;
	}
}

void ParameterSet::setGColour(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 5)
	{
		mColour = arg;
		lockedParameters[13] = lock;
	}
}

void ParameterSet::setCRadius(float arg, bool lock = true)
{
	cRadius = arg;
	lockedParameters[14] = lock;
}

void ParameterSet::setCHeight(float arg, bool lock = true)
{
	cHeight = arg;
	lockedParameters[15] = lock;
}

void ParameterSet::setCShape(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 3)
	{
		cShape = arg;
		lockedParameters[16] = lock;
	}
}

void ParameterSet::setCBAngle(float arg, bool lock = true)
{
	cBAngle = arg;
	lockedParameters[17] = lock;
}

void ParameterSet::setNEmboss(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 4)
	{
		if (arg == 0)
		{
			nEmboss = 3;
		}
		else
		{
			nEmboss = arg * 2;
		}
		lockedParameters[18] = lock;
	}
}

void ParameterSet::setCStyle(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 3)
	{
		cStyle = arg;
		lockedParameters[19] = lock;
	}
}

void ParameterSet::setPQuality(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 3)
	{
		pQuality = arg;
		lockedParameters[20] = lock;
	}
}

void ParameterSet::setWCondition(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 4)
	{
		wCondition = arg;
		lockedParameters[21] = lock;
	}
}

void ParameterSet::setHWStyle(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 2)
	{
		hWStyle = arg;
		lockedParameters[22] = lock;
	}
}

void ParameterSet::setHStyle(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 3)
	{
		hStyle = arg;
		lockedParameters[23] = lock;
	}
}

void ParameterSet::setHCLength(float arg, bool lock = true)
{
	hCLength = arg;
	lockedParameters[24] = lock;
}

void ParameterSet::setHCAngle(float arg, bool lock = true)
{
	hCAngle = arg;
	lockedParameters[25] = lock;
}

void ParameterSet::setWStyle(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 4)
	{
		wStyle = arg;
		lockedParameters[26] = lock;
	}
}

void ParameterSet::setCLength(float arg, bool lock = true)
{
	cLength = arg;
	lockedParameters[27];
}

void ParameterSet::setCoStyle(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 2)
	{
		coStyle = arg;
		lockedParameters[28] = lock;
	}
}

void ParameterSet::setCAmount(float arg, bool lock = true)
{
	cAmount = arg;
	lockedParameters[29] = lock;
}

void ParameterSet::setFWidth(float arg, bool lock = true)
{
	fWidth = arg;
	lockedParameters[30] = lock;
}

void ParameterSet::setFLength(float arg, bool lock = true)
{
	fLength = arg;
	lockedParameters[31] = lock;
}

void ParameterSet::setFDepth(float arg, bool lock = true)
{
	fDepth = arg;
	lockedParameters[32] = lock;
}

void ParameterSet::setHamon(bool arg, bool lock = true)
{
	hamon = arg;
	lockedParameters[33] = lock;
}

void ParameterSet::setHOffset(float arg, bool lock = true)
{
	hOffset = arg;
	lockedParameters[34] = lock;
}

void ParameterSet::setPLength(float arg, bool lock = true)
{
	pLength = arg;
	lockedParameters[35] = lock;
}

void ParameterSet::setEType(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 7)
	{
		eType = arg;
		lockedParameters[36] = lock;
	}
}

void ParameterSet::setBStyle(bool arg, bool lock = true)
{
	bStyle = arg;
	lockedParameters[37] = lock;
}

void ParameterSet::setHGAmount(float arg, bool lock = true)
{
	hGAmount = arg;
	lockedParameters[38] = lock;
}

void ParameterSet::setStWidth(float arg, bool lock = true)
{
	stWidth = arg;
	lockedParameters[39] = lock;
}

void ParameterSet::setSbHeight(float arg, bool lock = true)
{
	sBHeight = arg;
	lockedParameters[40] = lock;
}

void ParameterSet::setRLevel(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 10)
	{
		rLevel = arg;
		lockedParameters[41] = lock;
	}
}

void ParameterSet::setCLevel(int arg, bool lock = true)
{
	if (arg >= 0 && arg <= 7)
	{
		cLevel = arg;
		lockedParameters[42] = lock;
	}
}
