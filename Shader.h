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
    // Public methods
    void Use();
    GLuint GetID();
    GLint GetUniformLocation(string uniformName) const;
    void SetUniformMat4(string uniformName, mat4 value) const;
    void SetUniformVec3(string uniformName, vec3 value) const;
    void SetUniformVec4(string uniformName, vec4 value) const;
    void SetUniformFloat(string uniformName, float value) const;
    void SetLight(string uniformName, Light l) const;
    void SetMaterial(Material m) const;

    // Constructors
    Shader(); // Default constructor
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

private:
    // Private attributes
    GLuint shaderProgramID = 0;

    // Private methods
    static char* readShaderFile(const char* path);
    static void addShader(GLuint ShaderProgram, const char* shaderText, GLenum ShaderType);
    GLuint compileShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
};

#endif

