/*******************************************************************************************
    Computer Graphics - TCD
    Camille Peltier (peltierc@tcd.ie)
    2021
    -------------------
    Model.h
    Class to handle meshes
********************************************************************************************/
#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Shader.h"

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// To load texture image
#include <../stb/stb_image.h>

using namespace glm;

struct ModelData
{
    size_t mPointCount = 0;
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec2> mTextureCoords;
};

class Model
{
public:
    // Public attributes 
    vec3 translation_vec = vec3(0.0f, 0.0f, 0.0f);
    vec3 rotation_vec = vec3(0.0f, 0.0f, 0.0f);
    vec3 scale_vec = vec3(1.0f, 1.0f, 1.0f);

    // Constructors
    Model();
    Model(const char* file_name, GLuint shaderProgramID);
    Model(const char* file_name, GLuint shaderProgramID, vec3 trans, vec3 rot, vec3 scale);

    // Public methods
    mat4 GetModelLocalTransformationMatrix();
    ModelData GetMeshData();
    GLuint GetVao();

private:
    // Private attributes
    ModelData mesh_data;
    GLuint vao;

    // Private methods
    void generateObjectBufferMesh(GLuint shaderProgramID);
    void loadModel(const char* file_name);
};

#endif

