
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
#include "Turtle.h"
#include "Camera.h"
#include "Struct.h"

typedef unsigned long DWORD;

using namespace std;
using namespace glm;

int width = 800.0;
int height = 600.0;
bool key_states[256];

// Shaders
Shader ground_shader;
Shader turtle_shader;

#define GROUND_MESH_NAME "../Meshes/ground.dae"

// Models
Model ground;
Turtle* turtles[5]; // Turtle = collection of 5 Model objects for the 5 body parts; Array = for crowd


// View
Camera camera;

// Projection
bool perpective_proj = true;

// Light source
Light light;

Material material; // Same for all for now, to adapt (ex : highest phong exponennt for turtle shell)


void updateModels(float delta) {
	// Translations
	if (key_states['e']) {
		turtles[0]->shell.translation_vec.z -= 10.0f*delta;
	}
	if (key_states['r']) {
		turtles[0]->shell.translation_vec.z += 10.0f * delta;
	}
	if (key_states['t']) {
		turtles[0]->shell.translation_vec.x -= 10.0f * delta;
	}
	if (key_states['y']) {
		turtles[0]->shell.translation_vec.x += 10.0f * delta;
	}
	if (key_states['9']) {
		turtles[0]->shell.translation_vec.y -= 10.0f * delta;
	}
	if (key_states['0']) {
		turtles[0]->shell.translation_vec.y += 10.0f * delta;
	}


	// Rotations
	if (key_states['a']) {
		turtles[0]->shell.rotation_vec.y -= delta;
	}
	if (key_states['z']) {
		turtles[0]->shell.rotation_vec.y += delta;
	}

	if (key_states['q']) {
		if (turtles[0]->la.rotation_vec.y > -0.2) {
			turtles[0]->la.rotation_vec.y -= delta;
		}
	}
	if (key_states['s']) {
		if (turtles[0]->la.rotation_vec.y < 0) {
			turtles[0]->la.rotation_vec.y += delta;
		}
	}

	if (key_states['d']) {
		if (turtles[0]->ra.rotation_vec.y > -0.2) {
			turtles[0]->ra.rotation_vec.y -= delta;
		}
	}
	if (key_states['f']) {
		if (turtles[0]->ra.rotation_vec.y < 0) {
			turtles[0]->ra.rotation_vec.y += delta;
		}
	}

	if (key_states['g']) {
		if (turtles[0]->ll.rotation_vec.y > -0.2) {
			turtles[0]->ll.rotation_vec.y -= delta;
		}
	}
	if (key_states['h']) {
		if (turtles[0]->ll.rotation_vec.y < 0) {
			turtles[0]->ll.rotation_vec.y += delta;
		}
	}

	if (key_states['j']) {
		if (turtles[0]->rl.rotation_vec.y > -0.2) {
			turtles[0]->rl.rotation_vec.y -= delta;
		}
	}
	if (key_states['k']) {
		if (turtles[0]->rl.rotation_vec.y < 0) {
			turtles[0]->rl.rotation_vec.y += delta;
		}
	}

	// Uniform scaling
	if (key_states['1']) {
		turtles[0]->shell.scale_vec -= vec3(delta, delta, delta);
	}
	if (key_states['2']) {
		turtles[0]->shell.scale_vec += vec3(delta, delta, delta);
	}

	// Non uniform scaling
	if (key_states['3']) {
		turtles[0]->shell.scale_vec.x -= delta;
	}
	if (key_states['4']) {
		turtles[0]->shell.scale_vec.x += delta;
	}
	if (key_states['5']) {
		turtles[0]->shell.scale_vec.y -= delta;
	}
	if (key_states['6']) {
		turtles[0]->shell.scale_vec.y += delta;
	}
	if (key_states['7']) {
		turtles[0]->shell.scale_vec.x -= delta;
	}
	if (key_states['8']) {
		turtles[0]->shell.scale_vec.x += delta;
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

	glViewport(0, 0, width, height);

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
	ground_shader.SetUniformVec3("object_color", vec3(0.66, 0.66, 0.66));
	ground_shader.SetUniformVec3("view_position", camera.GetPosition());
	ground_shader.SetLight(light);
	ground_shader.SetMaterial(material);

	glDrawArrays(GL_TRIANGLES, 0, ground.GetMeshData().mPointCount);

	// TURTLES
	turtle_shader.Use();
	turtle_shader.SetUniformMat4("view", view);
	turtle_shader.SetUniformMat4("proj", proj);
	turtle_shader.SetUniformVec3("view_position", camera.GetPosition());
	turtle_shader.SetLight(light);
	turtle_shader.SetMaterial(material);
	for(int i = 0; i < 5; i++) {
		turtles[i]->Draw(turtle_shader, ground_model);
	}

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
	ground_shader = Shader("../Shaders/simpleVertexShader.txt", "../Shaders/simpleFragmentShader.txt");
	ground = Model(GROUND_MESH_NAME, ground_shader.GetID());

	// Turtle
	turtle_shader = Shader("../Shaders/vertexShaderWithTexture.txt", "../Shaders/fragmentShaderWithTexture.txt");
	turtles[0] = new Turtle(turtle_shader);

	for (int i = 1; i < 5; i++) {
		// Make copies of the first turtle and put them in a different location
		turtles[i] = new Turtle(*turtles[0]);
		turtles[i]->shell.translation_vec.x += 20.0*i;
	}

	camera = Camera(width, height, width / 2, height / 2, 100); // View
	light = Light {
		vec3(0.5, 0.5, 0.5),
		vec3(1.0, 1.0, 1.0),
		vec3(0.5, 0.5, 0.5),
		vec3(10.0, 10.0, 4.0) // Position
	};
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



