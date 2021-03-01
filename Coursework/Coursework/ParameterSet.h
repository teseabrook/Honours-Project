//Note: As c++ cannot nativley produce random floats, the min and max values are the min and max values multiplied by 10 billion (Moved 10DP up)

#define BLADE_MAX_LENGTH 1000
#define BLADE_MIN_LENGTH 0
#define BLADE_MAX_BREADTH 750
#define BLADE_MIN_BREADTH 0
#define BLADE_MAX_WIDTH 250
#define BLADE_MIN_WIDTH 0
#define BLADE_MAX_CURVE 31415926535
#define BLADE_MIN_CURVE 15707963267
#define HILT_MAX_LENGTH 2000
#define HILT_MIN_LENGTH 0
#define HILT_MAX_RADIUS 250
#define HILT_MIN_RADIUS 0
#define HILTTAPER_MAX_RADIUS 250
#define HILTTAPER_MIN_RADIUS 0
#define POMMEL_MAX_RADIUS 100
#define POMMEL_MIN_RADIUS 0
#define CROSSGUARD_MAX_RADIUS 500
#define CROSSGUARD_MIN_RADIUS 0
#define CROSSGUARD_MAX_HEIGHT 500
#define CROSSGUARD_MIN_HEIGHT 0
#define CROSSGUARD_MAX_BEND 7853981633
#define CROSSGUARD_MIN_BEND 0
#define HILTCOLLAR_MAX_LENGTH 100
#define HILTCOLLAR_MIN_LENGTH 0
#define HILTCOLLAR_MAX_ANGLE 7853981633
#define HILTCOLLAR_MIN_ANGLE 0
#define COLLAR_MAX_LENGTH 250
#define COLLAR_MIN_LENGTH 0
#define CUTAWAY_MAX_AMOUNT 75
#define CUTAWAT_MIN_AMOUNT 0
#define FULLER_MAX_WIDTH 750
#define FULLER_MIN_WIDTH 0
#define FULLER_MAX_LENGTH 1000
#define FULLER_MIN_LENGTH 0
#define FULLER_MAX_DEPTH 250
#define FULLER_MIN_DEPTH 0
#define HAMON_MAX_OFFSET 375
#define HAMON_MIN_OFFSET 0
#define POINT_MAX_LENGTH 1000
#define POINT_MIN_LENGTH 0
#define HOLLOWGRIND_MAX 100
#define HOLLOWGRIND_MIN 0
#define SPEARTIP_MAX_WIDTH 500
#define SPEARTIP_MIN_WIDTH 0
#define SPEARBASE_MAX_HEIGHT 500
#define SPEARBASE_MIN_HEIGHT 0
#define NUM_PARAMS 43

#pragma once
#include <random>
#include <ctime>

class ParameterSet
{
public:
	ParameterSet();
	~ParameterSet();

	void lockParameter(int param) { lockedParameters[param] = true; };
	void unlockParameter(int param) { lockedParameters[param] = false; };

	//Getters
	int getWType() { return wType; };
	bool getNBlades() { return nBlades; };
	float getBLength() { return bLength; };
	float getBBreadth() { return bBreadth; };
	float getBWidth() { return bWidth; };
	float getBCurve() { return bCurve; };
	float getHLength() { return hLength; };
	float getHRadius() { return hRadius; };
	float getHTRadius() { return hTRadius; };
	float getPRadius() { return pRadius; };
	int getPStyle() { return pStyle; };
	int getMaterial() { return material; };
	int getMColour() { return mColour; };
	int getGColour() { return gColour; };

