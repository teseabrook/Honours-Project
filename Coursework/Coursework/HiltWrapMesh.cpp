#include "HiltWrapMesh.h"

#define DEBUG_SCALE_FACTOR 25.0f



HiltWrapMesh::HiltWrapMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, bool a_partial)
{
	partial = a_partial;
	set = a_set;
	initBuffers(device);
}


HiltWrapMesh::~HiltWrapMesh()
{
}

void HiltWrapMesh::initBuffers(ID3D11Device* device)
{
	int resolution = 100;

	int index, i, j;
	float positionX, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	if (!partial)
	{
		float numLoops = ceil((set->getHLength() / DEBUG_SCALE_FACTOR) / 25.0f);

		vertexCount = 360 * 2 + 360 * (1 + numLoops);


		indexCount = 360 * 6 * (3 + numLoops);
		vertices = new VertexType[vertexCount];
		indices = new unsigned long[indexCount];

		vCounter = 0;
		iCounter = 0;

		createCirclePoints(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());
		generateSpiralPoints(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());
		createCirclePoints(XMFLOAT3(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR)), set->getHRadius());
		generateSpiral(0);
		//generateSpiral(359);
	}
	else
	{
		float numLoops = ceil((set->getHLength() / DEBUG_SCALE_FACTOR) / 25.0f);

		vertexCount = 360 * 2 + 360 * 4 * (1 + 4 * numLoops);


		indexCount = 360 * 10 * (3 + numLoops);
		vertices = new VertexType[vertexCount];
		indices = new unsigned long[indexCount];

		vCounter = 0;
		iCounter = 0;

		createCirclePoints(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());
		generateSpiralPoints(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());
		generateSpiralPoints(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius(), 180);
		generateBackwardSpiralPoints(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius());
		generateBackwardSpiralPoints(XMFLOAT3(0.0f, 0.0f, -140.0f), set->getHRadius(), 180);
		createCirclePoints(XMFLOAT3(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR)), set->getHRadius(), 180);
		generatePartialSpiral(0);
		generateBackwardPartialSpiral(0);
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

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}

void HiltWrapMesh::createCirclePoints(XMFLOAT3 centre, float radius, int offset)
{
	int vCounterStart = vCounter;

	float lastX = 0.0f;
	float lastY = radius;

	int loops = 360;

	for (int i = 0; i < loops; i++)
	{
		//For any given triangle tN, it consists of the points (0, 0, 1/2HL) [Vertex 0], (xtN-1, ytN-1, 0) and (xtN, ytN, 0), forming a triangle of angles pi/180, 179pi/360 and 179pi/360 and sides hR, hR and sin(pi/180) * hR/sin(179pi/180)
		//when n = 0, xN and yN = 0
		//For any other n, the coordinates should be (hR * sin(theta), hR * cos(theta * i))
		float theta = (std::_Pi / 180) * (i + 1 + offset);

		float newX = (radius / DEBUG_SCALE_FACTOR) * sin(theta) + centre.x;
		float newY = (radius / DEBUG_SCALE_FACTOR) * cos(theta) + centre.y;
		vertices[vCounter].position = XMFLOAT3(newX, newY, centre.z);
		vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);

		vCounter++;

		lastX = newX;
		lastY = newY;

	}
}

void HiltWrapMesh::generateSpiralPoints(XMFLOAT3 centre, float radius, int offset)
{
	//Generates a spiral of points

	int vCounterStart = vCounter;


	float lastX = 0.0f;
	float lastY = radius;

	float numLoops = ceil((set->getHLength() / DEBUG_SCALE_FACTOR) / 25.0f);

	int loops = 360 * (numLoops + 2);
	noVPoints = loops;

	float maxHeight = -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR);

	for (int i = 0; i < loops; i++)
	{
		//For any given triangle tN, it consists of the points (0, 0, 1/2HL) [Vertex 0], (xtN-1, ytN-1, 0) and (xtN, ytN, 0), forming a triangle of angles pi/180, 179pi/360 and 179pi/360 and sides hR, hR and sin(pi/180) * hR/sin(179pi/180)
		//when n = 0, xN and yN = 0
		//For any other n, the coordinates should be (hR * sin(theta), hR * cos(theta * i))
		float theta = (std::_Pi / 180) * (i + 1 + offset);

		float newX = ((radius * 1.005) / DEBUG_SCALE_FACTOR) * sin(theta) + centre.x;
		float newY = ((radius * 1.005)  / DEBUG_SCALE_FACTOR) * cos(theta) + centre.y;
		float newZ = centre.z + (i / 25.0f);

		if (newZ > maxHeight)
		{
			newZ = maxHeight;
		}

		vertices[vCounter].position = XMFLOAT3(newX, newY, newZ);
		vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);


		vCounter++;

		lastX = newX;
		lastY = newY;
	}
}

