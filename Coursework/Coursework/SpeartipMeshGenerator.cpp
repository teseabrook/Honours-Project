#include "SpeartipMeshGenerator.h"



SpeartipMeshGenerator::SpeartipMeshGenerator(ID3D11Device* device, ID3D11DeviceContext* dContext, ParameterSet* set)
{
	generateMeshes(device, dContext, set);
}


SpeartipMeshGenerator::~SpeartipMeshGenerator()
{
}

void SpeartipMeshGenerator::generateMeshes(ID3D11Device* device, ID3D11DeviceContext* dContext, ParameterSet* set)
{
	collarMesh = new DiskMesh(device, dContext, set, 2);
	spearBaseMesh = new SpearBaseMesh(device, dContext, set);
}
