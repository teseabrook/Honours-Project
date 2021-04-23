#include "PommelMeshGenerator.h"



PommelMeshGenerator::PommelMeshGenerator(ParameterSet* a_set, ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext)
{
	set = a_set;
	device = a_device;
	deviceContext = a_deviceContext;
	generatePommel();
}


PommelMeshGenerator::~PommelMeshGenerator()
{
}

void PommelMeshGenerator::generatePommel()
{
	int style = set->getPStyle();
	switch (style)
	{
		case 1:
			pommelMesh = new SphereMesh(device, deviceContext);
			break;

		case 2:
			pommelMesh = new DiskMesh(device, deviceContext, set);
			break;

		case 3:
			pommelMesh = new ClawMesh(device, deviceContext, set);
			break;

		case 5:
			pommelMesh = new ClawMesh(device, deviceContext, set);
			break;

		default:
			break;
	}

	

	if (style == 5)
	{
		gemMesh = new GemMesh(device, deviceContext, set);
	}
}


