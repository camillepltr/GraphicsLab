/*******************************************************************************************
	Computer Graphics - TCD
	Camille Peltier (peltierc@tcd.ie)
	2021
	-------------------
	Turtle.cpp
	A turtle = collection of 6 models (shell + five body parts).
	This class also contains methods for crowd animation using boids (a turtle = a boid)
********************************************************************************************/

#include "Turtle.h"

//Constructors 
Turtle::Turtle() {}

Turtle::Turtle(Shader shader) {
	// Create models for the 5 body parts
	shell = Model(TURTLE_SHELL_MESH_NAME, shader.GetID(), "../Textures/shell_texture.jpg");
	shell.SetMaterial(0.8, 1.0, 2.0, 100.0);
	shell.scale_vec *= 0.75;
	la = Model(TURTLE_LEFT_ARM_MESH_NAME, shader.GetID(), vec3(3.8, 0.0, 3.4), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), "../Textures/skin_texture.jpg");
	ra = Model(TURTLE_RIGHT_ARM_MESH_NAME, shader.GetID(), vec3(-3.8, 0.0, 3.4), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), "../Textures/skin_texture.jpg");
	ll = Model(TURTLE_LEFT_LEG_MESH_NAME, shader.GetID(), vec3(2.8, 0.0, -6.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), "../Textures/skin_texture.jpg");
	rl = Model(TURTLE_RIGHT_LEG_MESH_NAME, shader.GetID(), vec3(-2.8, 0.0, -6.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0), "../Textures/skin_texture.jpg");
	head = Model(TURTLE_HEAD_MESH_NAME, shader.GetID(), "../Textures/skin_texture.jpg");

	//Initialize shell and body colour
	shell_colour = vec4(0.9, 0.5 , 0.0, 1.0);
	body_colour = vec4(0.9, 0.9, 0.0, 1.0);

	// For crowds : A Turtle = a boid
	position = &shell.translation_vec;
	velocity = vec3((float)(rand() % 100) / (float)(rand() % 100 + 5000), 0.0, (float)(rand() % 100) / (float)(rand() % 100 + 5000));

}

Turtle::Turtle(const Turtle& t) {
	shell = t.shell;
	head = t.head;
	la = t.la;
	ra = t.ra;
	ll = t.ll;
	rl = t.rl;
	shell_colour = t.shell_colour;
	body_colour = t.body_colour;
	position = &shell.translation_vec;
	velocity = vec3((float)(rand() % 100) / (float)(rand() % 100 + 5000), 0.0, (float)(rand() % 100) / (float)(rand() % 100 + 5000));
}

// Other public methods
void Turtle::MoveToNextBoidPosition(Turtle** crowd, int crowd_size, Model** obstacles, int nb_obstacles, vec3 target, float delta) {
	//vec3 initial = *position;
	// 3 rules
	vec3 v1 = separation(crowd, crowd_size);
	vec3 v2 = cohesion(crowd, crowd_size);
	vec3 v3 = alignment(crowd, crowd_size);
	// Additional 
	vec3 v4 = seekPlace(target);
	vec3 v5 = avoidObstacles(obstacles, nb_obstacles);

	velocity += v1 + v2 + v3 + v4 + v5;
	// Check if not above max speed
	if (length(velocity) > MAX_SPEED) {
		velocity /= length(velocity);
		velocity *= MAX_SPEED;
	}

	// Update postion and orientation
	*position += velocity * delta;
	//adjustOrientation(initial, *position, delta); 

}

void Turtle::MoveBodyParts(float t) {
	la.rotation_vec.y = (sin(t*0.003f) - 2) / 10;
	ra.rotation_vec.y = (sin(t * 0.003f + PI) - 2) / 10;
	ll.rotation_vec.y = (cos(t * 0.004f + PI) - 2) / 10;
	rl.rotation_vec.y = (cos(t * 0.004f) - 2) / 10;
	head.rotation_vec.x = (cos(t * 0.001f) - 2) / 20;
}

