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

#pragma region SimpleTypes
struct ModelData
{
    size_t mPointCount = 0;
    vector<vec3> mVertices;
    vector<vec3> mNormals;
    vector<vec2> mTextureCoords;
};
#pragma endregion SimpleTypes

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