	float getCRadius() { return cRadius; };
	float getCHeight() { return cHeight; };
	int getCShape() { return cShape; };
	float getCBAngle() { return cBAngle; };
	int getNEmboss() { return nEmboss; };
	int getCStyle() { return cShape; };
	int getPQuality() { return pQuality; };
	int getWCondition() { return wCondition; };
	int getHWStyle() { return hWStyle; };
	int getHStyle() { return hStyle; };
	float getHCLength() { return hCLength; };
	float getHCAngle() { return hCAngle; };
	int getWStyle() { return wStyle; };
	float getCLength() { return cLength; };
	int getCoStyle() { return coStyle; };
	float getCAmount() { return cAmount; };
	float getFWidth() { return fWidth; };
	float getFLength() { return fLength; };
	float getFDepth() { return fDepth; };
	bool getHamon() { return hamon; };
	float getHOffset() { return hOffset; };
	float getPLength() { return pLength; };
	int getEType() { return eType; };
	bool getBStyle() { return bStyle; };
	float getHGAmount() { return hGAmount; };
	float getStWidth() { return stWidth; };
	float getSBHeigh() { return sBHeight; };
	int getRLevel() { return rLevel; };
	int getCLevel() { return cLevel; };

	bool getLockState(int param);

	//Setters
	void setWType(int arg, bool lock = true);
	void setNBlades(bool arg, bool lock = true);
	void setBLength(float arg, bool lock = true);
	void setBBreadth(float arg, bool lock = true);
	void setBWidth(float arg, bool lock = true);
	void setBCurve(float arg, bool lock = true);
	void setHLength(float arg, bool lock = true);
	void setHRadius(float arg, bool lock = true);
	void setHTRadius(float arg, bool lock = true);
	void setPRadius(float arg, bool lock = true);
	void setPStyle(int arg, bool lock = true);
	void setMaterial(int arg, bool lock = true);
	void setMColour(int arg, bool lock = true);
	void setGColour(int arg, bool lock = true);
	void setCRadius(float arg, bool lock = true);
	void setCHeight(float arg, bool lock = true);
	void setCShape(int arg, bool lock = true);
	void setCBAngle(float arg, bool lock = true);
	void setNEmboss(int arg, bool lock = true);
	void setCStyle(int arg, bool lock = true);
	void setPQuality(int arg, bool lock = true);
	void setWCondition(int arg, bool lock = true);
	void setHWStyle(int arg, bool lock = true);
	void setHStyle(int arg, bool lock = true);
	void setHCLength(float arg, bool lock = true);
	void setHCAngle(float arg, bool lock = true);
	void setWStyle(int arg, bool lock = true);
	void setCLength(float arg, bool lock = true);
	void setCoStyle(int arg, bool lock = true);
	void setCAmount(float arg, bool lock = true);
	void setFWidth(float arg, bool lock = true);
	void setFLength(float arg, bool lock = true);
	void setFDepth(float arg, bool lock = true);
	void setHamon(bool arg, bool lock = true);
	void setHOffset(float arg, bool lock = true);
	void setPLength(float arg, bool lock = true);
	void setEType(int arg, bool lock = true);
	void setBStyle(bool arg, bool lock = true);
	void setHGAmount(float arg, bool lock = true);
	void setStWidth(float arg, bool lock = true);
	void setSbHeight(float arg, bool lock = true);
	void setRLevel(int arg, bool lock = true);
	void setCLevel(int arg, bool lock = true);

	void regenerateAll();
	void regenerateAmount(int amount);

protected:
	//The list of parameters and how they're stored internally
	
	//Weapon Type
	//0: Sword, 1: Axe, 2: Spear
	int wType;
	void regenerateWType();

	//Number of Blades
	//True = 1, False = 2
	bool nBlades;
	void regenerateNBlades();

	//Blade Length in mm
	float bLength;
	void regenerateBLength();

	//Blade Breadth in mm
	float bBreadth;
	void regenerateBBreadth();

	//Blade Width in mm
	float bWidth;
	void regenerateBWidth();

	//Blade Curve in radians
	float bCurve;
	void regenerateBCurve();

	//Hilt Length in mm
	float hLength;
	void regenerateHLength();

	//Hilt Radius in mm
	float hRadius;
	void regenerateHRadius();

	//Hilt Taper Radius in mm
	float hTRadius;
	void regenerateHTRadius();

