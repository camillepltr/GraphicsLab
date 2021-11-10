/*******************************************************************************************
	Computer Graphics - TCD
	Camille Peltier (peltierc@tcd.ie)
	2021
	-------------------
	Model.cpp
	Class to handle meshes
********************************************************************************************/
#include "Model.h"

// Constructors
Model::Model() {
}

Model::Model(const char* file_name, GLuint shaderProgramID, const char* texture_file_name) {
	loadModel(file_name);
	generateObjectBufferMesh(shaderProgramID, texture_file_name);
}

Model::Model(const char* file_name, GLuint shaderProgramID, vec3 trans, vec3 rot, vec3 scale, const char* texture_file_name) {
	loadModel(file_name);
	generateObjectBufferMesh(shaderProgramID, texture_file_name);

	translation_vec = trans;
	rotation_vec = rot;
	scale_vec = scale;
}

// Public methods
mat4 Model::GetModelLocalTransformationMatrix() {
	// Compute a transformation matrix M = T*R*S
	mat4 translationMat = translate(mat4(1.0f), translation_vec);
	mat4 rotationMatX = rotate(mat4(1.0f), rotation_vec.x, vec3(1.0f, 0.0f, 0.0f));
	mat4 rotationMatY = rotate(mat4(1.0f), rotation_vec.y, vec3(0.0f, 1.0f, 0.0f));
	mat4 rotationMatZ = rotate(mat4(1.0f), rotation_vec.z, vec3(0.0f, 0.0f, 1.0f));
	mat4 scaleMat = scale(mat4(1.0f), scale_vec);

	return translationMat * rotationMatX * rotationMatY * rotationMatZ * scaleMat;
}

ModelData Model::GetMeshData() {
	return mesh_data;
}

GLuint Model::GetVao() {
	return vao;
}

GLuint Model::GetTexture() {
	return texture;
}

// Private methods 
void Model::loadModel(const char* file_name) {
	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		exit(1);
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		this->mesh_data.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				this->mesh_data.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				this->mesh_data.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				this->mesh_data.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
}

void Model::generateObjectBufferMesh(GLuint shaderProgramID, const char* texture_file_name) {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	GLuint loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	GLuint loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	GLuint loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	// Vertices
	GLuint vp_vbo = 0;
	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);

	// Normals
	GLuint vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	// Texture
	if (texture_file_name != NULL) {
		unsigned int vt_vbo = 0;
		glGenBuffers (1, &vt_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
		glBufferData (GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof (vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		unsigned char* data = stbi_load(texture_file_name, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc3);
		glBindBuffer (GL_ARRAY_BUFFER, texture);
	}
}

