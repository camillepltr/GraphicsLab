/*******************************************************************************************
	Computer Graphics - TCD
	Camille Peltier (peltierc@tcd.ie)
	2021
	-------------------
	main.cpp
********************************************************************************************/

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
#include "Skybox.h"

typedef unsigned long DWORD;

using namespace std;
using namespace glm;

int width = 1000.0;
int height = 750.0;
bool key_states[256];
bool animation = true;

// Shaders
Shader shader_with_texture;
Shader skybox_shader;

// Skybox
Skybox skybox;
vector<const char*> FACES_TEXTURE_NAMES =
{
	"../Textures/Skybox/front.jpg",
	"../Textures/Skybox/back.jpg",
	"../Textures/Skybox/top.jpg",
	"../Textures/Skybox/bottom.jpg",
	"../Textures/Skybox/left.jpg",
	"../Textures/Skybox/right.jpg"
};

// Models
#define GROUND_MESH_NAME "../Meshes/island.dae"
#define SEA_MESH_NAME "../Meshes/sea.dae"
#define ROCK_MESH_NAME "../Meshes/rock.dae"
#define PLANT_MESH_NAME "../Meshes/plant.dae"
Model ground;
Model sea;
Turtle** turtles; // Array of turtles = crowd
Model** rocks;
Model plant;
int crowd_size;
int nb_rocks;

// View
Camera camera;

// Projection
bool perpective_proj = true;

// Light sources
Light light;
Light orange_light;

//Mostly used for mid-term
void updateModels(float delta) {
	// Note: the choice of the keys makes sense on an AZERTY keyboard
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
		cout << turtles[0]->shell.rotation_vec.y << endl;
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

// Projection type + activation of animation
void updateParams() {
	// Swithcing between orthogonal and perspective projection (mid term)
	if (key_states['p']) {
		perpective_proj = !perpective_proj;
	}

	// Activate turtles and sea animation
	if (key_states['m']) {
		animation = !animation;
	}
}

mat4 computeProjectionMatrix(){
	mat4 proj = mat4(1.0f);
	if (perpective_proj) {
		proj = perspective(45.0f, (float)width / (float)height, 0.1f, 2000.0f);
	}
	else {
		proj = ortho(0.0, 10.0, 0.0, 10.0, 0.1, 100.0);
	}
	return proj;
}

// Rendering loop
void display() {
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// For transparency (alpha blending)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	// View and projection (common to all models)
	mat4 view = lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetFront(), camera.GetUp());
	mat4 proj = computeProjectionMatrix();

	// Skybox
	glDepthMask(GL_FALSE);
	skybox_shader.Use();
	mat4 view2 = mat4(mat3(view)); // Remove the translation part so that it always seems far away (= skybox always centerd around viewer)
	skybox_shader.SetUniformMat4("view", view2);
	skybox_shader.SetUniformMat4("proj", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetSkyboxTexture());
	glBindVertexArray(skybox.GetSkyboxVAO());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);


	shader_with_texture.Use();
	shader_with_texture.SetUniformMat4("view", view);
	shader_with_texture.SetUniformMat4("proj", proj);
	shader_with_texture.SetLight("light1", light);
	shader_with_texture.SetLight("light2", orange_light);
	shader_with_texture.SetUniformVec3("view_position", camera.GetPosition());

	// GROUND
	glBindVertexArray(ground.GetVao());
	mat4 ground_model = ground.GetModelLocalTransformationMatrix(); //mat4 M = T * R * S;
	shader_with_texture.SetUniformMat4("model", ground_model);
	shader_with_texture.SetUniformVec4("object_color", vec4(1.0, 1.0, 1.0, 1.0));
	shader_with_texture.SetMaterial(ground.GetMaterial());
	glBindTexture(GL_TEXTURE_2D, ground.GetTexture());
	glDrawArrays(GL_TRIANGLES, 0, ground.GetMeshData().mPointCount);

	// ROCKS
	shader_with_texture.SetMaterial(rocks[0]->GetMaterial());
	glBindTexture(GL_TEXTURE_2D, rocks[0]->GetTexture());
	for (int i = 0; i < nb_rocks; i++)
	{
		glBindVertexArray(rocks[i]->GetVao());
		mat4 ground_model = rocks[i]->GetModelLocalTransformationMatrix(); //mat4 M = T * R * S;
		shader_with_texture.SetUniformMat4("model", ground_model);
		glDrawArrays(GL_TRIANGLES, 0, rocks[i]->GetMeshData().mPointCount);
	}

	// PLANT
	glBindVertexArray(plant.GetVao());
	mat4 plant_model = plant.GetModelLocalTransformationMatrix();
	shader_with_texture.SetUniformMat4("model", plant_model); //mat4 M = T * R * S;
	shader_with_texture.SetMaterial(plant.GetMaterial());
	glBindTexture(GL_TEXTURE_2D, plant.GetTexture());
	glDrawArrays(GL_TRIANGLES, 0, plant.GetMeshData().mPointCount);


	// TURTLES
	for(int i = 0; i < crowd_size; i++) {
		turtles[i]->Draw(shader_with_texture, ground_model);
	}

	// SEA
	glBindVertexArray(sea.GetVao());
	mat4 sea_model = sea.GetModelLocalTransformationMatrix(); //mat4 M = T * R * S;
	shader_with_texture.SetUniformMat4("model", sea_model);
	shader_with_texture.SetMaterial(sea.GetMaterial());
	shader_with_texture.SetUniformVec4("object_color", vec4(1.0, 1.0, 1.0, 0.6)); // Particular value alpha=0.6 for transparency
	glBindTexture(GL_TEXTURE_2D, sea.GetTexture());
	glDrawArrays(GL_TRIANGLES, 0, sea.GetMeshData().mPointCount);

	glutSwapBuffers();
}

