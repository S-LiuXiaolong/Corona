#include "crpch.h"
#include "Geometry.h"

namespace Corona {

	Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		setupMesh();
	}

	void Mesh::setupMesh()
	{

	}

}