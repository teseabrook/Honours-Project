#pragma once
#include "ParameterSet.h"
#include "BaseMesh.h"
#include "SphereMesh.h"
#include "DiskMesh.h"
#include "GemMesh.h"

class PommelMeshGenerator
{
public:
	PommelMeshGenerator(ParameterSet* a_set, ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext);
	~PommelMeshGenerator();

	BaseMesh* getMesh() { return pommelMesh; };
	GemMesh* getGemMesh() { return gemMesh; };

protected:
	BaseMesh* pommelMesh;
	GemMesh* gemMesh;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	ParameterSet* set;

	void generatePommel();
};

