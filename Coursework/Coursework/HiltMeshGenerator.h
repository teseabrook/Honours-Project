#pragma once
#include "ParameterSet.h"
#include <BaseMesh.h>
#include <math.h>
#include <vector>
#include <string.h>

using namespace DirectX;

class HiltMeshGenerator : public BaseMesh
{
public:
	/*
	 * Takes the parameters from the given set and generates a hilt centered on (0, 0, 0)
	 */

	HiltMeshGenerator(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set);
	~HiltMeshGenerator();

	ID3D11ShaderResourceView* getTexture() { return textures[texture]; };
	void addTexture(ID3D11ShaderResourceView* s) { textures.push_back(s); };

protected:
	void initBuffers(ID3D11Device* device);
	ParameterSet* set;

	std::vector<ID3D11ShaderResourceView*> textures;
	int texture = 0;

	void generateCircle(XMFLOAT3 centre, float radius);
	void generateSides(int circle1Start);

	void generateSquare(XMFLOAT3 centre, float radius);
	void generateSquareSides(int start);
	void generateRecessSides(int start, float radius);

	VertexType* vertices;
	unsigned long* indices;

	int vCounter = 0;
	int iCounter = 0;
};

