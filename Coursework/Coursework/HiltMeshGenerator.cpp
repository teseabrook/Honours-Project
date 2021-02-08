#include "HiltMeshGenerator.h"

#define DEBUG_SCALE_FACTOR 10.0f

HiltMeshGenerator::HiltMeshGenerator(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set)
{
	set = a_set;
	initBuffers(device);
}


HiltMeshGenerator::~HiltMeshGenerator()
{
}

void HiltMeshGenerator::initBuffers(ID3D11Device* device)
{
	int resolution = 10;

	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// 360 points on the circle, 1 extra for the centre
	vertexCount = 362;

	//360 * 3 vertexes
	indexCount = 361 * 3;

	// Create the vertex and index array.
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	vertices[0].position = XMFLOAT3(0, 0, -1.0f);
	vertices[0].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(0, set->getHRadius() / DEBUG_SCALE_FACTOR, -2.0f);
	vertices[1].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

	float lastX = 0.0f;
	float lastY = set->getHRadius();

	int vCounter = 2;
	int iCounter = 0;

	int loops = 360;

	for (int i = 0; i < loops; i++)
	{
		//For any given triangle tN, it consists of the points (0, 0, 1/2HL) [Vertex 0], (xtN-1, ytN-1, 0) and (xtN, ytN, 0), forming a triangle of angles pi/180, 179pi/360 and 179pi/360 and sides hR, hR and sin(pi/180) * hR/sin(179pi/180)
		//when n = 0, xN and yN = 0
		//For any other n, the coordinates should be (hR * sin(theta), hR * cos(theta * i))
		float theta = (std::_Pi / 180) * (i + 1);

		float newX = (set->getHRadius() / DEBUG_SCALE_FACTOR) * sin(theta);
		float newY = (set->getHRadius() / DEBUG_SCALE_FACTOR) * cos(theta);
		vertices[vCounter].position = XMFLOAT3(newX, newY, -2.0f);
		vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

		if (i == (loops - 1))
		{
			indices[iCounter] = 0;
			iCounter++;
			indices[iCounter] = 1;
			iCounter++;
			indices[iCounter] = vCounter - 1;
			iCounter++;
		}

		else
		{
			indices[iCounter] = 0;
			iCounter++;
			indices[iCounter] = vCounter;
			iCounter++;
			indices[iCounter] = vCounter - 1;
			iCounter++;
		}

		vCounter++;

		lastX = newX;
		lastY = newY;

	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	//delete[] vertices;
	//vertices = 0;

	//delete[] indices;
	//indices = 0;
}
