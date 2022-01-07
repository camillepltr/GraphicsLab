/*******************************************************************************************
    Computer Graphics - TCD
    Camille Peltier (peltierc@tcd.ie)
    2021
    -------------------
    Shader.h
    This class represent a shader program, which has a vertex shader and a fragment shader
********************************************************************************************/
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Struct.h"

using namespace std;
using namespace glm;

class Shader
{
public:
    // Constructors
    Shader();
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    // Other public methods
    void Use();
    GLuint GetID();
    GLint GetUniformLocation(string uniformName) const;
    void SetUniformMat4(string uniformName, mat4 value) const;
    void SetUniformVec3(string uniformName, vec3 value) const;
    void SetUniformVec4(string uniformName, vec4 value) const;
    void SetUniformFloat(string uniformName, float value) const;
    void SetUniformInt(string uniformName, int value) const;
    void SetLight(string uniformName, Light l) const;
    void SetMaterial(Material m) const;

private:
    // Private attributes
    GLuint shaderProgramID = 0;

    // Private methods
    static char* readShaderFile(const char* path); // Read and return the content of the file 
    static void addShader(GLuint ShaderProgram, const char* shaderText, GLenum ShaderType); // Compile a shader and attach it to the shader program
    GLuint compileShaders(const char* vertexShaderPath, const char* fragmentShaderPath); // Compile vertex and frament shaders and link the shader program
};

#endif

