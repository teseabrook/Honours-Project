#include "HiltCapMesh.h"


#define DEBUG_SCALE_FACTOR 25.0f

HiltCapMesh::HiltCapMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, bool a_top)
{
	a_top = top;
	set = a_set;
	initBuffers(device);
}


HiltCapMesh::~HiltCapMesh()
{
}

void HiltCapMesh::initBuffers(ID3D11Device* device)
{
	int resolution = 10;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// 360 points on the circle, 1 extra for the centre
	vertexCount = 362 * 2;

	//360 * 3 vertexes, plus 360 * 6 for sides
	indexCount = 361 * 3 * 2 + 360 * 6;

	// Create the vertex and index array.
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	if (!top)
	{
		generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius() * 1.01);

		if (set->getWType() != 0)
		{
			generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHCLength() / DEBUG_SCALE_FACTOR), set->getHRadius() * 1.01, true);
		}
		else
		{
			generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHCLength() / DEBUG_SCALE_FACTOR), set->getHTRadius() * 1.01, true);
		}


		generateSides(1);
	}
	else
	{
		generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR)), set->getHRadius() * 1.01, true);

		if (set->getWType() != 0)
		{
			generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR) - (set->getHCLength() / DEBUG_SCALE_FACTOR)), set->getHRadius() * 1.01, false);
		}
		else
		{
			generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR) - (set->getHCLength() / DEBUG_SCALE_FACTOR)), set->getHTRadius() * 1.01, true);
		}


		generateSides(1);
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

}

void HiltCapMesh::generateCircle(XMFLOAT3 centre, float radius, bool invert)
{
	int vCounterStart = vCounter;

	vertices[vCounter].position = XMFLOAT3(centre.x, centre.y, centre.z);

	if (invert)
	{

		vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	}
	else
	{
		vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	}

	vertices[vCounter].texture = XMFLOAT2(0.5f, 0.5f);

	vCounter++;

	vertices[vCounter].position = XMFLOAT3(centre.x, radius / DEBUG_SCALE_FACTOR + centre.y, centre.z);
	if (invert)
	{

		vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	}
	else
	{
		vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	}
	vertices[vCounter].texture = XMFLOAT2(0.0f, 1.0f);

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
		vertices[vCounter].normal = XMFLOAT3(sin(theta), cos(theta), 0.0f);
		vertices[vCounter].texture = XMFLOAT2(sin(theta), cos(theta));

		if (i == (loops - 1))
		{
			if (invert)
			{
				indices[iCounter] = vCounterStart;
				iCounter++;
				indices[iCounter] = vCounter - 1;
				iCounter++;
				indices[iCounter] = vCounterStart + 1;
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

		vCounter++;

		lastX = newX;
		lastY = newY;

	}
}

void HiltCapMesh::generateInvisCircle(XMFLOAT3 centre, float radius, bool invert)
{
	int vCounterStart = vCounter;

	vertices[vCounter].position = XMFLOAT3(centre.x, centre.y, centre.z);

	if (invert)
	{

		vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	}
	else
	{
		vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	}

	vertices[vCounter].texture = XMFLOAT2(0.5f, 0.5f);

	vCounter++;

	vertices[vCounter].position = XMFLOAT3(centre.x, radius / DEBUG_SCALE_FACTOR + centre.y, centre.z);
	if (invert)
	{

		vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	}
	else
	{
		vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	}
	vertices[vCounter].texture = XMFLOAT2(0.0f, 1.0f);

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
		vertices[vCounter].normal = XMFLOAT3(sin(theta), cos(theta), 0.0f);
		vertices[vCounter].texture = XMFLOAT2(sin(theta), cos(theta));

		vCounter++;

		lastX = newX;
		lastY = newY;

	}
}

void HiltCapMesh::generateSides(int circle1Start)
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