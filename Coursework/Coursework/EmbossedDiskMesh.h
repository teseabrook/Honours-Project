#pragma once
#include "e:\Coursework 2020-2021\Honours-Project\Coursework\DXFramework\BaseMesh.h"
#include "ParameterSet.h"

/*
 * Embossed Disk
 * Not used currently, but essentially the fancier crossguards
 */

class EmbossedDiskMesh :
	public BaseMesh
{
public:
	EmbossedDiskMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, bool isPommel = true);
	~EmbossedDiskMesh();

protected:
	void initBuffers(ID3D11Device* device);
	void generateCircle(XMFLOAT3 centre, float radius, bool invert = false, bool invis = false);
	void generateSides(int circle1Start);
	void generateFlatBits(int number);
	int resolution = 5;
	ParameterSet* set;
	bool pommel = true;

	ID3D11DeviceContext* dContext;

	VertexType* vertices;
	unsigned long* indices;

	int vCounter = 0;
	int iCounter = 0;
};