void HiltWrapMesh::generateBackwardSpiralPoints(XMFLOAT3 centre, float radius, int offset)
{
	//Generates a spiral of points

	int vCounterStart = vCounter;


	float lastX = 0.0f;
	float lastY = radius;

	float numLoops = ceil((set->getHLength() / DEBUG_SCALE_FACTOR) / 25.0f);

	int loops = 360 * (numLoops + 2);
	noVPoints = loops;

	float maxHeight = -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR);

	for (int i = 0; i < loops; i++)
	{
		//For any given triangle tN, it consists of the points (0, 0, 1/2HL) [Vertex 0], (xtN-1, ytN-1, 0) and (xtN, ytN, 0), forming a triangle of angles pi/180, 179pi/360 and 179pi/360 and sides hR, hR and sin(pi/180) * hR/sin(179pi/180)
		//when n = 0, xN and yN = 0
		//For any other n, the coordinates should be (hR * sin(theta), hR * cos(theta * i))
		float theta = (std::_Pi / 180) * (360 - (i + 1 + offset));

		float newX = ((radius * 1.005) / DEBUG_SCALE_FACTOR) * sin(theta) + centre.x;
		float newY = ((radius * 1.005) / DEBUG_SCALE_FACTOR) * cos(theta) + centre.y;
		float newZ = centre.z + (i / 25.0f);

		if (newZ > maxHeight)
		{
			newZ = maxHeight;
		}

		vertices[vCounter].position = XMFLOAT3(newX, newY, newZ);
		vertices[vCounter].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertices[vCounter].texture = XMFLOAT2(0.0f, 0.0f);


		vCounter++;

		lastX = newX;
		lastY = newY;
	}
}

void HiltWrapMesh::generateSpiral(int start)
{
	float numLoops = ceil((set->getHLength() / DEBUG_SCALE_FACTOR) / 25.0f);
	int loops = 360 * (numLoops + 1);
	

	float height = 140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR);

	for (int i = 0; i < loops - 1; i++)
	{
		indices[iCounter] = i + start;
		iCounter++;

		indices[iCounter] = i + 1 + start;
		iCounter++;

		if (vertices[i + 360 + start].position.z <= height)
		{
			indices[iCounter] = i + 360 + start;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 720 + start;
			iCounter++;
		}

		indices[iCounter] = i + 1 + start;
		iCounter++;

		if (vertices[i + 361 + start].position.z <= height)
		{
			indices[iCounter] = i + 361 + start;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 722 + start;
			iCounter++;
		}

		if (vertices[i + 360 + start].position.z <= height)
		{
			indices[iCounter] = i + 360 + start;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 720 + start;
			iCounter++;
		}

	}

	//Test code
	/*indices[iCounter] = 0;
	iCounter++;

	indices[iCounter] = 1;
	iCounter++;

	indices[iCounter] = 360;
	iCounter++;

	indices[iCounter] = 1;
	iCounter++;

	indices[iCounter] = 361;
	iCounter++;

	indices[iCounter] = 360;
	iCounter++;*/
}

