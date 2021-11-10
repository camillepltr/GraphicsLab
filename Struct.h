#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Light {
	glm::vec3 ambient_colour = glm::vec3(0.5, 0.5, 0.5);
	glm::vec3 diffuse_colour = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 specular_colour = glm::vec3(0.5, 0.5, 0.5);
	glm::vec3 position = glm::vec3(10.0, 10.0, 4.0);
};


struct Material {
	float Ka = 0.4;
	float Kd = 1.0;
	float Ks = 1.8;
	float phong_exponent = 80.0; // shininess
};
/*

struct Turtle {
	Model shell;
	Model la; // Left arm
	Model ra; // Right arm
	Model ll; // Left leg
	Model rl; // Right leg
	glm::vec3 shell_colour;
	glm::vec3 body_colour;
};
*/