#include "ClawMesh.h"

#define DEBUG_SCALE_FACTOR 25.0f



ClawMesh::ClawMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set)
{
	set = a_set;
	initBuffers(device);
}


ClawMesh::~ClawMesh()
{
}

void ClawMesh::initBuffers(ID3D11Device* device)
{
	int resolution = 20;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 362 * 10;
	indexCount = 362 * 60 * 2;

	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	// Load the vertex array with data.

	//Generate a bunch of circles of decreasing radius and then connect them to form the claw
	generateCircle(XMFLOAT3(0.0f, 0.0f, 0.0f), set->getPRadius() / 10);
	generateCircle(XMFLOAT3(0.0f, 0.0f, -((set->getPRadius() / DEBUG_SCALE_FACTOR) * 0.1f)), set->getHRadius() / 12);
	generateCircle(XMFLOAT3(0.0f, ((set->getHRadius() / DEBUG_SCALE_FACTOR) * 0.025f), -((set->getHRadius() / DEBUG_SCALE_FACTOR) * 0.2f)), set->getPRadius() / 14);
	generateCircle(XMFLOAT3(0.0f, ((set->getHRadius() / DEBUG_SCALE_FACTOR) * 0.05f), -((set->getHRadius() / DEBUG_SCALE_FACTOR) * 0.3f)), set->getPRadius() / 16);
	generateCircle(XMFLOAT3(0.0f, ((set->getHRadius() / DEBUG_SCALE_FACTOR) * 0.025f), -((set->getHRadius() / DEBUG_SCALE_FACTOR) * 0.4f)), set->getPRadius() / 18);
	generateCircle(XMFLOAT3(0.0f, -((set->getHRadius() / DEBUG_SCALE_FACTOR) * 0.1f), -((set->getHRadius() / DEBUG_SCALE_FACTOR) * 0.5f)), set->getPRadius() / 20);
	generateSides(0);
	generateSides(363);
	generateSides(725);
	generateSides(1087);
	generateSides(1449);


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

//Create a circle composed of 360 1 degree triangle slices
void ClawMesh::generateCircle(XMFLOAT3 centre, float radius, bool invert)
{
	int vCounterStart = vCounter;

	vertices[vCounter].position = XMFLOAT3(centre.x, centre.y, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

	vCounter++;

	vertices[vCounter].position = XMFLOAT3(centre.x, radius / DEBUG_SCALE_FACTOR + centre.y, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
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
		vertices[vCounter].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertices[vCounter].texture = XMFLOAT2(sin(theta), cos(theta));

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

		vCounter++;

		lastX = newX;
		lastY = newY;

	}
}

//Same as the last one but doesnt fill indices, used for efficiency
void ClawMesh::generateInvisCircle(XMFLOAT3 centre, float radius)
{
	int vCounterStart = vCounter;

	vertices[vCounter].position = XMFLOAT3(centre.x, centre.y, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

	vCounter++;

	vertices[vCounter].position = XMFLOAT3(centre.x, radius / DEBUG_SCALE_FACTOR + centre.y, centre.z);
	vertices[vCounter].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
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
		vertices[vCounter].normal = XMFLOAT3(0, 1.0f, 0.0f);
		vertices[vCounter].texture = XMFLOAT2(sin(theta), cos(theta));

		vCounter++;

		lastX = newX;
		lastY = newY;

	}
}

//Connect two circles together
void ClawMesh::generateSides(int circle1Start)
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