void HiltWrapMesh::generatePartialSpiral(int start)
{
	float numLoops = ceil((set->getHLength() / DEBUG_SCALE_FACTOR) / 25.0f);
	int loops = 360 * (numLoops + 2);


	float height = 140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR);
	for (int i = 0; i < 180; i++)
	{
		indices[iCounter] = i + start;
		iCounter++;

		indices[iCounter] = i + 1 + start;
		iCounter++;

		if (vertices[i + 360 + start].position.z <= height)
		{
			indices[iCounter] = i + 360 + start;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 720 + start;
			iCounter++;
		}

		indices[iCounter] = i + 1 + start;
		iCounter++;

		if (vertices[i + 361 + start].position.z <= height)
		{
			indices[iCounter] = i + 361 + start;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 722 + start;
			iCounter++;
		}

		if (vertices[i + 360 + start].position.z <= height)
		{
			indices[iCounter] = i + 360 + start;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 720 + start;
			iCounter++;
		}
	}
	//start += 180;

	

	for (int i = 0; i < loops - 181; i++)
	{


			//First Spiral (upper)
			if (vertices[i + start + 180 + 360].position.z <= height)
			{
				indices[iCounter] = i + start + 180 + 360;
			}
			else
			{
				indices[iCounter] = i + 180 + 360 + start + noVPoints * 2;
			}
			iCounter++;

			//Second spiral (lower)
			indices[iCounter] = i + 360 + start + noVPoints;
			iCounter++;

			//First Spiral (upper)
			if (vertices[i + 1 + start + 180 + 360].position.z <= height)
			{
				indices[iCounter] = i + 1 + start + 180 + 360;
			}
			else
			{
				indices[iCounter] = i + 1 + 180 + 360 + start + noVPoints * 2;
			}
			iCounter++;

			//First Spiral (upper)
			if (vertices[i + 1 + start + 180 + 360].position.z <= height)
			{
				indices[iCounter] = i + 1 + start + 180 + 360;
			}
			else
			{
				indices[iCounter] = i + 1 + 180 + 360 + start + noVPoints * 2;
			}
			iCounter++;

			//Second spiral (lower)
			indices[iCounter] = i + 360 + start + noVPoints;
			iCounter++;

			//Second spiral (lower)
			indices[iCounter] = i + 361 + start + noVPoints;
			iCounter++;
		

		
	}

}

void HiltWrapMesh::generateBackwardPartialSpiral(int start)
{
	float numLoops = ceil((set->getHLength() / DEBUG_SCALE_FACTOR) / 25.0f);
	int loops = 360 * (numLoops + 2);


	float height = 140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR);
	for (int i = 0; i < 180; i++)
	{
		indices[iCounter] = 360 - (i + start);
		iCounter++;

		if (vertices[i + 360 + start].position.z <= height)
		{
			indices[iCounter] = i + 360 + start + noVPoints * 2;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 720 + start;
			iCounter++;
		}

		indices[iCounter] = 360 - ( i + 1 + start);
		iCounter++;

		indices[iCounter] = 360 - (i + 1 + start);
		iCounter++;

		if (vertices[i + 360 + start].position.z <= height)
		{
			indices[iCounter] = i + 360 + start + noVPoints * 2;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 720 + start + noVPoints * 2;
			iCounter++;
		}

		if (vertices[i + 361 + start].position.z <= height)
		{
			indices[iCounter] = i + 361 + start + noVPoints * 2;
			iCounter++;
		}
		else
		{
			indices[iCounter] = i + 722 + start + noVPoints * 2;
			iCounter++;
		}

		
	}
	//start += 180;



	for (int i = 0; i < loops - 181; i++)
	{


		//First Spiral (upper)
		if (vertices[i + start + 180 + 360].position.z <= height)
		{
			indices[iCounter] = i + start + 180 + 360 + noVPoints * 2;
		}
		else
		{
			indices[iCounter] = i + 180 + 360 + start + noVPoints * 4;
		}
		iCounter++;

		//First Spiral (upper)
		if (vertices[i + 1 + start + 180 + 360].position.z <= height)
		{
			indices[iCounter] = i + 1 + start + 180 + 360 + noVPoints * 2;
		}
		else
		{
			indices[iCounter] = i + 1 + 180 + 360 + start + noVPoints * 2 + noVPoints * 2;
		}
		iCounter++;

		//Second spiral (lower)
		indices[iCounter] = i + 360 + start + noVPoints + noVPoints * 2;
		iCounter++;

		//First Spiral (upper)
		if (vertices[i + 1 + start + 180 + 360].position.z <= height)
		{
			indices[iCounter] = i + 1 + start + 180 + 360 + noVPoints * 2;
		}
		else
		{
			indices[iCounter] = i + 1 + 180 + 360 + start + noVPoints * 2 + noVPoints * 2;
		}
		iCounter++;

		//Second spiral (lower)
		indices[iCounter] = i + 361 + start + noVPoints + noVPoints * 2;
		iCounter++;

		//Second spiral (lower)
		indices[iCounter] = i + 360 + start + noVPoints + noVPoints * 2;
		iCounter++;



	}

}
