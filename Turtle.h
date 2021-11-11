#pragma once
#ifndef TURTLE_H
#define TURTLE_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Model.h"
#include "Shader.h"
#include "Struct.h"

using namespace glm;

#define TURTLE_SHELL_MESH_NAME "../Meshes/turtle_shell.dae"
#define TURTLE_LEFT_ARM_MESH_NAME "../Meshes/turtle_left_arm.dae"
#define TURTLE_RIGHT_ARM_MESH_NAME "../Meshes/turtle_right_arm.dae"
#define TURTLE_LEFT_LEG_MESH_NAME "../Meshes/turtle_left_leg.dae"
#define TURTLE_RIGHT_LEG_MESH_NAME "../Meshes/turtle_right_leg.dae"

class Turtle
{
public:
	// Public attributes
	Model shell = Model();
	Model la = Model(); // Left arm
	Model ra = Model(); // Right arm
	Model ll = Model(); // Left leg
	Model rl = Model(); // Right leg
	vec3 shell_colour = vec3(0.5, 0.5, 0.0);
	vec3 body_colour = vec3(0.4, 0.2, 0.0);

	// Constructors
	Turtle(); 
	Turtle(Shader shader);
	Turtle(const Turtle &t);

	// Public methods
	void Draw(Shader turtle_shader, mat4 ground_model);
};

#endif

