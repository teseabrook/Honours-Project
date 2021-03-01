#pragma once
#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"

class DiskMesh :
	public BaseMesh
{
public:
	DiskMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set);
	~DiskMesh();

protected:
	void initBuffers(ID3D11Device* device);
	void generateCircle(XMFLOAT3 centre, float radius, bool invert = false);
	void generateSides(int circle1Start);
	int resolution = 5;
	ParameterSet* set;

	ID3D11DeviceContext* dContext;

	VertexType* vertices;
	unsigned long* indices;

	int vCounter = 0;
	int iCounter = 0;
};

