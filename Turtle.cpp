#include "Turtle.h"

Turtle::Turtle() {}

Turtle::Turtle(Shader shader) {
	// Create models for the 5 body parts
	shell = Model(TURTLE_SHELL_MESH_NAME, shader.GetID(), "../Textures/shell_texture.jpg");
	la = Model(TURTLE_LEFT_ARM_MESH_NAME, shader.GetID(), vec3(3.8, 0.0, 3.4), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), "../Textures/skin_texture.jpg");
	ra = Model(TURTLE_RIGHT_ARM_MESH_NAME, shader.GetID(), vec3(-3.8, 0.0, 3.4), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), "../Textures/skin_texture.jpg");
	ll = Model(TURTLE_LEFT_LEG_MESH_NAME, shader.GetID(), vec3(2.8, 0.0, -6.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), "../Textures/skin_texture.jpg");
	rl = Model(TURTLE_RIGHT_LEG_MESH_NAME, shader.GetID(), vec3(-2.8, 0.0, -6.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), "../Textures/skin_texture.jpg");

	//Initialize shell and body colour
	shell_colour = vec3(0.9, 0.5 , 0.0);
	body_colour = vec3(0.9, 0.9, 0.0);
}