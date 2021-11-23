
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
#include "Skybox.h"

typedef unsigned long DWORD;

using namespace std;
using namespace glm;

int width = 1000.0;
int height = 750.0;
bool key_states[256];


GLuint vao;
Shader shader_programme;
GLuint tex_cube;
// Shaders
Shader shader_with_texture;
//Shader skybox_shader;

#define GROUND_MESH_NAME "../Meshes/island.dae"
#define SEA_MESH_NAME "../Meshes/sea.dae"
vector<string> FACES_TEXTURE_NAMES = 
{
	"../Textures/Skybox/right.jpg",
	"../Textures/Skybox/left.jpg",
	"../Textures/Skybox/top.jpg",
	"../Textures/Skybox/bottom.jpg",
	"../Textures/Skybox/front.jpg",
	"../Textures/Skybox/back.jpg"
};

// Models
//Skybox skybox;
Model ground;
Model sea;
Turtle** turtles; // Turtle = collection of 5 Model objects for the 5 body parts; Array = for crowd
int crowd_size;

// View
Camera camera;

// Projection
bool perpective_proj = true;

// Light sources
Light light;
Light orange_light;

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

void updateProjection() {
	// Swithcing between otho and perspective projection
	if (key_states['p']) {
		perpective_proj = !perpective_proj;
	}
}

mat4 computeProjectionMatrix(){
	mat4 proj = mat4(1.0f);
	if (perpective_proj) {
		proj = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	}
	else {
		proj = ortho(0.0, 10.0, 0.0, 10.0, 0.1, 100.0);
	}
	return proj;
}

void display(){

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); 
	//glEnable(GL_TEXTURE_CUBE_MAP);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// For transparency (alpha blending)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	// View and projection (common to models)
	mat4 view = lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetFront(), camera.GetUp());
	mat4 proj = computeProjectionMatrix();

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

	// TURTLES
	for(int i = 0; i < crowd_size; i++) {
		turtles[i]->Draw(shader_with_texture, ground_model);
	}

	// SEA
	glBindVertexArray(sea.GetVao());
	mat4 sea_model = sea.GetModelLocalTransformationMatrix();
	shader_with_texture.SetUniformMat4("model", sea_model);
	shader_with_texture.SetMaterial(sea.GetMaterial());
	shader_with_texture.SetUniformVec4("object_color", vec4(1.0, 1.0, 1.0, 0.8));
	glBindTexture(GL_TEXTURE_2D, sea.GetTexture());
	glDrawArrays(GL_TRIANGLES, 0, sea.GetMeshData().mPointCount);

	// Skybox
	//glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	//skybox_shader.Use();
	//view = mat4(mat3(view)); // Remove translation from the view matrix
	//skybox_shader.SetUniformMat4("view", view);
	//skybox_shader.SetUniformMat4("proj", proj);
	//glBindVertexArray(skybox.GetSkyboxVAO());
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetSkyboxTexture());
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(0);
	//glDepthFunc(GL_LESS); // set depth function back to default


	glDepthMask(GL_FALSE);
	shader_programme.Use();
	shader_programme.SetUniformMat4("view", view);
	shader_programme.SetUniformMat4("proj", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

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
	for (int i = 0; i < crowd_size; i++) {
		turtles[i]->MoveToNextBoidPosition(turtles, crowd_size, delta);
	}
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

bool load_cube_map_side(
	GLuint texture, GLenum side_target, const char* file_name) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(
		file_name, &x, &y, &n, force_channels);
	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// non-power-of-2 dimensions check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(stderr,
			"WARNING: image %s is not power-of-2 dimensions\n",
			file_name);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D(
		side_target,
		0,
		GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data);
	free(image_data);
	return true;
}

