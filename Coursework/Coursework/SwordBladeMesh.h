#pragma once
#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"

using namespace DirectX;

class SwordBladeMesh :
	public BaseMesh
{


public:
	SwordBladeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, int resolution = 20);
	~SwordBladeMesh();


protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
	ParameterSet* set;
};

