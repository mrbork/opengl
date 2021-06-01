#include "pch.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh( float* vertices , unsigned int numVertices, unsigned int* indices , unsigned int numIndices);
	void RenderMesh();
	void ClearMesh();

private:
	unsigned int vao , vbo , ibo, uIndices;
	
};

