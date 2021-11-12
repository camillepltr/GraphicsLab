/*******************************************************************************************
    Computer Graphics - TCD
    Camille Peltier (peltierc@tcd.ie)
    2021
    -------------------
    Crowd.h
    Crowd of Turtle object
    Movement of turtles (boids) using Reynolds' 3 rules
********************************************************************************************/
#pragma once
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>

#include "Shader.h"
#include "Model.h"
#include "Turtle.h"

using namespace glm;

Turtle** create_turtle_crowd(Shader shader, int size);

// Boids rules
vec3 rule1(Turtle* t, Turtle** crowd, int n); // Seperation
vec3 rule2(Turtle* t, Turtle** crowd, int n); // Alignment
vec3 rule3(Turtle* t, Turtle** crowd, int n); // Cohesion


