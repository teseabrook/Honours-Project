#include "HiltMeshGenerator.h"

#define DEBUG_SCALE_FACTOR 25.0f

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

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;


	if (set->getHStyle() > 1)
	{
		// 360 points on the circle, 1 extra for the centre
		vertexCount = 362 * 2;

		//360 * 3 vertexes, plus 360 * 6 for sides
		indexCount = 361 * 3 * 2 + 360 * 6;

		// Create the vertex and index array.
		vertices = new VertexType[vertexCount];
		indices = new unsigned long[indexCount];

		generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());

		if (set->getWType() != 0)
		{
			generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHLength() / DEBUG_SCALE_FACTOR), set->getHRadius());
		}
		else
		{
			generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHLength() / DEBUG_SCALE_FACTOR), set->getHTRadius());
		}


		generateSides(1);

		texture = set->getMaterial();
	}
	else if (set->getHStyle() == 1)
	{
		//4 per side for top/bottom, 4 per side for long sides
		vertexCount = 8 + 8;

		//6 for top/bottom, 6 for sides, 12 per side for long side
		indexCount = 24 + 24;

		// Create the vertex and index array.
		vertices = new VertexType[vertexCount];
		indices = new unsigned long[indexCount];

		generateSquare(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());
		generateSquare(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHLength() / DEBUG_SCALE_FACTOR), set->getHRadius());

		generateSquareSides(0);
		generateRecessSides(0, set->getHRadius());
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

void HiltMeshGenerator::generateCircle(XMFLOAT3 centre, float radius)
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

		if (i == (loops - 1))
		{
			indices[iCounter] = vCounterStart;
			iCounter++;
			indices[iCounter] = vCounterStart + 1;
			iCounter++;
			indices[iCounter] = vCounter - 1;
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

		vCounter++;

		lastX = newX;
		lastY = newY;

	}
}

void HiltMeshGenerator::generateSides(int circle1Start)
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

void HiltMeshGenerator::generateSquare(XMFLOAT3 centre, float radius)
{
	
	for (int i = 0; i < 4; i++)
	{
		float theta;
		switch (i)
		{
			case 0:
				theta = 30;
				break;
			case 1:
				theta = 150;
				break;
			case 2:
				theta = 210;
				break;
			case 3:
				theta = 330;
				break;
		}

		vertices[vCounter].position = XMFLOAT3((radius / DEBUG_SCALE_FACTOR) * sin((std::_Pi / 180) * (theta)) + centre.x, (radius / DEBUG_SCALE_FACTOR) * cos((std::_Pi / 180) * (theta)) + centre.y, centre.z);
		vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
		vCounter++;
	}

	indices[iCounter] = vCounter - 4;
	iCounter++;
	indices[iCounter] = vCounter - 2;
	iCounter++;
	indices[iCounter] = vCounter - 3;
	iCounter++;

	indices[iCounter] = vCounter - 4;
	iCounter++;
	indices[iCounter] = vCounter - 1;
	iCounter++;
	indices[iCounter] = vCounter - 2;
	iCounter++;

}

void HiltMeshGenerator::generateSquareSides(int start)
{
	indices[iCounter] = start;
	iCounter++;
	indices[iCounter] = start + 4;
	iCounter++;
	indices[iCounter] = start + 7;
	iCounter++;

	indices[iCounter] = start + 3;
	iCounter++;
	indices[iCounter] = start;
	iCounter++;
	indices[iCounter] = start + 7;
	iCounter++;

	indices[iCounter] = start + 6;
	iCounter++;
	indices[iCounter] = start + 5;
	iCounter++;
	indices[iCounter] = start + 1;
	iCounter++;

	indices[iCounter] = start + 2;
	iCounter++;
	indices[iCounter] = start + 6;
	iCounter++;
	indices[iCounter] = start + 1;
	iCounter++;

}

void HiltMeshGenerator::generateRecessSides(int start, float radius)
{
	float indent = (radius / DEBUG_SCALE_FACTOR * 0.3);

	//Generate the long sides

	//Top Right
	vertices[vCounter].position = vertices[start].position;
	vertices[vCounter].position.y -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 4].position;
	vertices[vCounter].position.y -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	indices[iCounter] = start;
	iCounter++;
	indices[iCounter] = start + 8;
	iCounter++;
	indices[iCounter] = start + 4;
	iCounter++;

	indices[iCounter] = start + 4;
	iCounter++;
	indices[iCounter] = start + 8;
	iCounter++;
	indices[iCounter] = start + 9;
	iCounter++;

	//Bottom Right
	vertices[vCounter].position = vertices[start + 1].position;
	vertices[vCounter].position.y += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 5].position;
	vertices[vCounter].position.y += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	indices[iCounter] = start + 10;
	iCounter++;
	indices[iCounter] = start + 1;
	iCounter++;
	indices[iCounter] = start + 5;
	iCounter++;

	indices[iCounter] = start + 10;
	iCounter++;
	indices[iCounter] = start + 5;
	iCounter++;
	indices[iCounter] = start + 11;
	iCounter++;

	//Top Left
	vertices[vCounter].position = vertices[start + 3].position;
	vertices[vCounter].position.y -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 7].position;
	vertices[vCounter].position.y -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	indices[iCounter] = start + 3;
	iCounter++;
	indices[iCounter] = start + 7;
	iCounter++;
	indices[iCounter] = start + 12;
	iCounter++;

	indices[iCounter] = start + 7;
	iCounter++;
	indices[iCounter] = start + 13;
	iCounter++;
	indices[iCounter] = start + 12;
	iCounter++;

	//Bottom Left
	vertices[vCounter].position = vertices[start + 2].position;
	vertices[vCounter].position.y += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 6].position;
	vertices[vCounter].position.y += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	indices[iCounter] = start + 14;
	iCounter++;
	indices[iCounter] = start + 6;
	iCounter++;
	indices[iCounter] = start + 2;
	iCounter++;

	indices[iCounter] = start + 14;
	iCounter++;
	indices[iCounter] = start + 15;
	iCounter++;
	indices[iCounter] = start + 6;
	iCounter++;
}
