/*******************************************************************************************
    Computer Graphics - TCD
    Camille Peltier (peltierc@tcd.ie)
    2021
    -------------------
    Skybox.cpp
    Class to load and store data for skybox
********************************************************************************************/

#include "Skybox.h"

// Constructors 
Skybox::Skybox() {}

Skybox::Skybox(vector<const char*> faces_paths, GLuint shaderProgramID, int size) {
    size = size;
    for (int i = 0; i < 108; i++) {
        skybox_vertices[i] *= size;
    }
    faces = faces_paths;
    generateObjectBuffer(shaderProgramID);
    loadCubemap();
}

// Other public methods
GLuint Skybox::GetSkyboxTexture() {
    return skybox_texture;
}

GLuint Skybox::GetSkyboxVAO() {
    return vao;
}

// Private methods
void Skybox::generateObjectBuffer(GLuint shaderProgramID) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &skybox_vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}

bool Skybox::loadCubemapFace(GLuint texture, GLenum side_target, const char* file_name) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file_name, &width, &height, &nrChannels, 4);
    if (!data) {
        cout << "Failed to load cubemap texture: " << file_name << endl;
        stbi_image_free(data);
        return false;
    }

    glTexImage2D(side_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return true;
}

void Skybox::loadCubemap() {
    glEnable(GL_TEXTURE_CUBE_MAP); 
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);

    // generate a cube-map texture to hold all the sides
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &skybox_texture);

    // load each image into a side of the cube-map texture
    loadCubemapFace(skybox_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, faces[0]);
    loadCubemapFace(skybox_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, faces[1]);
    loadCubemapFace(skybox_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, faces[2]);
    loadCubemapFace(skybox_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, faces[3]);
    loadCubemapFace(skybox_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, faces[4]);
    loadCubemapFace(skybox_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X, faces[5]);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
