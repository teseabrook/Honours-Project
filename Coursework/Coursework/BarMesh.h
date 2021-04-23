/*
 * BarMesh
 * A simple mesh to create a bar
 * Not used currently, will be used for more advanced crossguards
 * For now it's a clone of CubeMesh
 */

#include "BaseMesh.h"

using namespace DirectX;

class BarMesh : public BaseMesh
{

public:
	/** \brief Initialises and builds a cube mesh
	*
	* Can specify resolution of cube, this deteremines how many subdivisions are on each side of the cube.
	* @param device is the renderer device
	* @param device context is the renderer device context
	* @param resolution is a int for subdivision of the cube. Default is 20.
	*/
	BarMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 20);
	~BarMesh();

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
};
