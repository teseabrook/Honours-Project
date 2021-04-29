#include "CrossguardMeshGenerator.h"


//Save the arguments and begin generation
CrossguardMeshGenerator::CrossguardMeshGenerator(ParameterSet* a_set, ID3D11Device* a_device, ID3D11DeviceContext* a_deviceContext)
{
	set = a_set;
	device = a_device;
	deviceContext = a_deviceContext;
	generateCrossguard();
}


CrossguardMeshGenerator::~CrossguardMeshGenerator()
{
}

//Go through the different shapes and get the correct one
void CrossguardMeshGenerator::generateCrossguard()
{
	int shape = set->getCShape();
	switch (shape)
	{
	case 0:
		//Simple Bar
		
			pommelMesh = new BarMesh(device, deviceContext);
		
		break;

	case 1:
		//Placeholder
		pommelMesh = new SphereMesh(device, deviceContext);
		break;

	case 2:
		//Disk Mesh
		if (set->getCStyle() == 0 || set->getCStyle() == 2)
		{
			pommelMesh = new DiskMesh(device, deviceContext, set, 0);
		}
		else
		{
			//pommelMesh = new EmbossedDiskMesh(device, deviceContext, set, false);
			//Embossed disks shelved for if I have time later

			pommelMesh = new DiskMesh(device, deviceContext, set, 0);
		}
		break;


	default:
		break;
	}

}
