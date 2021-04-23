#pragma once
#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"

/*
 * HighPolyDeformMesh
 * Creates a flat mesh with lots of verticies for deforming
 */

class HighPolyDeformMesh :
	public BaseMesh
{
public:
	HighPolyDeformMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, XMFLOAT3 startPos, XMFLOAT2 a_size, bool invert = false);
	~HighPolyDeformMesh();

protected:
	void initBuffers(ID3D11Device* device);
	ParameterSet* set;
	XMFLOAT3 start;
	XMFLOAT2 size;
	bool inverted;
};