void Turtle::Draw(Shader turtle_shader, mat4 ground_model) {
	//Note : the hierarchy with the gorund isn't useful anymore, the ground is static.
	turtle_shader.SetUniformVec4("object_color", shell_colour);
	turtle_shader.SetMaterial(shell.GetMaterial());
	glBindTexture(GL_TEXTURE_2D, shell.GetTexture());

	glBindVertexArray(shell.GetVao());
	mat4 turtle_shell_model = shell.GetModelLocalTransformationMatrix();

	// Draw local shell model
	turtle_shell_model = ground_model * turtle_shell_model;
	turtle_shader.SetUniformMat4("model", turtle_shell_model);
	glDrawArrays(GL_TRIANGLES, 0, shell.GetMeshData().mPointCount);

	turtle_shader.SetUniformVec4("object_color", body_colour);
	turtle_shader.SetMaterial(la.GetMaterial());
	glBindTexture(GL_TEXTURE_2D, la.GetTexture());

	// Draw local la model
	glBindVertexArray(la.GetVao());
	mat4 turtle_la_model = la.GetModelLocalTransformationMatrix();
	turtle_la_model = ground_model * turtle_shell_model * turtle_la_model;
	turtle_shader.SetUniformMat4("model", turtle_la_model);
	glDrawArrays(GL_TRIANGLES, 0, la.GetMeshData().mPointCount);

	// Draw local ra model
	glBindVertexArray(ra.GetVao());
	mat4 turtle_ra_model = ra.GetModelLocalTransformationMatrix();
	turtle_ra_model = ground_model * turtle_shell_model * turtle_ra_model;
	turtle_shader.SetUniformMat4("model", turtle_ra_model);
	glDrawArrays(GL_TRIANGLES, 0, ra.GetMeshData().mPointCount);

	// Draw local ll model
	glBindVertexArray(ll.GetVao());
	mat4 turtle_ll_model = ll.GetModelLocalTransformationMatrix();
	turtle_ll_model = ground_model * turtle_shell_model * turtle_ll_model;
	turtle_shader.SetUniformMat4("model", turtle_ll_model);
	glDrawArrays(GL_TRIANGLES, 0, ll.GetMeshData().mPointCount);

	// Draw local rl model
	glBindVertexArray(rl.GetVao());
	mat4 turtle_rl_model = rl.GetModelLocalTransformationMatrix();
	turtle_rl_model = ground_model * turtle_shell_model * turtle_rl_model;
	turtle_shader.SetUniformMat4("model", turtle_rl_model);
	glDrawArrays(GL_TRIANGLES, 0, rl.GetMeshData().mPointCount);

	// Draw local head model
	glBindVertexArray(head.GetVao());
	mat4 turtle_head_model = head.GetModelLocalTransformationMatrix();
	turtle_head_model = ground_model * turtle_shell_model * turtle_head_model;
	turtle_shader.SetUniformMat4("model", turtle_head_model);
	glDrawArrays(GL_TRIANGLES, 0, head.GetMeshData().mPointCount);
}

// Private methods
vec3 Turtle::separation(Turtle** crowd, int n) {
	vec3 res = vec3(0, 0, 0);
	for (int i = 0; i < n; i++) {
		if ((crowd[i] != this) && (distance(*crowd[i]->position, *position) < SAFETY_DISTANCE)) {
			res -= (*crowd[i]->position - *position);
		}
	}
	return res;
}

vec3 Turtle::cohesion(Turtle** crowd, int n) {
	int N = 0;
	vec3 centroid = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < n; i++) {
		if ((crowd[i] != this) && isInVisualRange(crowd[i])) {
			centroid += *crowd[i]->position;
			N++;
		}
	}

	if (!N) {
		return vec3(0.0, 0.0, 0.0);
	}
	else {
		centroid /= (float)N;
		// Add portion of the average velocity to the boid's velocity
		vec3 res = 0.1f * (centroid - *position);
		return res;
	}
}

vec3 Turtle::alignment(Turtle** crowd, int n) {
	int N = 0;
	vec3 average = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < n; i++) {
		if ((crowd[i] != this) && isInVisualRange(crowd[i])) {
			average += crowd[i]->velocity;
			N++;
		}
	}

	if (!N) {
		return vec3(0.0, 0.0, 0.0);
	}
	else {
		average /= (float)N;
		// Move position 2% ? towards the centroid
		vec3 res = 0.05f * (average - velocity);
		return res;
	}
}

vec3 Turtle::seekPlace(vec3 target) {
	vec3 res = target - *position;
	res *= 0.01; //Limit the weight 
	return res;
}

vec3 Turtle::avoidObstacles(Model** obstacles, int n) {
	vec3 res = vec3(0, 0, 0);
	for (int i = 0; i < n; i++) {
		if (distance(obstacles[i]->translation_vec, *position) < SAFETY_DISTANCE) {
			res -= (obstacles[i]->translation_vec - *position);
		}
	}
	return res;
}

bool Turtle::isInVisualRange(Turtle* t) {
	return (distance(*position, *t->position) < VISUAL_RANGE);
}

void Turtle::adjustOrientation(vec3 v1, vec3 v2, float delta) {
	float angle = degrees(orientedAngle(normalize(vec2(v2.x - v1.x, v2.z - v1.z)), vec2(0.0 , 1.0)));
	cout << angle << endl;
	if (angle > degrees(shell.rotation_vec.y)) shell.rotation_vec.y -= delta;
	if (angle < degrees(shell.rotation_vec.y)) shell.rotation_vec.y += delta;
}