	//Pommel Radius in mm
	float pRadius;
	void regeneratePRadius();

	//Pommel Style
	//0: None, 1: Ball, 2: Disk, 3: Claw, 4: Claw + Gem, 5: Disk + Gem;
	int pStyle;
	void regeneratePStyle();

	//Primary Material
	//0: Wood, 1: Metal, 2: Stone
	int material;
	void regenerateMaterial();

	//Metal Colour
	//0: Steel, 1: Silver, 2: Dark Steel, 3: Red, 4: Blue, 5: Green, 6: Brass, 7: Purple, 8: Yellow, 9: Light Blue
	int mColour;
	void regenerateMColour();

	//Gem Colour
	//0: Diamond, 1: Ruby, 2: Sapphire, 3: Emerald, 4: Quartz, 5: Amethyst
	int gColour;
	void regenerateGColour();

	//Crossguard Radius in mm
	float cRadius;
	void regenerateCRadius();

	//Crossguard Height in mm
	float cHeight;
	void regenerateCHeight();

	//Crossguard Shape
	//0: Bar, 1: Bow-Tie, 2: Disk, 3: None
	int cShape;
	void regenerateCShape();

	//Crossguard Bend Angle in radians
	float cBAngle;
	void regenerateCBAngle();

	//Number of embosses
	int nEmboss;
	void regenerateNEmboss();

	//Crossguard Style
	//0: Solid, 1: Embossed, 2: Single Gem, 3: Embossed with Gem
	int cStyle;
	void regenerateCStyle();

	//Production Quality
	//0: Common, 1: High, 2: Exquisite, 3: Masterpiece
	int pQuality;
	void regeneratePQuality();

	//Weapon Condition
	//0: New, 1: Cared-For, 2: Used, 3: Battered, 4: Poor
	int wCondition;
	void regenerateWCondition();

	//Hilt Wrapping
	//0: None, 1: Partial, 2: Full
	int hWStyle;
	void regenerateHWStyle();

	//Hilt Style
	//0: Ribbed, 1: Enscribed, 2: Rubber, 3: None
	int hStyle;
	void regenerateHStyle();

	//Hilt Collar Length in mm
	float hCLength;
	void regenerateHCLength();

	//Hilt Collar Angle in radians
	float hCAngle;
	void regenerateHCAngle();

	//Wrapping Style
	//0: Brown, 1: Tan, 2: Black, 3: Metal, 4: Effect
	int wStyle;
	void regenerateWStyle();

	//Collar Length in mm
	float cLength;
	void regenerateCLength();

	//Collar Style
	//0: Cutaway, 1: Cap, 2: None
	int coStyle;
	void regenerateCoStyle();

	//Cutaway Amount in mm
	float cAmount;
	void regenerateCAmount();

	//Fuller Width in mm
	float fWidth;
	void regenerateFWidth();

	//Fuller Length in mm
	float fLength;
	void regenerateFLength();

	//Fuller Depth in mm
	float fDepth;
	void regenerateFDepth();

	//Has Hamon?
	bool hamon;
	void regenerateHamon();

	//Hamon offset in mm
	float hOffset;
	void regenerateHOffset();

	//Point Length
	float pLength;
	void regeneratePLength();

	//Effect Type
	//0: Magic, 1: Fire, 2: Ice, 3: Wind, 4: Toxic, 5: Electric, 6: Holy, 7: None
	int eType;
	void regenerateEType();

	//Blade Style
	//True: Diamond, False: Hollow Grind
	bool bStyle;
	void regenerateBStyle();

	//Hollow Grind Amount in mm
	float hGAmount;
	void regenerateHGAmount();

	//Speartip Width in mm
	float stWidth;
	void regenerateStWidth();

	//Spear Base Height in mm
	float sBHeight;
	void regenerateSBHeight();

	//Damage Levels
	int rLevel, cLevel;
	void regenerateDamage();
	void regenerateRLevel();
	void regenerateCLevel();

	bool lockedParameters[NUM_PARAMS] = { false };

};

