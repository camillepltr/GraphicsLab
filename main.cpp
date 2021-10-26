
//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"


typedef unsigned long DWORD;

using namespace std;
using namespace glm;

#define GROUND_MESH_NAME "../Meshes/ground.dae"
#define TURTLE_BODY_MESH_NAME "../Meshes/turtle_body.dae"
#define TURTLE_LEFT_ARM_MESH_NAME "../Meshes/turtle_left_arm.dae"
#define TURTLE_RIGHT_ARM_MESH_NAME "../Meshes/turtle_right_arm.dae"
#define TURTLE_LEFT_LEG_MESH_NAME "../Meshes/turtle_left_leg.dae"
#define TURTLE_RIGHT_LEG_MESH_NAME "../Meshes/turtle_right_leg.dae"

int width = 800.0;
int height = 600.0;
bool key_states[256];

Shader ground_shader;
Shader turtle_shader;

// Models
Model ground;
Model turtle_body;
Model turtle_la; // Left arm
Model turtle_ra; // Right arm
Model turtle_ll; // Left leg
Model turtle_rl; // Right leg

// View
Camera camera;

// Projection
bool perpective_proj = true;


void updateModels(float delta) {
	// Translations
	if (key_states['e']) {
		turtle_body.translation_vec.z -= 10.0f*delta;
	}
	if (key_states['r']) {
		turtle_body.translation_vec.z += 10.0f * delta;
	}
	if (key_states['t']) {
		turtle_body.translation_vec.x -= 10.0f * delta;
	}
	if (key_states['y']) {
		turtle_body.translation_vec.x += 10.0f * delta;
	}


	// Rotations
	if (key_states['a']) {
		turtle_body.rotation_vec.y -= delta;
	}
	if (key_states['z']) {
		turtle_body.rotation_vec.y += delta;
	}

	if (key_states['q']) {
		if (turtle_la.rotation_vec.y > -0.2) {
			turtle_la.rotation_vec.y -= delta;
		}
	}
	if (key_states['s']) {
		if (turtle_la.rotation_vec.y < 0) {
			turtle_la.rotation_vec.y += delta;
		}
	}

	if (key_states['d']) {
		if (turtle_ra.rotation_vec.y > -0.2) {
			turtle_ra.rotation_vec.y -= delta;
		}
	}
	if (key_states['f']) {
		if (turtle_ra.rotation_vec.y < 0) {
			turtle_ra.rotation_vec.y += delta;
		}
	}

	if (key_states['g']) {
		if (turtle_ll.rotation_vec.y > -0.2) {
			turtle_ll.rotation_vec.y -= delta;
		}
	}
	if (key_states['h']) {
		if (turtle_ll.rotation_vec.y < 0) {
			turtle_ll.rotation_vec.y += delta;
		}
	}

	if (key_states['j']) {
		if (turtle_rl.rotation_vec.y > -0.2) {
			turtle_rl.rotation_vec.y -= delta;
		}
	}
	if (key_states['k']) {
		if (turtle_rl.rotation_vec.y < 0) {
			turtle_rl.rotation_vec.y += delta;
		}
	}

	// Uniform scaling
	if (key_states['1']) {
		turtle_body.scale_vec -= vec3(delta, delta, delta);
	}
	if (key_states['2']) {
		turtle_body.scale_vec += vec3(delta, delta, delta);
	}

	// Non uniform scaling
	if (key_states['3']) {
		turtle_body.scale_vec.x -= delta;
	}
	if (key_states['4']) {
		turtle_body.scale_vec.x += delta;
	}
	if (key_states['5']) {
		turtle_body.scale_vec.y -= delta;
	}
	if (key_states['6']) {
		turtle_body.scale_vec.y += delta;
	}
	if (key_states['7']) {
		turtle_body.scale_vec.x -= delta;
	}
	if (key_states['8']) {
		turtle_body.scale_vec.x += delta;
	}
}

void updateProjection() {
	// Swithcing between otho and perspective projection
	if (key_states['p']) {
		perpective_proj = !perpective_proj;
	}
}

mat4 computeProjectionMatrix(){
	mat4 proj = mat4(1.0f);
	if (perpective_proj) {
		proj = perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	}
	else {
		proj = ortho(0.0, 10.0, 0.0, 10.0, 0.1, 100.0);
	}
	return proj;
}

