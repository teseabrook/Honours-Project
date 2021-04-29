#include "SpearBaseMesh.h"
#define DEBUG_SCALE_FACTOR 25.0f


SpearBaseMesh::SpearBaseMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ParameterSet* a_set, int lresolution)
{
	set = a_set;
	resolution = lresolution;
	initBuffers(device);
}


SpearBaseMesh::~SpearBaseMesh()
{
}

void SpearBaseMesh::initBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// 6 vertices per quad, res*res is face, times 6 for each face
	vertexCount = 80;
	indexCount = 1000;

	vertices = new VertexType[vertexCount];
	indices = new unsigned long[indexCount];

	vertices[0].position = XMFLOAT3(0, 0, -((set->getSBHeigh() / DEBUG_SCALE_FACTOR) / 4));  // Centre Bottom
	vertices[0].texture = XMFLOAT2(0, 0);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[1].position = XMFLOAT3(0, 0, (set->getSBHeigh() / DEBUG_SCALE_FACTOR) - ((set->getSBHeigh() / DEBUG_SCALE_FACTOR) / 4));  //Centre
	vertices[1].texture = XMFLOAT2(0, 0);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[2].position = XMFLOAT3(0, set->getStWidth() / DEBUG_SCALE_FACTOR, (set->getSBHeigh() / DEBUG_SCALE_FACTOR) - ((set->getSBHeigh() / DEBUG_SCALE_FACTOR) / 4));  //Top
	vertices[2].texture = XMFLOAT2(0, 0);
	vertices[2].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	vertices[3].position = XMFLOAT3(0, -(set->getStWidth() / DEBUG_SCALE_FACTOR), (set->getSBHeigh() / DEBUG_SCALE_FACTOR) - ((set->getSBHeigh() / DEBUG_SCALE_FACTOR) / 4));  //Bottom
	vertices[3].texture = XMFLOAT2(0, 0);
	vertices[3].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices[4].position = XMFLOAT3((set->getBWidth() / DEBUG_SCALE_FACTOR), 0, (set->getSBHeigh() / DEBUG_SCALE_FACTOR) - ((set->getSBHeigh() / DEBUG_SCALE_FACTOR) / 4));  //Right
	vertices[4].texture = XMFLOAT2(0, 0);
	vertices[4].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

	vertices[5].position = XMFLOAT3(-(set->getBWidth() / DEBUG_SCALE_FACTOR), 0, (set->getSBHeigh() / DEBUG_SCALE_FACTOR) - ((set->getSBHeigh() / DEBUG_SCALE_FACTOR) / 4));  //Left
	vertices[5].texture = XMFLOAT2(0, 0);
	vertices[5].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

	//First, find the point on the line between points 0 and 2
	XMFLOAT3 distance;
	distance.x = (vertices[2].position.x - vertices[0].position.x);
	distance.y = (vertices[2].position.y - vertices[0].position.y);
	distance.z = (vertices[2].position.z - vertices[0].position.z);

	float m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	XMFLOAT3 intPoint;
	intPoint.x = (int)((float)vertices[0].position.x + distance.x * (m / 2));
	intPoint.y = (int)((float)vertices[0].position.y + distance.y * (m / 2));
	intPoint.z = (int)((float)vertices[0].position.z + distance.z * (m / 2));

	//Now, repeat the process between the new point and v0 + v1.z
	distance.x = (vertices[1].position.x - intPoint.x);
	distance.y = (vertices[1].position.y - intPoint.y);
	distance.z = (vertices[1].position.z - intPoint.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	XMFLOAT3 newPoint;
	newPoint.x = ((float)intPoint.x + distance.x * (set->getHGAmount() / DEBUG_SCALE_FACTOR));
	newPoint.y = ((float)intPoint.y + distance.y * (set->getHGAmount() / DEBUG_SCALE_FACTOR));
	newPoint.z = ((float)intPoint.z + distance.z * (set->getHGAmount() / DEBUG_SCALE_FACTOR));

	vertices[6].position = XMFLOAT3(0, 0, newPoint.z);  //Interpolated Point, Middle
	vertices[6].texture = XMFLOAT2(0, 0);
	vertices[6].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[7].position = newPoint;  //Interpolated Point, Top
	vertices[7].texture = XMFLOAT2(0, 0);
	vertices[7].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);


	vertices[9].position = newPoint;  //Interpolated Point, Bottom
	vertices[9].position.y = -vertices[9].position.y;
	vertices[9].texture = XMFLOAT2(0, 0);
	vertices[9].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	//First, find the point on the line between points 0 and 2
	distance.x = (vertices[4].position.x - vertices[0].position.x);
	distance.y = (vertices[4].position.y - vertices[0].position.y);
	distance.z = (vertices[4].position.z - vertices[0].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[0].position.x + distance.x * (m / 2));
	intPoint.y = ((float)vertices[0].position.y + distance.y * (m / 2));
	intPoint.z = ((float)vertices[0].position.z + distance.z * (m / 2));

	//Now, repeat the process between the new point and v0 + v1.z
	distance.x = (vertices[1].position.x - intPoint.x);
	distance.y = (vertices[1].position.y - intPoint.y);
	distance.z = (vertices[1].position.z - intPoint.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	newPoint.x = ((float)intPoint.x + distance.x * (set->getHGAmount() / DEBUG_SCALE_FACTOR));
	newPoint.y = ((float)intPoint.y + distance.y * (set->getHGAmount() / DEBUG_SCALE_FACTOR));
	newPoint.z = ((float)intPoint.z + distance.z * (set->getHGAmount() / DEBUG_SCALE_FACTOR));

	vertices[8].position = newPoint;  //Interpolated Point, Right
	vertices[8].position.z = vertices[7].position.z;
	vertices[8].texture = XMFLOAT2(0, 0);
	vertices[8].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);


	vertices[10].position = newPoint;  //Interpolated Point, Left
	vertices[10].position.x = -vertices[10].position.x;
	vertices[10].position.z = vertices[7].position.z;
	vertices[10].texture = XMFLOAT2(0, 0);
	vertices[10].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);


	//This is the loop point

	//Now, interpolate between points 7 and 8 to form new points
	distance.x = (vertices[8].position.x - vertices[7].position.x);
	distance.y = (vertices[8].position.y - vertices[7].position.y);
	distance.z = (vertices[8].position.z - vertices[7].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[7].position.x + distance.x * (m / 2));
	intPoint.y = ((float)vertices[7].position.y + distance.y * (m / 2));
	intPoint.z = ((float)vertices[7].position.z + distance.z * (m / 2));

	vertices[11].position = intPoint;
	vertices[11].texture = XMFLOAT2(0, 0);
	vertices[11].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);//*/

	intPoint.x = ((float)vertices[7].position.x + distance.x * (m / 4));
	intPoint.y = ((float)vertices[7].position.y + distance.y * (m / 4));
	intPoint.z = ((float)vertices[7].position.z + distance.z * (m / 4));

	vertices[12].position = intPoint;
	vertices[12].texture = XMFLOAT2(0, 0);
	vertices[12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	intPoint.x = ((float)vertices[7].position.x + distance.x * (3 * m / 4));
	intPoint.y = ((float)vertices[7].position.y + distance.y * (3 * m / 4));
	intPoint.z = ((float)vertices[7].position.z + distance.z * (3 * m / 4));

	vertices[13].position = intPoint;
	vertices[13].texture = XMFLOAT2(0, 0);
	vertices[13].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	//Now we interpolate between points 11/12/13 and point 6
	distance.x = (vertices[11].position.x - vertices[6].position.x);
	distance.y = (vertices[11].position.y - vertices[6].position.y);
	distance.z = (vertices[11].position.z - vertices[6].position.z);
	
	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[11].position.x - distance.x * (m / 2));
	intPoint.y = ((float)vertices[11].position.y - distance.y * (m / 2));
	intPoint.z = ((float)vertices[11].position.z - distance.z * (m / 2));

	vertices[14].position = intPoint;
	vertices[14].texture = XMFLOAT2(0, 0);
	vertices[14].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);

	vertices[17].position = intPoint;
	vertices[17].position.y = -vertices[17].position.y;
	vertices[17].texture = XMFLOAT2(0, 0);
	vertices[17].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[20].position = intPoint;
	vertices[20].position.y = -vertices[20].position.y;
	vertices[20].position.x = -vertices[20].position.x;
	vertices[20].texture = XMFLOAT2(0, 0);
	vertices[20].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);

	vertices[23].position = intPoint;
	vertices[23].position.x = -vertices[23].position.x;
	vertices[23].texture = XMFLOAT2(0, 0);
	vertices[23].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);

	///

	distance.x = (vertices[12].position.x - vertices[6].position.x);
	distance.y = (vertices[12].position.y - vertices[6].position.y);
	distance.z = (vertices[12].position.z - vertices[6].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[12].position.x - distance.x * (m / 4));
	intPoint.y = ((float)vertices[12].position.y - distance.y * (m / 4));
	intPoint.z = ((float)vertices[12].position.z - distance.z * (m / 4));

	vertices[15].position = intPoint;
	vertices[15].texture = XMFLOAT2(0, 0);
	vertices[15].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);

	vertices[18].position = intPoint;
	vertices[18].position.y = -vertices[18].position.y;
	vertices[18].texture = XMFLOAT2(0, 0);
	vertices[18].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[21].position = intPoint;
	vertices[21].position.y = -vertices[21].position.y;
	vertices[21].position.x = -vertices[21].position.x;
	vertices[21].texture = XMFLOAT2(0, 0);
	vertices[21].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);

	vertices[24].position = intPoint;
	vertices[24].position.x = -vertices[24].position.x;
	vertices[24].texture = XMFLOAT2(0, 0);
	vertices[24].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);

	///

	distance.x = (vertices[13].position.x - vertices[6].position.x);
	distance.y = (vertices[13].position.y - vertices[6].position.y);
	distance.z = (vertices[13].position.z - vertices[6].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[13].position.x - distance.x * (m / 4));
	intPoint.y = ((float)vertices[13].position.y - distance.y * (m / 4));
	intPoint.z = ((float)vertices[13].position.z - distance.z * (m / 4));

	vertices[16].position = intPoint;
	vertices[16].texture = XMFLOAT2(0, 0);
	vertices[16].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);

	vertices[19].position = intPoint;
	vertices[19].position.y = -vertices[19].position.y;
	vertices[19].texture = XMFLOAT2(0, 0);
	vertices[19].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[22].position = intPoint;
	vertices[22].position.y = -vertices[22].position.y;
	vertices[22].position.x = -vertices[22].position.x;
	vertices[22].texture = XMFLOAT2(0, 0);
	vertices[22].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);

	vertices[25].position = intPoint;
	vertices[25].position.x = -vertices[25].position.x;
	vertices[25].texture = XMFLOAT2(0, 0);
	vertices[25].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);



	indices[6] = 0;
	indices[7] = 7;
	indices[8] = 15;

	indices[9] = 0;
	indices[10] = 15;
	indices[11] = 7;

	indices[12] = 0;
	indices[13] = 14;
	indices[14] = 15;

	indices[15] = 0;
	indices[16] = 15;
	indices[17] = 14;

	indices[18] = 0;
	indices[19] = 14;
	indices[20] = 16;

	indices[21] = 0;
	indices[22] = 16;
	indices[23] = 14;

	indices[24] = 0;
	indices[25] = 8;
	indices[26] = 16;

	indices[27] = 0;
	indices[28] = 16;
	indices[29] = 8;




	indices[30] = 0;
	indices[31] = 9;
	indices[32] = 18;

	indices[33] = 0;
	indices[34] = 18;
	indices[35] = 9;

	indices[36] = 0;
	indices[37] = 17;
	indices[38] = 18;

	indices[39] = 0;
	indices[40] = 18;
	indices[41] = 17;

	indices[42] = 0;
	indices[43] = 17;
	indices[44] = 19;

	indices[45] = 0;
	indices[46] = 19;
	indices[47] = 17;

	indices[48] = 0;
	indices[49] = 8;
	indices[50] = 19;

	indices[51] = 0;
	indices[52] = 19;
	indices[53] = 8;



	indices[54] = 0;
	indices[55] = 9;
	indices[56] = 21;

	indices[57] = 0;
	indices[58] = 21;
	indices[59] = 9;

	indices[60] = 0;
	indices[61] = 20;
	indices[62] = 21;

	indices[63] = 0;
	indices[64] = 21;
	indices[65] = 20;

	indices[66] = 0;
	indices[67] = 20;
	indices[68] = 22;

	indices[69] = 0;
	indices[70] = 22;
	indices[71] = 20;

	indices[72] = 0;
	indices[73] = 10;
	indices[74] = 22;

	indices[75] = 0;
	indices[76] = 22;
	indices[77] = 10;



	indices[78] = 0;
	indices[79] = 7;
	indices[80] = 24;

	indices[81] = 0;
	indices[82] = 24;
	indices[83] = 7;

	indices[84] = 0;
	indices[85] = 23;
	indices[86] = 24;

	indices[87] = 0;
	indices[88] = 24;
	indices[89] = 23;

	indices[90] = 0;
	indices[91] = 23;
	indices[92] = 25;

	indices[93] = 0;
	indices[94] = 25;
	indices[95] = 23;

	indices[96] = 0;
	indices[97] = 10;
	indices[98] = 25;

	indices[99] = 0;
	indices[100] = 25;
	indices[101] = 10;

	//This is the loop end
	
	//Now interpolate the midsection
	distance.x = (vertices[4].position.x - vertices[2].position.x);
	distance.y = (vertices[4].position.y - vertices[2].position.y);
	distance.z = (vertices[4].position.z - vertices[2].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[2].position.x + distance.x * (m / 2));
	intPoint.y = ((float)vertices[2].position.y + distance.y * (m / 2));
	intPoint.z = ((float)vertices[2].position.z + distance.z * (m / 2));

	vertices[26].position = intPoint;
	vertices[26].texture = XMFLOAT2(0, 0);
	vertices[26].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);//*/

	intPoint.x = ((float)vertices[2].position.x + distance.x * (m / 4));
	intPoint.y = ((float)vertices[2].position.y + distance.y * (m / 4));
	intPoint.z = ((float)vertices[2].position.z + distance.z * (m / 4));

	vertices[27].position = intPoint;
	vertices[27].texture = XMFLOAT2(0, 0);
	vertices[27].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	intPoint.x = ((float)vertices[2].position.x + distance.x * (3 * m / 4));
	intPoint.y = ((float)vertices[2].position.y + distance.y * (3 * m / 4));
	intPoint.z = ((float)vertices[2].position.z + distance.z * (3 * m / 4));

	vertices[28].position = intPoint;
	vertices[28].texture = XMFLOAT2(0, 0);
	vertices[28].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);





	distance.x = (vertices[26].position.x - vertices[1].position.x);
	distance.y = (vertices[26].position.y - vertices[1].position.y);
	distance.z = (vertices[26].position.z - vertices[1].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[26].position.x - distance.x * (m / 2));
	intPoint.y = ((float)vertices[26].position.y - distance.y * (m / 2));
	intPoint.z = ((float)vertices[26].position.z - distance.z * (m / 2));

	vertices[29].position = intPoint;
	vertices[29].texture = XMFLOAT2(0, 0);
	vertices[29].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);

	vertices[32].position = intPoint;
	vertices[32].position.y = -vertices[32].position.y;
	vertices[32].texture = XMFLOAT2(0, 0);
	vertices[32].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[35].position = intPoint;
	vertices[35].position.y = -vertices[35].position.y;
	vertices[35].position.x = -vertices[35].position.x;
	vertices[35].texture = XMFLOAT2(0, 0);
	vertices[35].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);

	vertices[38].position = intPoint;
	vertices[38].position.x = -vertices[38].position.x;
	vertices[38].texture = XMFLOAT2(0, 0);
	vertices[38].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);




	distance.x = (vertices[27].position.x - vertices[1].position.x);
	distance.y = (vertices[27].position.y - vertices[1].position.y);
	distance.z = (vertices[27].position.z - vertices[1].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[27].position.x - distance.x * (m / 4));
	intPoint.y = ((float)vertices[27].position.y - distance.y * (m / 4));
	intPoint.z = ((float)vertices[27].position.z - distance.z * (m / 4));

	vertices[30].position = intPoint;
	vertices[30].texture = XMFLOAT2(0, 0);
	vertices[30].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);

	vertices[33].position = intPoint;
	vertices[33].position.y = -vertices[33].position.y;
	vertices[33].texture = XMFLOAT2(0, 0);
	vertices[33].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[36].position = intPoint;
	vertices[36].position.y = -vertices[36].position.y;
	vertices[36].position.x = -vertices[36].position.x;
	vertices[36].texture = XMFLOAT2(0, 0);
	vertices[36].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);

	vertices[39].position = intPoint;
	vertices[39].position.x = -vertices[39].position.x;
	vertices[39].texture = XMFLOAT2(0, 0);
	vertices[39].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);





	distance.x = (vertices[28].position.x - vertices[1].position.x);
	distance.y = (vertices[28].position.y - vertices[1].position.y);
	distance.z = (vertices[28].position.z - vertices[1].position.z);

	m = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z + distance.z);

	distance.x /= m;
	distance.y /= m;
	distance.z /= m;

	intPoint.x = ((float)vertices[28].position.x - distance.x * (m / 4));
	intPoint.y = ((float)vertices[28].position.y - distance.y * (m / 4));
	intPoint.z = ((float)vertices[28].position.z - distance.z * (m / 4));

	vertices[31].position = intPoint;
	vertices[31].texture = XMFLOAT2(0, 0);
	vertices[31].normal = XMFLOAT3(1.0f, 1.0f, 0.0f);

	vertices[34].position = intPoint;
	vertices[34].position.y = -vertices[34].position.y;
	vertices[34].texture = XMFLOAT2(0, 0);
	vertices[34].normal = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[37].position = intPoint;
	vertices[37].position.y = -vertices[37].position.y;
	vertices[37].position.x = -vertices[37].position.x;
	vertices[37].texture = XMFLOAT2(0, 0);
	vertices[37].normal = XMFLOAT3(-1.0f, -1.0f, 0.0f);

	vertices[40].position = intPoint;
	vertices[40].position.x = -vertices[40].position.x;
	vertices[40].texture = XMFLOAT2(0, 0);
	vertices[40].normal = XMFLOAT3(-1.0f, 1.0f, 0.0f);



	indices[102] = 1;
	indices[103] = 2;
	indices[104] = 30;

	indices[105] = 1;
	indices[106] = 30;
	indices[107] = 2;

	indices[108] = 1;
	indices[109] = 30;
	indices[110] = 29;

	indices[111] = 1;
	indices[112] = 29;
	indices[113] = 30;

	indices[114] = 1;
	indices[115] = 29;
	indices[116] = 31;

	indices[117] = 1;
	indices[118] = 31;
	indices[119] = 29;

	indices[120] = 1;
	indices[121] = 31;
	indices[122] = 4;

	indices[123] = 1;
	indices[124] = 4;
	indices[125] = 31;

	int iCounter = 126;

	indices[iCounter] = 2;
	iCounter++;
	indices[iCounter] = 7;
	iCounter++;
	indices[iCounter] = 15;
	iCounter++;

	indices[iCounter] = 2;
	iCounter++;
	indices[iCounter] = 15;
	iCounter++;
	indices[iCounter] = 7;
	iCounter++;

	indices[iCounter] = 2;
	iCounter++;
	indices[iCounter] = 30;
	iCounter++;
	indices[iCounter] = 15;
	iCounter++;

	indices[iCounter] = 2;
	iCounter++;
	indices[iCounter] = 15;
	iCounter++;
	indices[iCounter] = 30;
	iCounter++;



	indices[iCounter] = 15;
	iCounter++;
	indices[iCounter] = 30;
	iCounter++;
	indices[iCounter] = 14;
	iCounter++;

	indices[iCounter] = 15;
	iCounter++;
	indices[iCounter] = 14;
	iCounter++;
	indices[iCounter] = 30;
	iCounter++;

	indices[iCounter] = 30;
	iCounter++;
	indices[iCounter] = 29;
	iCounter++;
	indices[iCounter] = 14;
	iCounter++;

	indices[iCounter] = 30;
	iCounter++;
	indices[iCounter] = 14;
	iCounter++;
	indices[iCounter] = 29;
	iCounter++;



	indices[iCounter] = 29;
	iCounter++;
	indices[iCounter] = 16;
	iCounter++;
	indices[iCounter] = 14;
	iCounter++;

	indices[iCounter] = 29;
	iCounter++;
	indices[iCounter] = 14;
	iCounter++;
	indices[iCounter] = 16;
	iCounter++;

	indices[iCounter] = 16;
	iCounter++;
	indices[iCounter] = 29;
	iCounter++;
	indices[iCounter] = 31;
	iCounter++;

	indices[iCounter] = 16;
	iCounter++;
	indices[iCounter] = 31;
	iCounter++;
	indices[iCounter] = 29;
	iCounter++;




	indices[iCounter] = 31;
	iCounter++;
	indices[iCounter] = 16;
	iCounter++;
	indices[iCounter] = 8;
	iCounter++;

	indices[iCounter] = 31;
	iCounter++;
	indices[iCounter] = 8;
	iCounter++;
	indices[iCounter] = 16;
	iCounter++;

	indices[iCounter] = 8;
	iCounter++;
	indices[iCounter] = 31;
	iCounter++;
	indices[iCounter] = 4;
	iCounter++;

	indices[iCounter] = 8;
	iCounter++;
	indices[iCounter] = 4;
	iCounter++;
	indices[iCounter] = 31;
	iCounter++;

	int a, b, c, d, e, f, g, h, i, j;
	a = 8;
	b = 19;
	c = 17;
	d = 18;
	e = 9;
	f = 4;
	g = 34;
	h = 32;
	i = 33;
	j = 3;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;


	a = 10;
	b = 22;
	c = 20;
	d = 21;
	
	f = 5;
	g = 37;
	h = 35;
	i = 36;
	

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	
	b = 25;
	c = 23;
	d = 24;
	e = 7;
	
	g = 40;
	h = 38;
	i = 39;
	j = 2;


	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;



	//Now onto the tip of the speartip
	//Kinda cheating here by essentially having the top be the bottom, just stretched
	//It'll be dandy when its done


	float newZ = vertices[1].position.z + (((set->getBLength() / DEBUG_SCALE_FACTOR) * 2) / 3);

	//Need new versions of points 7 through 10 and 14 through 25

	int vCount = 41;

	for (int i = 0; i < 4; i++)
	{
		vertices[vCount].position = vertices[7 + i].position;
		vertices[vCount].position.z = -vertices[vCount].position.z + newZ;
		vertices[vCount].texture = XMFLOAT2(0, 0);
		vertices[vCount].normal = vertices[7 + i].position;

		vCount++;
	}
	//Meaning 41 = top, 42 = right, 43 = bottom and 44 = left

	for (int i = 0; i < 12; i++)
	{
		vertices[vCount].position = vertices[14 + i].position;
		vertices[vCount].position.z = -vertices[vCount].position.z + newZ;
		vertices[vCount].texture = XMFLOAT2(0, 0);
		vertices[vCount].normal = vertices[14 + i].normal;

		vCount++;
	}

	//45-47 are top right, 48-50 are bottom right, 51-53 are bottom left and 54-56 are top left
	a = 4;
	b = 31;
	c = 29;
	d = 30;
	e = 2;
	f = 42;
	g = 47;
	h = 45;
	i = 46;
	j = 41;


	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;



	b = 34;
	c = 32;
	d = 33;
	e = 3;
	
	g = 50;
	h = 48;
	i = 49;
	j = 43;


	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	a = 5;
	b = 37;
	c = 35;
	d = 36;
	
	f = 44;
	g = 53;
	h = 51;
	i = 52;


	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	
	b = 40;
	c = 38;
	d = 39;
	e = 2;
	
	g = 56;
	h = 54;
	i = 55;
	j = 41;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = j;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = j;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;

	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = i;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = h;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = g;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	indices[iCounter] = g;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = a;
	iCounter++;

	//Final point
	vertices[57].position = XMFLOAT3(0, 0, (vertices[56].position.z + (newZ / 3)));
	vertices[57].texture = XMFLOAT2(0, 0);
	vertices[57].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);


	a = 57;

	b = 41;
	c = 47;
	d = 45;
	e = 46;
	f = 42;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
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
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	b = 43;
	c = 50;
	d = 48;
	e = 49;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
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
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;


	c = 53;
	d = 51;
	e = 52;
	f = 44;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
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
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;

	b = 41;
	c = 56;
	d = 54;
	e = 55;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;
	indices[iCounter] = b;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = d;
	iCounter++;
	indices[iCounter] = c;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = c;
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
	indices[iCounter] = d;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = f;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;

	indices[iCounter] = a;
	iCounter++;
	indices[iCounter] = e;
	iCounter++;
	indices[iCounter] = f;
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
