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
#include "Struct.h"

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// To load texture image
#include <../stb/stb_image.h>

using namespace glm;

class Model
{
public:
    // Public attributes 
    vec3 translation_vec = vec3(0.0f, 0.0f, 0.0f);
    vec3 rotation_vec = vec3(0.0f, 0.0f, 0.0f);
    vec3 scale_vec = vec3(1.0f, 1.0f, 1.0f);

    // Constructors
    Model();
    Model(const char* file_name, GLuint shaderProgramID, const char* texture_file_name = NULL); // Default initial position/rotation/scale, texture optional
    Model(const char* file_name, GLuint shaderProgramID, vec3 trans, vec3 rot, vec3 scale, const char* texture_file_name);  // With initial position/rotation/scale, texture optional

    // Public methods
    mat4 GetModelLocalTransformationMatrix();
    ModelData GetMeshData();
    GLuint GetVao();
    GLuint GetTexture();

private:
    // Private attributes
    ModelData mesh_data;
    GLuint vao;
    GLuint texture;

    // Private methods
    void generateObjectBufferMesh(GLuint shaderProgramID, const char* texture_file_name = NULL);
    void loadModel(const char* file_name);
};

#endif

