#include "GemMesh.h"
#define DEBUG_SCALE_FACTOR 25.0f;


GemMesh::GemMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set)
{
	set = a_set;
	initBuffers(device);
}


GemMesh::~GemMesh()
{
}

void GemMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 6;
	indexCount = 24;

	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	float radius = set->getPRadius() / DEBUG_SCALE_FACTOR;

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);  // Centre.
	vertices[0].texture = XMFLOAT2(0.5f, 0.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[1].position = XMFLOAT3(-radius, radius, -radius);  // top left.
	vertices[1].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[2].position = XMFLOAT3(radius, radius, -radius);  // top right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[3].position = XMFLOAT3(-radius, -radius, -radius);  // bottom left.
	vertices[3].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[4].position = XMFLOAT3(radius, -radius, -radius);  // bottom right.
	vertices[4].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[4].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[5].position = XMFLOAT3(0.0f, 0.0f, -radius * 2);  // Centre.
	vertices[5].texture = XMFLOAT2(0.5f, 0.0f);
	vertices[5].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Centre/
	indices[1] = 1;  // Top left.
	indices[2] = 2;  // Top right.

	indices[3] = 0;  // Centre/
	indices[4] = 3;  // Top left.
	indices[5] = 1;  // Bottom Left

	indices[6] = 0;  // Centre/
	indices[7] = 2;  // Top left.
	indices[8] = 4;  // Bottom Left

	indices[9] = 0;  // Centre/
	indices[10] = 4;  // Top left.
	indices[11] = 3;  // Bottom Left

	indices[12] = 5;  // Centre/
	indices[13] = 2;  // Top left.
	indices[14] = 1;  // Top right.

	indices[15] = 5;  // Centre/
	indices[16] = 1;  // Top left.
	indices[17] = 3;  // Bottom Left

	indices[18] = 5;  // Centre/
	indices[19] = 4;  // Top left.
	indices[20] = 2;  // Bottom Left

	indices[9] = 5;  // Centre/
	indices[10] = 3;  // Top left.
	indices[11] = 4;  // Bottom Left

	vertexBufferDesc = { sizeof(VertexType) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	vertexData = { vertices, 0 , 0 };

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

	indexBufferDesc = { sizeof(unsigned long) * indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	indexData = { indices, 0, 0 };
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	//// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}