#include "EmbossedDiskMesh.h"

#define DEBUG_SCALE_FACTOR 25.0f



EmbossedDiskMesh::EmbossedDiskMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, bool isPommel)
{
	pommel = isPommel;
	set = a_set;
	initBuffers(device);
}


EmbossedDiskMesh::~EmbossedDiskMesh()
{
	BaseMesh::~BaseMesh();
}

void EmbossedDiskMesh::initBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// 360 points on the circle, 1 extra for the centre
	vertexCount = 363 * 3 + 24;

	//360 * 3 vertexes, plus 360 * 6 for sides
	indexCount = 361 * 3 * 3 + 360 * 6 + 36;

	// Create the vertex and index array.
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];


	float radius;

	if (pommel)
	{
		radius = (set->getPRadius());
	}
	else
	{
		radius = set->getCRadius();
	}

	generateCircle(XMFLOAT3(0, 0, 0), radius);

	if (pommel)
	{
		generateCircle(XMFLOAT3(0, 0, (radius / 6)), radius, true, true);
	}
	else
	{
		generateCircle(XMFLOAT3(0, 0, set->getCHeight() / DEBUG_SCALE_FACTOR), radius, true, true);
	}

	generateSides(1);

	generateCircle(XMFLOAT3(0, 0, set->getCHeight() / DEBUG_SCALE_FACTOR), set->getPRadius(), true);
	//generateFlatBits(set->getNEmboss());



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
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void EmbossedDiskMesh::generateCircle(XMFLOAT3 centre, float radius, bool invert, bool invis)
{
	int vCounterStart = vCounter;

	vertices[vCounter].position = XMFLOAT3(centre.x, centre.y, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

	vCounter++;

	vertices[vCounter].position = XMFLOAT3(centre.x, radius / DEBUG_SCALE_FACTOR + centre.y, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

	vCounter++;

	float lastX = 0.0f;
	float lastY = radius;

	int loops = 360;

	for (int i = 0; i < loops; i++)
	{
		//For any given triangle tN, it consists of the points (0, 0, 1/2HL) [Vertex 0], (xtN-1, ytN-1, 0) and (xtN, ytN, 0), forming a triangle of angles pi/180, 179pi/360 and 179pi/360 and sides hR, hR and sin(pi/180) * hR/sin(179pi/180)
		//when n = 0, xN and yN = 0
		//For any other n, the coordinates should be (hR * sin(theta), hR * cos(theta * i))
		float theta = (std::_Pi / 180) * (i + 1);

		float newX = (radius / DEBUG_SCALE_FACTOR) * sin(theta) + centre.x;
		float newY = (radius / DEBUG_SCALE_FACTOR) * cos(theta) + centre.y;
		vertices[vCounter].position = XMFLOAT3(newX, newY, centre.z);
		vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

		if (!invis)
		{
			if (i == (loops - 1))
			{
				if (invert)
				{
					indices[iCounter] = vCounterStart;
					iCounter++;
					indices[iCounter] = vCounterStart - 1;
					iCounter++;
					indices[iCounter] = vCounter = 1;
					iCounter++;
				}
				else
				{
					indices[iCounter] = vCounterStart;
					iCounter++;
					indices[iCounter] = vCounterStart + 1;
					iCounter++;
					indices[iCounter] = vCounter - 1;
					iCounter++;
				}
			}

			else
			{
				if (invert)
				{
					indices[iCounter] = vCounterStart;
					iCounter++;
					indices[iCounter] = vCounter - 1;
					iCounter++;
					indices[iCounter] = vCounter;
					iCounter++;
				}
				else
				{
					indices[iCounter] = vCounterStart;
					iCounter++;
					indices[iCounter] = vCounter;
					iCounter++;
					indices[iCounter] = vCounter - 1;
					iCounter++;
				}
			}
		}

		vCounter++;

		lastX = newX;
		lastY = newY;

	}
}


void EmbossedDiskMesh::generateSides(int circle1Start)
{
	for (int i = 0; i < 360; i++)
	{
		indices[iCounter] = i + circle1Start;
		iCounter++;
		indices[iCounter] = i + circle1Start + 1;
		iCounter++;
		indices[iCounter] = i + circle1Start + 362;
		iCounter++;

		indices[iCounter] = i + circle1Start + 1;
		iCounter++;
		indices[iCounter] = i + circle1Start + 363;
		iCounter++;
		indices[iCounter] = i + circle1Start + 362;
		iCounter++;
	}
}

void EmbossedDiskMesh::generateFlatBits(int number)
{
	int vCounterStart = vCounter;

	XMFLOAT3 centre = XMFLOAT3(0, 0, set->getCHeight());

	//First flat bit
	float theta = (std::_Pi / 180) * (355);

	float newX = (set->getPRadius() / DEBUG_SCALE_FACTOR) * sin(theta) + centre.x;
	float newY = (set->getPRadius() / DEBUG_SCALE_FACTOR) * cos(theta) + centre.y;

	vertices[vCounter].position = XMFLOAT3(newX, newY, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;


	theta = (std::_Pi / 180) * (5);

	newX = (set->getPRadius() / DEBUG_SCALE_FACTOR) * sin(theta) + centre.x;
	newY = (set->getPRadius() / DEBUG_SCALE_FACTOR) * cos(theta) + centre.y;

	vertices[vCounter].position = XMFLOAT3(newX, newY, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;


	theta = (std::_Pi / 180) * (355);

	newX = (set->getCRadius() / DEBUG_SCALE_FACTOR) * sin(theta) + centre.x;
	newY = (set->getCRadius() / DEBUG_SCALE_FACTOR) * cos(theta) + centre.y;

	vertices[vCounter].position = XMFLOAT3(newX, newY, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;


	theta = (std::_Pi / 180) * (5);

	newX = (set->getCRadius() / DEBUG_SCALE_FACTOR) * sin(theta) + centre.x;
	newY = (set->getCRadius() / DEBUG_SCALE_FACTOR) * cos(theta) + centre.y;

	vertices[vCounter].position = XMFLOAT3(newX, newY, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	//indices[iCounter] = vCounterStart;
	//iCounter++;
	//indices[iCounter] = vCounterStart + 1;
	//iCounter++;
	//indices[iCounter] = vCounterStart + 2;
	//iCounter++;
}