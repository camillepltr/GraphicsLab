#pragma once
#ifndef SEA_H
#define SEA_H

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
#include "Model.h"

using namespace glm;
using namespace std;

const int SIDE_SIZE = 1000;

class Sea: public Model
{
public:
	Sea();
	Sea(GLuint shaderProgramID, const char* texture_file_name);
	void RiseSeaLevel(float delta);

private:
	float sea_level;

	void generateMesh();
};
#endif