void create_cube_map(
	const char* front,
	const char* back,
	const char* top,
	const char* bottom,
	const char* left,
	const char* right,
	GLuint* tex_cube) {
	// generate a cube-map texture to hold all the sides
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, tex_cube);

	// load each image and copy into a side of the cube-map texture
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void init()
{
	float points[] = {
  -500.0f,  500.0f, -500.0f,
  -500.0f, -500.0f, -500.0f,
   500.0f, -500.0f, -500.0f,
   500.0f, -500.0f, -500.0f,
   500.0f,  500.0f, -500.0f,
  -500.0f,  500.0f, -500.0f,

  -500.0f, -500.0f,  500.0f,
  -500.0f, -500.0f, -500.0f,
  -500.0f,  500.0f, -500.0f,
  -500.0f,  500.0f, -500.0f,
  -500.0f,  500.0f,  500.0f,
  -500.0f, -500.0f,  500.0f,

   500.0f, -500.0f, -500.0f,
   500.0f, -500.0f,  500.0f,
   500.0f,  500.0f,  500.0f,
   500.0f,  500.0f,  500.0f,
   500.0f,  500.0f, -500.0f,
   500.0f, -500.0f, -500.0f,

  -500.0f, -500.0f,  500.0f,
  -500.0f,  500.0f,  500.0f,
   500.0f,  500.0f,  500.0f,
   500.0f,  500.0f,  500.0f,
   500.0f, -500.0f,  500.0f,
  -500.0f, -500.0f,  500.0f,

  -500.0f,  500.0f, -500.0f,
   500.0f,  500.0f, -500.0f,
   500.0f,  500.0f,  500.0f,
   500.0f,  500.0f,  500.0f,
  -500.0f,  500.0f,  500.0f,
  -500.0f,  500.0f, -500.0f,

  -500.0f, -500.0f, -500.0f,
  -500.0f, -500.0f,  500.0f,
   500.0f, -500.0f, -500.0f,
   500.0f, -500.0f, -500.0f,
  -500.0f, -500.0f,  500.0f,
   500.0f, -500.0f,  500.0f
	};
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	shader_programme = Shader("../Shaders/skyboxVertexShader.txt", "../Shaders/skyboxFragmentShader.txt");
	create_cube_map("../Textures/Skybox/right.jpg",
		"../Textures/Skybox/left.jpg",
		"../Textures/Skybox/top.jpg",
		"../Textures/Skybox/bottom.jpg",
		"../Textures/Skybox/front.jpg",
		"../Textures/Skybox/back.jpg", 
		&tex_cube);

	shader_with_texture = Shader("../Shaders/vertexShaderWithTexture.txt", "../Shaders/fragmentShaderWithTexture2Lights.txt");
	//skybox_shader = Shader("../Shaders/skyboxVertexShader.txt", "../Shaders/skyboxFragmentShader.txt");

	//// Skybox
	//skybox = Skybox(FACES_TEXTURE_NAMES, skybox_shader.GetID());
	//skybox_shader.Use();
	//skybox_shader.SetUniformInt("skybox", 0);

	// Ground
	ground = Model(GROUND_MESH_NAME, shader_with_texture.GetID(), "../Textures/sand_texture.jpg");
	ground.SetMaterial(0.6, 0.9, 1.8, 100.0);

	// Sea
	sea = Model(SEA_MESH_NAME, shader_with_texture.GetID(), "../Textures/sea_texture.jpg");

	// Turtles
	crowd_size = 12;
	turtles = new Turtle* [crowd_size];
	turtles[0] = new Turtle(shader_with_texture);

	for (int i = 1; i < crowd_size; i++) {
		// Make copies of the first turtle (boid) and put them in a different location
		turtles[i] = new Turtle(*turtles[0]);
		// Using translation vector for position (= origin (0,0,0) + translation defined by translation_vector)
		turtles[i]->shell.translation_vec.x += rand() % 500 - 250;
		turtles[i]->shell.translation_vec.z += rand() % 500 - 250;
	}



	camera = Camera(width, height, width / 2, height / 2, 100); // View
	light = Light {
		vec3(0.5, 0.5, 0.5),
		vec3(1.0, 1.0, 1.0),
		vec3(0.5, 0.5, 0.5),
		vec3(10.0, 10.0, 4.0) // Position
	};
	orange_light = Light{
		vec3(0.5, 0.25, 0.1),
		vec3(1.0, 0.5, 0.2),
		vec3(1.0, 0.5, 0.2),
		vec3(-100.0, 50.0, 4.0) // Position
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



