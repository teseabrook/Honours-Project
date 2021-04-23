#pragma once

/*
 * Crossguard Mesh Generator
 * Contains a BaseMesh and sets it to the correct mesh from the given ParameterSet
 */

#include "ParameterSet.h"
#include "BaseMesh.h"
#include "SphereMesh.h"
#include "DiskMesh.h"
#include "GemMesh.h"
#include "ClawMesh.h"
#include "BaseMesh.h"
#include "BarMesh.h"
#include "EmbossedDiskMesh.h"

class CrossguardMeshGenerator
{
public:
	CrossguardMeshGenerator(ParameterSet* a_set, ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext);
	~CrossguardMeshGenerator();

	BaseMesh* getMesh() { return pommelMesh; };

protected:
	BaseMesh* pommelMesh;
	GemMesh* gemMesh;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ParameterSet* set;

	void generateCrossguard();
};

