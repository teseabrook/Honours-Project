#pragma once
#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"

class SpearBaseMesh :
	public BaseMesh
{
public:
	SpearBaseMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, int resolution = 20);;
	~SpearBaseMesh();

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
	ParameterSet* set;
};

