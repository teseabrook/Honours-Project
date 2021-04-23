#pragma once

/*
 * Claw Mesh Generator
 * Makes the funny claw thing for some of the hilts
 */

#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"

class ClawMesh :
	public BaseMesh
{
public:
	ClawMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set);
	~ClawMesh();

protected:
	ParameterSet* set;
	void initBuffers(ID3D11Device* device);

	void generateCircle(XMFLOAT3 centre, float radius, bool invert = false);
	void generateInvisCircle(XMFLOAT3 centre, float radius);
	void generateSides(int circle1Start);

	VertexType* vertices;
	unsigned long* indices;
	int vCounter = 0;
	int iCounter = 0;
};

