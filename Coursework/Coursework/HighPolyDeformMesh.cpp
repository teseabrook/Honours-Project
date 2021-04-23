#include "HighPolyDeformMesh.h"



HighPolyDeformMesh::HighPolyDeformMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, XMFLOAT3 startPos, XMFLOAT2 a_size, bool invert)
{
	inverted = invert;

	set = a_set;
	start = startPos;
	size = a_size;
	initBuffers(device);
}


HighPolyDeformMesh::~HighPolyDeformMesh()
{
}

void HighPolyDeformMesh::initBuffers(ID3D11Device* device)
{
	int resolution = 100;

	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float positionX, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = resolution * resolution * 4;


	indexCount = resolution * resolution * 6;
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	int vCounter, iCounter;
	vCounter = 0;
	iCounter = 0;

	for (int i = 0; i < resolution; i++)
	{
		for (int j = 0; j < resolution; j++)
		{
			vertices[vCounter].position = start;
			vertices[vCounter].position.y -= (size.x / resolution) * (i);
			vertices[vCounter].position.z += (size.y / resolution) * (j);

			if (inverted)
			{
				vertices[vCounter].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			}
			else
			{
				vertices[vCounter].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			}
			vertices[vCounter].texture = XMFLOAT2(i / resolution, j / resolution);
			vCounter++;

			vertices[vCounter].position = start;
			vertices[vCounter].position.y -= (size.x / resolution) * (i + 1);
			vertices[vCounter].position.z += (size.y / resolution) * (j);
			if (inverted)
			{
				vertices[vCounter].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			}
			else
			{
				vertices[vCounter].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			}
			vertices[vCounter].texture = XMFLOAT2(i + 1 / resolution, j / resolution);
			vCounter++;

			vertices[vCounter].position = start;
			vertices[vCounter].position.y -= (size.x / resolution) * (i);
			vertices[vCounter].position.z += (size.y / resolution) * (j + 1);
			if (inverted)
			{
				vertices[vCounter].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			}
			else
			{
				vertices[vCounter].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			}
			vertices[vCounter].texture = XMFLOAT2(i / resolution, j + 1/ resolution);
			vCounter++;

			vertices[vCounter].position = start;
			vertices[vCounter].position.y -= (size.x / resolution) * (i + 1);
			vertices[vCounter].position.z += (size.y / resolution) * (j + 1);
			if (inverted)
			{
				vertices[vCounter].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			}
			else
			{
				vertices[vCounter].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			}
			vertices[vCounter].texture = XMFLOAT2(i + 1 / resolution, j + 1 / resolution);
			vCounter++;

			if (inverted)
			{
				indices[iCounter] = vCounter - 4;
				iCounter++;
				indices[iCounter] = vCounter - 2;
				iCounter++;
				indices[iCounter] = vCounter - 3;
				iCounter++;

				indices[iCounter] = vCounter - 3;
				iCounter++;
				indices[iCounter] = vCounter - 2;
				iCounter++;
				indices[iCounter] = vCounter - 1;
				iCounter++;
			}
			else
			{
				indices[iCounter] = vCounter - 4;
				iCounter++;
				indices[iCounter] = vCounter - 3;
				iCounter++;
				indices[iCounter] = vCounter - 2;
				iCounter++;

				indices[iCounter] = vCounter - 3;
				iCounter++;
				indices[iCounter] = vCounter - 1;
				iCounter++;
				indices[iCounter] = vCounter - 2;
				iCounter++;
			}

		}
	}

	//Test code
	/*vertices[0].position = start;
	vertices[0].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = start;
	vertices[1].position.y -= size.x;
	vertices[1].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[1].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = start;
	vertices[2].position.z += size.y;
	vertices[2].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[3].position = start;
	vertices[3].position.z += size.y;
	vertices[3].position.y -= size.x;
	vertices[3].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);*/

	/*indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;*/


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

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}