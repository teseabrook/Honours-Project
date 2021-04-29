#include "SwordBladeMesh.h"
#define DEBUG_SCALE_FACTOR 25.0f


SwordBladeMesh::SwordBladeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, int lresolution)
{
	set = a_set;
	resolution = lresolution;
	initBuffers(device);
}


SwordBladeMesh::~SwordBladeMesh()
{
	BaseMesh::~BaseMesh();
}

void SwordBladeMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// 6 vertices per quad, res*res is face, times 6 for each face
	vertexCount = 40;

	indexCount = 400;

	// Create the vertex and index array.
	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	vertices[0].position = XMFLOAT3(0, -((set->getBBreadth() / DEBUG_SCALE_FACTOR) * 0.5) , 0);
	vertices[0].texture = XMFLOAT2(0, 0);
	vertices[0].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices[1].position = XMFLOAT3((set->getBWidth() / DEBUG_SCALE_FACTOR) * 0.5, -((set->getBBreadth() / DEBUG_SCALE_FACTOR) * 0.5) + (set->getFWidth() / DEBUG_SCALE_FACTOR), 0);
	vertices[1].texture = XMFLOAT2(0, 0);
	vertices[1].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[2].position = XMFLOAT3(0, ((set->getBBreadth() / DEBUG_SCALE_FACTOR) * 0.5), 0);
	vertices[2].texture = XMFLOAT2(0, 0);
	vertices[2].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	vertices[3].position = XMFLOAT3(-((set->getBWidth() / DEBUG_SCALE_FACTOR) * 0.5), -((set->getBBreadth() / DEBUG_SCALE_FACTOR) * 0.5) + (set->getFWidth() / DEBUG_SCALE_FACTOR), 0);
	vertices[3].texture = XMFLOAT2(0, 0);
	vertices[3].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	

	vertices[4].position = XMFLOAT3(0, -((set->getBBreadth() / DEBUG_SCALE_FACTOR) * 0.5), (set->getBLength() / DEBUG_SCALE_FACTOR));
	vertices[4].texture = XMFLOAT2(0, 0);
	vertices[4].normal = XMFLOAT3(0.0f, -1.0f, 1.0f);

	vertices[5].position = XMFLOAT3((set->getBWidth() / DEBUG_SCALE_FACTOR) * 0.5, -((set->getBBreadth() / DEBUG_SCALE_FACTOR) * 0.5) + (set->getFWidth() / DEBUG_SCALE_FACTOR), (set->getBLength() / DEBUG_SCALE_FACTOR));
	vertices[5].texture = XMFLOAT2(0, 0);
	vertices[5].normal = XMFLOAT3(1.0f, -1.0f, 1.0f);

	vertices[6].position = XMFLOAT3(0, ((set->getBBreadth() / DEBUG_SCALE_FACTOR) * 0.5), (set->getBLength() / DEBUG_SCALE_FACTOR));
	vertices[6].texture = XMFLOAT2(0, 0);
	vertices[6].normal = XMFLOAT3(0.0f, 1.0f, 1.0f);

	vertices[7].position = XMFLOAT3(-((set->getBWidth() / DEBUG_SCALE_FACTOR) * 0.5), -((set->getBBreadth() / DEBUG_SCALE_FACTOR) * 0.5) + (set->getFWidth() / DEBUG_SCALE_FACTOR), (set->getBLength() / DEBUG_SCALE_FACTOR));
	vertices[7].texture = XMFLOAT2(0, 0);
	vertices[7].normal = XMFLOAT3(-1.0f, -1.0f, 1.0f);

	vertices[4].position.y -= (set->getHCLength() / DEBUG_SCALE_FACTOR);
	vertices[5].position.y -= (set->getHCLength() / DEBUG_SCALE_FACTOR);
	vertices[6].position.y -= (set->getHCLength() / DEBUG_SCALE_FACTOR);
	vertices[7].position.y -= (set->getHCLength() / DEBUG_SCALE_FACTOR);

	vertices[4].position.z -= ((set->getHCLength() / DEBUG_SCALE_FACTOR) / 2);
	vertices[5].position.z -= ((set->getHCLength() / DEBUG_SCALE_FACTOR) / 4);
	vertices[7].position.z -= ((set->getHCLength() / DEBUG_SCALE_FACTOR) / 4);


	//Now we interpolate
	//Between P2 and 3
	XMFLOAT3 distance, newPoint;
	float m;

	distance = XMFLOAT3(vertices[2].position.x - vertices[3].position.x, vertices[2].position.y - vertices[3].position.y, vertices[2].position.z - vertices[3].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	newPoint.x = ((float)vertices[3].position.x + distance.x * (m / 4));
	newPoint.y = ((float)vertices[3].position.y + distance.y * (m / 4));
	newPoint.z = ((float)vertices[3].position.z + distance.z * (m / 4));

	vertices[8].position = newPoint;
	vertices[8].texture = XMFLOAT2(0, 0);
	vertices[8].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	newPoint.x = ((float)vertices[3].position.x + distance.x * (m / 2));
	newPoint.y = ((float)vertices[3].position.y + distance.y * (m / 2));
	newPoint.z = ((float)vertices[3].position.z + distance.z * (m / 2));

	vertices[9].position = newPoint;
	vertices[9].texture = XMFLOAT2(0, 0);
	vertices[9].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);


	newPoint.x = ((float)vertices[3].position.x + distance.x * (m * 3 / 4));
	newPoint.y = ((float)vertices[3].position.y + distance.y * (m * 3 / 4));
	newPoint.z = ((float)vertices[3].position.z + distance.z * (m * 3 / 4));

	vertices[10].position = newPoint;
	vertices[10].texture = XMFLOAT2(0, 0);
	vertices[10].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	distance = XMFLOAT3(vertices[9].position.x - 0, vertices[9].position.y - vertices[9].position.y, vertices[9].position.z - vertices[9].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	newPoint.x = ((float)vertices[8].position.x - distance.x * (m / 4));
	newPoint.y = ((float)vertices[8].position.y - distance.y * (m / 4));
	newPoint.z = ((float)vertices[8].position.z - distance.z * (m / 4));

	vertices[11].position = newPoint;
	vertices[11].texture = XMFLOAT2(0, 0);
	vertices[11].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);

	newPoint.x = ((float)vertices[9].position.x - distance.x * (m / 3));
	newPoint.y = ((float)vertices[9].position.y - distance.y * (m / 3));
	newPoint.z = ((float)vertices[9].position.z - distance.z * (m / 3));

	vertices[12].position = newPoint;
	vertices[12].texture = XMFLOAT2(0, 0);
	vertices[12].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);

	newPoint.x = ((float)vertices[10].position.x - distance.x * (m / 4));
	newPoint.y = ((float)vertices[10].position.y - distance.y * (m / 4));
	newPoint.z = ((float)vertices[10].position.z - distance.z * (m / 4));

	vertices[13].position = newPoint;
	vertices[13].texture = XMFLOAT2(0, 0);
	vertices[13].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);




	vertices[14].position = vertices[11].position;
	vertices[14].position.x = -vertices[11].position.x;
	vertices[14].texture = XMFLOAT2(0, 0);
	vertices[14].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);

	vertices[15].position = vertices[12].position;
	vertices[15].position.x = -vertices[12].position.x;
	vertices[15].texture = XMFLOAT2(0, 0);
	vertices[15].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);

	vertices[16].position = vertices[13].position;
	vertices[16].position.x = -vertices[13].position.x;
	vertices[16].texture = XMFLOAT2(0, 0);
	vertices[16].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);



	vertices[17].position = vertices[11].position;
	vertices[17].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[17].texture = XMFLOAT2(0, 0);
	vertices[17].normal = vertices[11].normal;

	vertices[18].position = vertices[12].position;
	vertices[18].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[18].texture = XMFLOAT2(0, 0);
	vertices[18].normal = vertices[12].normal;

	vertices[19].position = vertices[13].position;
	vertices[19].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[19].texture = XMFLOAT2(0, 0);
	vertices[19].normal = vertices[13].normal;



	vertices[20].position = vertices[14].position;
	vertices[20].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[20].texture = XMFLOAT2(0, 0);
	vertices[20].normal = vertices[14].normal;

	vertices[21].position = vertices[15].position;
	vertices[21].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[21].texture = XMFLOAT2(0, 0);
	vertices[21].normal = vertices[15].normal;

	vertices[22].position = vertices[16].position;
	vertices[22].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[22].texture = XMFLOAT2(0, 0);
	vertices[22].normal = vertices[16].normal;


	//Now between 3 and 0


	distance = XMFLOAT3(vertices[3].position.x - vertices[0].position.x, vertices[3].position.y - vertices[0].position.y, vertices[3].position.z - vertices[0].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	newPoint.x = ((float)vertices[3].position.x - distance.x * (m / 4));
	newPoint.y = ((float)vertices[3].position.y - distance.y * (m / 4));
	newPoint.z = ((float)vertices[3].position.z - distance.z * (m / 4));

	vertices[23].position = newPoint;
	vertices[23].texture = XMFLOAT2(0, 0);
	vertices[23].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	newPoint.x = ((float)vertices[3].position.x - distance.x * (m / 2));
	newPoint.y = ((float)vertices[3].position.y - distance.y * (m / 2));
	newPoint.z = ((float)vertices[3].position.z - distance.z * (m / 2));

	vertices[24].position = newPoint;
	vertices[24].texture = XMFLOAT2(0, 0);
	vertices[24].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);


	newPoint.x = ((float)vertices[3].position.x - distance.x * (m * 3 / 4));
	newPoint.y = ((float)vertices[3].position.y - distance.y * (m * 3 / 4));
	newPoint.z = ((float)vertices[3].position.z - distance.z * (m * 3 / 4));

	vertices[25].position = newPoint;
	vertices[25].texture = XMFLOAT2(0, 0);
	vertices[25].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);



	distance = XMFLOAT3(vertices[24].position.x - 0, vertices[24].position.y - vertices[24].position.y, vertices[24].position.z - vertices[24].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	newPoint.x = ((float)vertices[23].position.x - distance.x * (m / 6));
	newPoint.y = ((float)vertices[23].position.y - distance.y * (m / 6));
	newPoint.z = ((float)vertices[23].position.z - distance.z * (m / 6));

	vertices[26].position = newPoint;
	vertices[26].texture = XMFLOAT2(0, 0);
	vertices[26].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);

	newPoint.x = ((float)vertices[24].position.x - distance.x * (m / 4));
	newPoint.y = ((float)vertices[24].position.y - distance.y * (m / 4));
	newPoint.z = ((float)vertices[24].position.z - distance.z * (m / 4));

	vertices[27].position = newPoint;
	vertices[27].texture = XMFLOAT2(0, 0);
	vertices[27].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);

	newPoint.x = ((float)vertices[25].position.x - distance.x * (m / 6));
	newPoint.y = ((float)vertices[25].position.y - distance.y * (m / 6));
	newPoint.z = ((float)vertices[25].position.z - distance.z * (m / 6));

	vertices[28].position = newPoint;
	vertices[28].texture = XMFLOAT2(0, 0);
	vertices[28].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);




	vertices[29].position = vertices[26].position;
	vertices[29].position.x = -vertices[26].position.x;
	vertices[29].texture = XMFLOAT2(0, 0);
	vertices[29].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[30].position = vertices[27].position;
	vertices[30].position.x = -vertices[27].position.x;
	vertices[30].texture = XMFLOAT2(0, 0);
	vertices[30].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[31].position = vertices[28].position;
	vertices[31].position.x = -vertices[28].position.x;
	vertices[31].texture = XMFLOAT2(0, 0);
	vertices[31].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);



	vertices[32].position = vertices[26].position;
	vertices[32].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[32].texture = XMFLOAT2(0, 0);
	vertices[32].normal = vertices[26].normal;

	vertices[33].position = vertices[27].position;
	vertices[33].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[33].texture = XMFLOAT2(0, 0);
	vertices[33].normal = vertices[27].normal;

	vertices[34].position = vertices[28].position;
	vertices[34].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[34].texture = XMFLOAT2(0, 0);
	vertices[34].normal = vertices[28].normal;



	vertices[35].position = vertices[29].position;
	vertices[35].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[35].texture = XMFLOAT2(0, 0);
	vertices[35].normal = vertices[29].normal;

	vertices[36].position = vertices[30].position;
	vertices[36].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[36].texture = XMFLOAT2(0, 0);
	vertices[36].normal = vertices[30].normal;

	vertices[37].position = vertices[31].position;
	vertices[37].position.z = set->getBLength() / DEBUG_SCALE_FACTOR;
	vertices[37].texture = XMFLOAT2(0, 0);
	vertices[37].normal = vertices[31].normal;




	vertices[38].position = XMFLOAT3(0, vertices[4].position.y, vertices[4].position.z + (set->getPLength() / DEBUG_SCALE_FACTOR));
	vertices[38].texture = XMFLOAT2(0, 0);
	vertices[38].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

	//face 1
	/*indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;

	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 1;

	indices[6] = 3;
	indices[7] = 2;
	indices[8] = 1;

	indices[9] = 3;
	indices[10] = 1;
	indices[11] = 2;


	//face 2
	indices[12] = 4;
	indices[13] = 5;
	indices[14] = 7;

	indices[15] = 4;
	indices[16] = 7;
	indices[17] = 5;

	indices[18] = 7;
	indices[19] = 6;
	indices[20] = 5;

	indices[21] = 7;
	indices[22] = 5;
	indices[23] = 6;*/





	/*indices[24] = 6;
	indices[25] = 8;
	indices[26] = 3;

	indices[27] = 6;
	indices[28] = 3;
	indices[29] = 8;

	indices[30] = 6;
	indices[31] = 8;
	indices[32] = 9;

	indices[33] = 6;
	indices[34] = 9;
	indices[35] = 8;

	indices[36] = 6;
	indices[37] = 10;
	indices[38] = 9;

	indices[39] = 6;
	indices[40] = 9;
	indices[41] = 10;

	indices[42] = 6;
	indices[43] = 10;
	indices[44] = 2;

	indices[45] = 6;
	indices[46] = 2;
	indices[47] = 10;*/
	

	int iCounter = 0;

	int a, b, c, d, e, f, g, h, i, j;
	

	a = 2;
	b = 3;

	c = 13;
	d = 12;
	e = 11;

	f = 6;
	g = 7;
	
	h = 19;
	i = 18;
	j = 17;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;


	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;



	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;


	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;


	a = 2;
	b = 1;

	c = 16;
	d = 15;
	e = 14;

	f = 6;
	g = 5;

	h = 22;
	i = 21;
	j = 20;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;


	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;



	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;


	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;



	a = 3;
	b = 0;

	c = 26;
	d = 27;
	e = 28;

	f = 7;
	g = 4;

	h = 32;
	i = 33;
	j = 34;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;


	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;



	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;


	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;



	a = 1;
	b = 0;

	c = 29;
	d = 30;
	e = 31;

	f = 5;
	g = 4;

	h = 35;
	i = 36;
	j = 37;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;


	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;



	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;


	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;



	//Sword Point
	a = 38;

	b = 6;
	c = 7;

	d = 19;
	e = 18;
	f = 17;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;


	b = 6;
	c = 7;

	d = 19;
	e = 18;
	f = 17;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;


	b = 4;
	c = 7;

	d = 34;
	e = 33;
	f = 32;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	b = 4;
	c = 5;

	d = 37;
	e = 36;
	f = 35;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

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
