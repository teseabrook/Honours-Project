#pragma once
#include "BaseMesh.h"
#include "ParameterSet.h"
#include "DiskMesh.h"
#include "SpearBaseMesh.h"

class SpeartipMeshGenerator
{
public:
	SpeartipMeshGenerator(ID3D11Device* device, ID3D11DeviceContext* dContext, ParameterSet* set);
	~SpeartipMeshGenerator();

	BaseMesh* getCollarMesh() { return collarMesh; };
	BaseMesh* getSpeartipMesh() { return speartipMesh; };
	BaseMesh* getSpearBaseMesh() { return spearBaseMesh; };

protected:
	BaseMesh* collarMesh;
	BaseMesh* speartipMesh;
	BaseMesh* spearBaseMesh;

	void generateMeshes(ID3D11Device* device, ID3D11DeviceContext* dContext, ParameterSet* set);
};

