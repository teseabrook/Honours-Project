#pragma once
#include "ParameterSet.h"
#include <BaseMesh.h>

using namespace DirectX;

class HiltMeshGenerator : public BaseMesh
{
public:
	/*
	 * Takes the parameters from the given set and generates a hilt centered on (0, 0, 0)
	 */

	HiltMeshGenerator(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* set);
	~HiltMeshGenerator();

protected:
	void initBuffers(ID3D11Device* device);
};

