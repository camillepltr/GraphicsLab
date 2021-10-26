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

