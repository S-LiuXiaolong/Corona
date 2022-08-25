#pragma once
#include "Corona.h"
#include <string>

namespace Corona {

	using Microsoft::WRL::ComPtr;
	using namespace DirectX;
	using namespace DirectX::PackedVector;
	using namespace std;

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 n;
	};

	struct Texture {
		unsigned int id;
		string type;
	};

	class Mesh {

	public:
		// mesh Data
		vector<Vertex>       vertices;
		vector<unsigned int> indices;
		vector<Texture>      textures;

		// constructor
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	private:
		void setupMesh();

	};

}



