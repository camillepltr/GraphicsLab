/*******************************************************************************************
	Computer Graphics - TCD
	Camille Peltier (peltierc@tcd.ie)
	2021
	-------------------
	Turtle.h
	A turtle = collection of 6 models (shell + five body parts).
	This class also contains methods for crowd animation using boids (a turtle = a boid)
********************************************************************************************/

#pragma once
#ifndef TURTLE_H
#define TURTLE_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp> 
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
#define TURTLE_HEAD_MESH_NAME "../Meshes/turtle_head.dae"
#define TURTLE_LEFT_ARM_MESH_NAME "../Meshes/turtle_left_arm.dae"
#define TURTLE_RIGHT_ARM_MESH_NAME "../Meshes/turtle_right_arm.dae"
#define TURTLE_LEFT_LEG_MESH_NAME "../Meshes/turtle_left_leg.dae"
#define TURTLE_RIGHT_LEG_MESH_NAME "../Meshes/turtle_right_leg.dae"

#define MAX_SPEED 3.0
#define SAFETY_DISTANCE 25
#define VISUAL_RANGE 200
#define PI 3.141592635

class Turtle
{
public:
	// Public attributes
	Model shell = Model();
	Model head = Model(); 
	Model la = Model(); // Left arm
	Model ra = Model(); // Right arm
	Model ll = Model(); // Left leg
	Model rl = Model(); // Right leg

	vec4 shell_colour = vec4(0.5, 0.5, 0.0, 1.0);
	vec4 body_colour = vec4(0.4, 0.2, 0.0, 1.0);

	vec3* position;
	vec3 velocity;

	// Constructors
	Turtle(); 
	Turtle(Shader shader);
	Turtle(const Turtle &t);
	
	// Public methods
	void Draw(Shader turtle_shader, mat4 ground_model);
	void MoveBodyParts(float t);
	void MoveToNextBoidPosition(Turtle** crowd, int crowd_size, Model** obstacles, int nb_obstacles, vec3 target, float delta);

private:
	// Private mathods
	vec3 separation(Turtle** crowd, int n); // Reynold rule 1 : Seperation
	vec3 alignment(Turtle** crowd, int n); // Reynold rule 2 : Alignment
	vec3 cohesion(Turtle** crowd, int n); // Reynold rule 3 : Cohesion
	vec3 seekPlace(vec3 target); // Additional rule : tend toowards a particultar place
	vec3 avoidObstacles(Model** obstacles, int n); // Additional rule : avoid obstacles (basically the same thing as for the Separation rule but by looking at other obejcts
	bool isInVisualRange(Turtle* t);
	void adjustOrientation(vec3 v1, vec3 v2, float delta);
};

#endif