void display(){

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, width, height); //glViewport(0, 0, width / 2, height /2) sets viewport in bottom left quarter

	// View and projection (common to models)
	mat4 view = lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetFront(), camera.GetUp());
	mat4 proj = computeProjectionMatrix();

	// GROUND
	ground_shader.Use();
	glBindVertexArray(ground.GetVao());

	// Ground model
	mat4 ground_model = ground.GetModelLocalTransformationMatrix(); //mat4 M = T * R * S;

	ground_shader.SetUniformMat4("view", view);
	ground_shader.SetUniformMat4("proj", proj);
	ground_shader.SetUniformMat4("model", ground_model);

	glDrawArrays(GL_TRIANGLES, 0, ground.GetMeshData().mPointCount);

	// TURTLE
	turtle_shader.Use();
	turtle_shader.SetUniformMat4("view", view);
	turtle_shader.SetUniformMat4("proj", proj);

	// Update local turtle_body model
	glBindVertexArray(turtle_body.GetVao());
	mat4 turtle_body_model = turtle_body.GetModelLocalTransformationMatrix();
	turtle_body_model = ground_model * turtle_body_model;
	turtle_shader.SetUniformMat4("model", turtle_body_model);
	glDrawArrays(GL_TRIANGLES, 0, turtle_body.GetMeshData().mPointCount);

	// Update local turtle_la model
	glBindVertexArray(turtle_la.GetVao());
	mat4 turtle_la_model = turtle_la.GetModelLocalTransformationMatrix();
	turtle_la_model = ground_model * turtle_body_model * turtle_la_model;
	turtle_shader.SetUniformMat4("model", turtle_la_model);
	glDrawArrays(GL_TRIANGLES, 0, turtle_la.GetMeshData().mPointCount);

	// Update local turtle_ra model
	glBindVertexArray(turtle_ra.GetVao());
	mat4 turtle_ra_model = turtle_ra.GetModelLocalTransformationMatrix();
	turtle_ra_model = ground_model * turtle_body_model * turtle_ra_model;
	turtle_shader.SetUniformMat4("model", turtle_ra_model);
	glDrawArrays(GL_TRIANGLES, 0, turtle_ra.GetMeshData().mPointCount);

	// Update local turtle_ll model
	glBindVertexArray(turtle_ll.GetVao());
	mat4 turtle_ll_model = turtle_ll.GetModelLocalTransformationMatrix();
	turtle_ll_model = ground_model * turtle_body_model * turtle_ll_model;
	turtle_shader.SetUniformMat4("model", turtle_ll_model);
	glDrawArrays(GL_TRIANGLES, 0, turtle_ll.GetMeshData().mPointCount);

	// Update local turtle_rl model
	glBindVertexArray(turtle_rl.GetVao());
	mat4 turtle_rl_model = turtle_rl.GetModelLocalTransformationMatrix();
	turtle_rl_model = ground_model * turtle_body_model * turtle_rl_model;
	turtle_shader.SetUniformMat4("model", turtle_rl_model);
	glDrawArrays(GL_TRIANGLES, 0, turtle_rl.GetMeshData().mPointCount);

	glutSwapBuffers();
}

void updateScene() {	

	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f) {
		delta = 0.03f;
	}
	last_time = curr_time;

	camera.UpdateCamera(key_states, delta); //View
	updateModels(delta); // Update model transformation variables
	updateProjection(); //If switch between ortho/perspective

	// Draw the next frame
	glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y) {
	key_states[key] = false;
}

void keyDown(unsigned char key, int x, int y) {
	key_states[key] = true;
}

void mouseMove(int x, int y)
{
	camera.MouseMove(x, y);
}

void init()
{
	// Ground
	ground_shader = Shader("../Shaders/groundVertexShader.txt", "../Shaders/groundFragmentShader.txt");
	ground = Model(GROUND_MESH_NAME, ground_shader.GetID());

	// Turtle
	turtle_shader = Shader("../Shaders/simpleVertexShader.txt", "../Shaders/simpleFragmentShader.txt");
	turtle_body = Model(TURTLE_BODY_MESH_NAME, turtle_shader.GetID());
	turtle_la = Model(TURTLE_LEFT_ARM_MESH_NAME, turtle_shader.GetID(), vec3(3.8, 0.0, 3.4), vec3(0.0,0.0,0.0), vec3(1.0,1.0,1.0));
	turtle_ra = Model(TURTLE_RIGHT_ARM_MESH_NAME, turtle_shader.GetID(), vec3(-3.8, 0.0, 3.4), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
	turtle_ll = Model(TURTLE_LEFT_LEG_MESH_NAME, turtle_shader.GetID(), vec3(2.8, 0.0, -6.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
	turtle_rl = Model(TURTLE_RIGHT_LEG_MESH_NAME, turtle_shader.GetID(), vec3(-2.8, 0.0, -6.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));

	camera = Camera(width, height, width / 2, height / 2, 100); // View
}

int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Midterm assignment");
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardUpFunc(keyUp);
	glutKeyboardFunc(keyDown);
	glutPassiveMotionFunc(mouseMove);

	 // A call to glewInit() must be done after glut is initialized!
	glewExperimental = GL_TRUE; //for non-lab machines, this line gives better modern GL support
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
    return 0;
}



