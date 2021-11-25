/*******************************************************************************************
	Computer Graphics - TCD
	Camille Peltier (peltierc@tcd.ie)
	2021
	-------------------
	Camera.h
	Class to set, store and update camera properties
********************************************************************************************/
#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Camera
{
public:
	// Contructors
	Camera();
	Camera(int w, int h, float init_x, float init_y, int t);

	// Public methods
	void UpdateCamera(bool key_states[], float delta);
	void MouseMove(int x, int y);
	vec3 GetPosition();
	vec3 GetFront();
	vec3 GetUp();

private:
	// Attributes 
	// Default vuew transformation variables
	vec3 camera_pos = vec3(0.0f, 20.0f, 50.0f);
	vec3 camera_front = vec3(0.0f, 0.0f, -1.0f);
	vec3 camera_up = vec3(0.0f, 1.0f, 0.0f);

	float yaw_angle = -90.0f;                           //écart
	float pitch_angle = 0.0f;                           //pas

	int width = 800.0;
	int height = 600.0;
	float last_x = width / 2;
	float last_y = height / 2;
	int tol = 100;

	// Private methods
	vec3 computeCameraFront();
};

#endif