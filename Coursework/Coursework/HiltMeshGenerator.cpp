#include "HiltMeshGenerator.h"

#define DEBUG_SCALE_FACTOR 25.0f

HiltMeshGenerator::HiltMeshGenerator(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set)
{
	set = a_set;
	dContext = deviceContext;
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

	//Skip normal gen if we're fully wrapped
	if (set->getHWStyle() == 2)
	{
		// 360 points on the circle, 1 extra for the centre
		vertexCount = 362 * 2;

		//360 * 3 vertexes, plus 360 * 6 for sides
		indexCount = 361 * 3 * 2 + 360 * 6;

		// Create the vertex and index array.
		vertices = new VertexType[vertexCount];
		indices = new unsigned long[indexCount];

		generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());

		
		generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHLength() / DEBUG_SCALE_FACTOR), set->getHRadius(), true);
		

		texture = set->getMaterial();

		wrapMesh = new HiltWrapMesh(device, dContext, set);

		hiltCaps[0] = new HiltCapMesh(device, dContext, set);
		hiltCaps[1] = new HiltCapMesh(device, dContext, set);
	}
	else
	{
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
				generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHLength() / DEBUG_SCALE_FACTOR), set->getHRadius(), true);
			}
			else
			{
				generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHLength() / DEBUG_SCALE_FACTOR), set->getHTRadius(), true);
			}


			generateSides(1);

			texture = set->getMaterial();
		}
		else if (set->getHStyle() == 1)
		{
			//4 per side for top/bottom, 8 per side for long sides, 4 per side for each wall thing
			vertexCount = 8 + 16 + 8;

			//6 for top/bottom, 6 for sides, 24 per side for long side, 6 * 4 per side for the walls
			indexCount = 24 + (24 * 2) + (6 * 4 * 2);

			// Create the vertex and index array.
			vertices = new VertexType[vertexCount];
			indices = new unsigned long[indexCount];

			generateSquare(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius(), false);
			generateSquare(XMFLOAT3(0.0f, 0.0f, -140.0f + set->getHLength() / DEBUG_SCALE_FACTOR), set->getHRadius(), true);

			generateSquareSides(0);
			generateRecessSides(0, set->getHRadius());
			generateWallThings(16, ((set->getHRadius() / DEBUG_SCALE_FACTOR * 0.3) * 0.25));

			XMFLOAT2 size;
			size.x = abs(vertices[16 + 8].position.y - vertices[16 + 9].position.y);
			size.y = abs(vertices[16 + 8].position.z - vertices[16 + 10].position.z);

			HighPolyDeformMesh* m = new HighPolyDeformMesh(device, dContext, set, vertices[16 + 8].position, size);
			deformMeshes.push_back(m);

			HighPolyDeformMesh* m1 = new HighPolyDeformMesh(device, dContext, set, vertices[16 + 12].position, size, true);
			deformMeshes.push_back(m1);
		}
		else if (set->getHStyle() == 0)
		{
			float length = ceil(set->getHLength() / DEBUG_SCALE_FACTOR);

			// 360 points on the circle, 1 extra for the centre
			vertexCount = 362 * (2 + length);

			//360 * 3 vertexes, plus 360 * 6 for sides
			indexCount = 361 * 3 * 2 + 361 * (6 * length);

			// Create the vertex and index array.
			vertices = new VertexType[vertexCount];
			indices = new unsigned long[indexCount];

			generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());

			bool shrink = true;

			for (int i = 0; i < length; i++)
			{
				if (i != 0 && i % 2 == 0)
				{
					if (shrink)
					{
						generateInvisCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + i), set->getHRadius() * 0.9f);
						shrink = false;
					}
					else
					{
						generateInvisCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + i), set->getHRadius());
						shrink = true;
					}
				}
			}

			generateCircle(XMFLOAT3(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR)), set->getHRadius(), true);

			for (int i = 0; i < length / 2; i++)
			{
				generateSides(362 * i + 1);
			}

			texture = set->getMaterial();


		}

		if (set->getHWStyle() == 1)
		{
			wrapMesh = new HiltWrapMesh(device, dContext, set, true);
			hiltCaps[0] = new HiltCapMesh(device, dContext, set);
			hiltCaps[1] = new HiltCapMesh(device, dContext, set);
		}
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

