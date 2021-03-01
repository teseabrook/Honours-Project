#pragma once
#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"
class GemMesh :
	public BaseMesh
{
public:
	GemMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set);
	~GemMesh();

protected:
	void initBuffers(ID3D11Device* device);
	ParameterSet* set;
};

