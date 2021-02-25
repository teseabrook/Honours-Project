#pragma once
#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"

class HiltWrapMesh :
	public BaseMesh
{
public:
	HiltWrapMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, bool a_partial = false);
	~HiltWrapMesh();

protected:
	void initBuffers(ID3D11Device* device);
	void createCirclePoints(XMFLOAT3 centre, float radius, int offset = 0);
	void generateSpiralPoints(XMFLOAT3 centre, float radius, int offset = 0);
	void generateBackwardSpiralPoints(XMFLOAT3 centre, float radius, int offset = 0);
	void generateSpiral(int start);
	void generatePartialSpiral(int start);
	void generateBackwardPartialSpiral(int start);

	int vCounter, iCounter, noVPoints;
	ParameterSet* set;
	VertexType* vertices;
	unsigned long* indices;
	bool partial;
};