void HiltMeshGenerator::generateCircle(XMFLOAT3 centre, float radius, bool invert)
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

	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

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
		vertices[vCounter].normal = XMFLOAT3(sin(theta), cos(theta), 0.0f);
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

		if (i == (loops - 1))
		{
			if (invert)
			{
				indices[iCounter] = vCounterStart;
				iCounter++;
				indices[iCounter] = vCounterStart -1;
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

void HiltMeshGenerator::generateInvisCircle(XMFLOAT3 centre, float radius)
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
		vertices[vCounter].normal = XMFLOAT3(sin(theta), cos(theta), 0.0f);
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	
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

void HiltMeshGenerator::generateSquare(XMFLOAT3 centre, float radius, bool invert = false)
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
		if (invert)
		{
			vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		}
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
		vCounter++;
	}

	if (invert)
	{
		indices[iCounter] = vCounter - 4;
		iCounter++;
		indices[iCounter] = vCounter - 3;
		iCounter++;
		indices[iCounter] = vCounter - 2;
		iCounter++;

		indices[iCounter] = vCounter - 4;
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
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 4].position;
	vertices[vCounter].position.y -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
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
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 5].position;
	vertices[vCounter].position.y += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
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
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 7].position;
	vertices[vCounter].position.y -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
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
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 6].position;
	vertices[vCounter].position.y += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
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

	//Left Close
	vertices[vCounter].position = vertices[start + 8].position;
	vertices[vCounter].position.z += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 10].position;
	vertices[vCounter].position.z += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	indices[iCounter] = start + 16;
	iCounter++;
	indices[iCounter] = start + 8;
	iCounter++;
	indices[iCounter] = start + 10;
	iCounter++;

	indices[iCounter] = start + 10;
	iCounter++;
	indices[iCounter] = start + 17;
	iCounter++;
	indices[iCounter] = start + 16;
	iCounter++;

	//Left Far
	vertices[vCounter].position = vertices[start + 9].position;
	vertices[vCounter].position.z -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 11].position;
	vertices[vCounter].position.z -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	indices[iCounter] = start + 18;
	iCounter++;
	indices[iCounter] = start + 11;
	iCounter++;
	indices[iCounter] = start + 9;
	iCounter++;

	indices[iCounter] = start + 11;
	iCounter++;
	indices[iCounter] = start + 18;
	iCounter++;
	indices[iCounter] = start + 19;
	iCounter++;

	//Right Close
	vertices[vCounter].position = vertices[start + 12].position;
	vertices[vCounter].position.z += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 14].position;
	vertices[vCounter].position.z += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	indices[iCounter] = start + 20;
	iCounter++;
	indices[iCounter] = start + 14;
	iCounter++;
	indices[iCounter] = start + 12;
	iCounter++;

	indices[iCounter] = start + 14;
	iCounter++;
	indices[iCounter] = start + 20;
	iCounter++;
	indices[iCounter] = start + 21;
	iCounter++;

	//Right Far
	vertices[vCounter].position = vertices[start + 13].position;
	vertices[vCounter].position.z -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 15].position;
	vertices[vCounter].position.z -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	indices[iCounter] = start + 22;
	iCounter++;
	indices[iCounter] = start + 13;
	iCounter++;
	indices[iCounter] = start + 15;
	iCounter++;

	indices[iCounter] = start + 15;
	iCounter++;
	indices[iCounter] = start + 23;
	iCounter++;
	indices[iCounter] = start + 22;
	iCounter++;
}

void HiltMeshGenerator::generateWallThings(int start, float indent)
{
	//Verticies
	vertices[vCounter].position = vertices[start].position;
	vertices[vCounter].position.x -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 1].position;
	vertices[vCounter].position.x -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 2].position;
	vertices[vCounter].position.x -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 3].position;
	vertices[vCounter].position.x -= indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 4].position;
	vertices[vCounter].position.x += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 5].position;
	vertices[vCounter].position.x += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 6].position;
	vertices[vCounter].position.x += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	vertices[vCounter].position = vertices[start + 7].position;
	vertices[vCounter].position.x += indent;
	vertices[vCounter].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);
	vCounter++;

	//Right Side
	indices[iCounter] = start;
	iCounter++;
	indices[iCounter] = start + 1;
	iCounter++;
	indices[iCounter] = start + 8;
	iCounter++;

	indices[iCounter] = start + 1;
	iCounter++;
	indices[iCounter] = start + 9;
	iCounter++;
	indices[iCounter] = start + 8;
	iCounter++;

	indices[iCounter] = start + 2;
	iCounter++;
	indices[iCounter] = start + 10;
	iCounter++;
	indices[iCounter] = start + 3;
	iCounter++;

	indices[iCounter] = start + 3;
	iCounter++;
	indices[iCounter] = start + 10;
	iCounter++;
	indices[iCounter] = start + 11;
	iCounter++;

	indices[iCounter] = start;
	iCounter++;
	indices[iCounter] = start + 8;
	iCounter++;
	indices[iCounter] = start + 10;
	iCounter++;

	indices[iCounter] = start;
	iCounter++;
	indices[iCounter] = start + 10;
	iCounter++;
	indices[iCounter] = start + 2;
	iCounter++;

	indices[iCounter] = start + 1;
	iCounter++;
	indices[iCounter] = start + 11;
	iCounter++;
	indices[iCounter] = start + 9;
	iCounter++;

	indices[iCounter] = start + 1;
	iCounter++;
	indices[iCounter] = start + 3;
	iCounter++;
	indices[iCounter] = start + 11;
	iCounter++;

	//Left Side
	indices[iCounter] = start + 4;
	iCounter++;
	indices[iCounter] = start + 6;
	iCounter++;
	indices[iCounter] = start + 12;
	iCounter++;

	indices[iCounter] = start + 12;
	iCounter++;
	indices[iCounter] = start + 6;
	iCounter++;
	indices[iCounter] = start + 14;
	iCounter++;

	indices[iCounter] = start + 5;
	iCounter++;
	indices[iCounter] = start + 13;
	iCounter++;
	indices[iCounter] = start + 7;
	iCounter++;

	indices[iCounter] = start + 13;
	iCounter++;
	indices[iCounter] = start + 15;
	iCounter++;
	indices[iCounter] = start + 7;
	iCounter++;

	indices[iCounter] = start + 6;
	iCounter++;
	indices[iCounter] = start + 7;
	iCounter++;
	indices[iCounter] = start + 15;
	iCounter++;

	indices[iCounter] = start + 6;
	iCounter++;
	indices[iCounter] = start + 15;
	iCounter++;
	indices[iCounter] = start + 14;
	iCounter++;

	indices[iCounter] = start + 4;
	iCounter++;
	indices[iCounter] = start + 13;
	iCounter++;
	indices[iCounter] = start + 5;
	iCounter++;

	indices[iCounter] = start + 4;
	iCounter++;
	indices[iCounter] = start + 12;
	iCounter++;
	indices[iCounter] = start + 13;
	iCounter++;

	
	
}