// Update loop
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
	updateModels(delta); // Update model transformation variables (mostly used for mid-term to move and scale the first turtle)
	updateParams(); // Projection type + activation of the animation

	if (animation) {
		for (int i = 0; i < crowd_size; i++) {
			turtles[i]->MoveToNextBoidPosition(turtles, crowd_size, rocks, nb_rocks, plant.translation_vec, delta);
			turtles[i]->MoveBodyParts(curr_time); //Just for animation
		}
		sea.translation_vec.y += 0.3*delta;
	}
		
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

// Initialization of the shaders, models, camera, light sources
void init()
{
	// Shaders
	shader_with_texture = Shader("../Shaders/vertexShaderWithTexture.txt", "../Shaders/fragmentShaderWithTexture2Lights.txt");
	skybox_shader = Shader("../Shaders/skyboxVertexShader.txt", "../Shaders/skyboxFragmentShader.txt");

	// Skybox 
	skybox = Skybox(FACES_TEXTURE_NAMES, skybox_shader.GetID(), 1000);

	// Ground
	ground = Model(GROUND_MESH_NAME, shader_with_texture.GetID(), "../Textures/sand_texture.jpg");
	ground.SetMaterial(0.6, 0.9, 1.8, 100.0);

	// Rocks
	nb_rocks = 20;
	rocks = new Model* [nb_rocks];
	rocks[0] = new Model(ROCK_MESH_NAME, shader_with_texture.GetID(), "../Textures/rock_texture.jpg");
	rocks[0]->translation_vec.z -= 30;
	for (int i = 1; i < nb_rocks; i++) {
		// Make copies of the first rock
		rocks[i] = new Model(*rocks[0]);
		// Put them in random location + rotate them
		rocks[i]->translation_vec.x += rand() % 700 - 600;
		rocks[i]->translation_vec.z += rand() % 900 - 350;
		rocks[i]->rotation_vec.y += rand() % 10;
	}

	// Plant
	plant = Model(PLANT_MESH_NAME, shader_with_texture.GetID(), "../Textures/plant_texture.jpg");
	plant.translation_vec = vec3(-700.0, 0.0, -100.0);


	// Sea
	sea = Model(SEA_MESH_NAME, shader_with_texture.GetID(), "../Textures/sea_texture.jpg");
	sea.translation_vec.y -= 20;

	// Turtles
	crowd_size = 12;
	turtles = new Turtle* [crowd_size];
	turtles[0] = new Turtle(shader_with_texture);
	turtles[0]->shell.rotation_vec.y = -PI/2.0;

	for (int i = 1; i < crowd_size; i++) {
		// Make copies of the first turtle (boid) and put them in a different location
		turtles[i] = new Turtle(*turtles[0]);
		// Using translation vector for position (= origin (0,0,0) + translation defined by translation_vector)
		turtles[i]->shell.translation_vec.x += rand() % 500 - 250;
		turtles[i]->shell.translation_vec.z += rand() % 600 - 300;
	}

	// Make some of the turles different
	for (int i = 0; i < 0.7 * crowd_size; i++) {
		turtles[i]->shell.scale_vec *= 1.15;
		turtles[i]->shell.SetMaterial(0.2, 0.5, 2.0, 50.0);
		turtles[i]->la.SetMaterial(0.8, 0.8, 2.0, 90.0);
	}

	// Initialize the camera
	camera = Camera(width, height, width / 2, height / 2, 100); // View

	// 2 light sources
	light = Light {
		vec3(0.5, 0.5, 0.5),
		vec3(1.0, 1.0, 1.0),
		vec3(0.5, 0.5, 0.5),
		vec3(600.0, 100.0, -600) // Position
	};
	orange_light = Light{
		vec3(0.5, 0.25, 0.1),
		vec3(1.0, 0.5, 0.2),
		vec3(1.0, 0.5, 0.2),
		vec3(-800.0, 100.0, -800) // Position
	};
}

int main(int argc, char** argv){

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Computer Graphics project");
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
