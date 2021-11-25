#include "Sea.h"

Sea::Sea() {}
Sea::Sea(GLuint shaderProgramID, const char* texture_file_name) {
	generateMesh();
	generateObjectBufferMesh(shaderProgramID, texture_file_name);
	cout << mesh_data.mVertices[400].x << endl;
}

void Sea::RiseSeaLevel(float delta) {
	sea_level += delta;
}

void Sea::generateMesh() {
	for (int x = -SIDE_SIZE /2; x < SIDE_SIZE /2; x++)
	{
		for (int z = -SIDE_SIZE /2; z < SIDE_SIZE /2; z++)
		{
			mesh_data.mVertices.push_back(vec3(x, 20.0, z));
			mesh_data.mNormals.push_back(vec3(0.0, 1.0, 0.0));
			mesh_data.mTextureCoords.push_back(vec2(x, z));
			mesh_data.mPointCount++;
		}
	}
}