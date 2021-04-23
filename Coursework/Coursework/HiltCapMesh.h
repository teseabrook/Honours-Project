#pragma once
#include "BaseMesh.h"
#include "ParameterSet.h"

/*
 * HiltCapMesh
 * Creates a mesh for the hilt caps
 */

class HiltCapMesh : public BaseMesh
{
public:
	HiltCapMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, bool top = false);
	~HiltCapMesh();

	void initBuffers(ID3D11Device* device);

protected:
	void generateCircle(XMFLOAT3 centre, float radius, bool invert = false);
	void generateInvisCircle(XMFLOAT3 centre, float radius, bool invert = false);
	void generateSides(int circle1Start);

	int vCounter, iCounter, noVPoints;
	ParameterSet* set;
	VertexType* vertices;
	unsigned long* indices;
	bool top;
};

