#pragma once
#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"

class HiltWrapMesh :
	public BaseMesh
{
public:
	HiltWrapMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set);
	~HiltWrapMesh();

protected:
	void initBuffers(ID3D11Device* device);
	void createCirclePoints(XMFLOAT3 centre, float radius);
	void generateSpiralPoints(XMFLOAT3 centre, float radius);
	void generateSpiral(int start);

	int vCounter, iCounter;
	ParameterSet* set;
	VertexType* vertices;
	unsigned long* indices;
};

