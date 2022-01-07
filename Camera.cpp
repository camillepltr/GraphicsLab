/*******************************************************************************************
	Computer Graphics - TCD
	Camille Peltier (peltierc@tcd.ie)
	2021
	-------------------
	Camera.cpp
	Class to set, store and update camera properties
********************************************************************************************/
#include "Camera.h"

// Constructors
Camera::Camera() {}

Camera::Camera(int w, int h, float init_x, float init_y, int t) {
	int width = w;
	int height = h;
	float last_x = init_x;
	float last_y = init_y;
	int tol = t;
}

// Other public methods
void Camera::UpdateCamera(bool key_states[], float delta) {
	float speed = 30.0f * delta;
	// Note: the choice of the keys makes sense on an AZERTY keyboard
	if (key_states['w']) { // Dolly in (move forward)
		camera_pos += speed * camera_front;
	}
	if (key_states['x']) { // Dolly out (move backwards)
		camera_pos -= speed * camera_front;
	}
	if (key_states['c']) { // Track rigth (move right)
		camera_pos += speed * normalize(cross(camera_front, camera_up));
	}
	if (key_states['v']) { // Track left (move left)
		camera_pos -= speed * normalize(cross(camera_front, camera_up));
	}
	if (key_states['b']) { // Slide up
		camera_pos += speed * camera_up;
	}
	if (key_states['n']) { // Slide down
		camera_pos -= speed * camera_up;
	}
}

void Camera::MouseMove(int x, int y)
{
		if (x < tol || x > width - tol || y < tol || y > height - tol) {
			glutWarpPointer(width / 2, height / 2);
		}
		else {
			float xoffset = x - last_x;
			float yoffset = last_y - y; // because y coords go from botton to up
			glutWarpPointer(last_x, last_y);

			float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw_angle += xoffset;
			pitch_angle += yoffset;
			if (pitch_angle < -89.0f) {
				pitch_angle = -89.0f;
			}
			else if (pitch_angle > 89.0f) {
				pitch_angle = 89.0f;
			}
			camera_front = computeCameraFront();
		}
}

vec3 Camera::GetPosition() {
	return camera_pos;
}

vec3 Camera::GetFront() {
	return camera_front;
}

vec3 Camera::GetUp() {
	return camera_up;
}

// Private methods
vec3 Camera::computeCameraFront() {
	vec3 front;
	// Basic trigonometry
	front.x = cos(radians(yaw_angle)) * cos(radians(pitch_angle));
	front.y = sin(radians(pitch_angle));
	front.z = sin(radians(yaw_angle)) * cos(radians(pitch_angle));
	return normalize(front);
}