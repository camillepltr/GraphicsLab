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
void Turtle::MoveToNextBoidPosition(Turtle** crowd, int n, float delta) {
	vec3 initial = *position;
	vec3 v1 = separation(crowd, n);
	vec3 v2 = cohesion(crowd, n);
	vec3 v3 = alignment(crowd, n);
	velocity += v1 + v2 + v3;
	// Check if not above max speed
	if (length(velocity) > MAX_SPEED) {
		velocity /= length(velocity);
		velocity *= MAX_SPEED;
	}
	*position += velocity * delta;

	// Rotate around y axis (as turtle move on (x,z) plane, in the direction of the velocit
	shell.rotation_vec.y += direction(initial, *position) * delta;
}

void Turtle::Draw(Shader turtle_shader, mat4 ground_model) {


	turtle_shader.SetUniformVec4("object_color", shell_colour);
	turtle_shader.SetMaterial(shell.GetMaterial());
	glBindTexture(GL_TEXTURE_2D, shell.GetTexture());

	glBindVertexArray(shell.GetVao());
	mat4 turtle_shell_model = shell.GetModelLocalTransformationMatrix();

	// Update local shell model
	turtle_shell_model = ground_model * turtle_shell_model;
	turtle_shader.SetUniformMat4("model", turtle_shell_model);
	glDrawArrays(GL_TRIANGLES, 0, shell.GetMeshData().mPointCount);

	turtle_shader.SetUniformVec4("object_color", body_colour);
	turtle_shader.SetMaterial(la.GetMaterial());
	glBindTexture(GL_TEXTURE_2D, la.GetTexture());

	// Update local la model
	glBindVertexArray(la.GetVao());
	mat4 turtle_la_model = la.GetModelLocalTransformationMatrix();
	turtle_la_model = ground_model * turtle_shell_model * turtle_la_model;
	turtle_shader.SetUniformMat4("model", turtle_la_model);
	glDrawArrays(GL_TRIANGLES, 0, la.GetMeshData().mPointCount);

	// Update local ra model
	glBindVertexArray(ra.GetVao());
	mat4 turtle_ra_model = ra.GetModelLocalTransformationMatrix();
	turtle_ra_model = ground_model * turtle_shell_model * turtle_ra_model;
	turtle_shader.SetUniformMat4("model", turtle_ra_model);
	glDrawArrays(GL_TRIANGLES, 0, ra.GetMeshData().mPointCount);

	// Update local ll model
	glBindVertexArray(ll.GetVao());
	mat4 turtle_ll_model = ll.GetModelLocalTransformationMatrix();
	turtle_ll_model = ground_model * turtle_shell_model * turtle_ll_model;
	turtle_shader.SetUniformMat4("model", turtle_ll_model);
	glDrawArrays(GL_TRIANGLES, 0, ll.GetMeshData().mPointCount);

	// Update local rl model
	glBindVertexArray(rl.GetVao());
	mat4 turtle_rl_model = rl.GetModelLocalTransformationMatrix();
	turtle_rl_model = ground_model * turtle_shell_model * turtle_rl_model;
	turtle_shader.SetUniformMat4("model", turtle_rl_model);
	glDrawArrays(GL_TRIANGLES, 0, rl.GetMeshData().mPointCount);

	// Update local head model
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
		if ((crowd[i] != this) && (distance(*crowd[i]->position, *position) < MAX_DISTANCE_BETWEEN_BOIDS)) {
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

bool Turtle::isInVisualRange(Turtle* t) {
	return (distance(*position, *t->position) < VISUAL_RANGE);
}

int Turtle::direction(vec3 v1, vec3 v2) {
	float cos = (float)dot(v1, v2) / (float)(length(v1) * length(v2));
	if (cos == 1) {
		//Same direction : dont turn
		return 0;
	}
	else if (cos == -1) {
		// Opposite direction : doesn't matter, lets say turn in positive direction
		return 1;
	}
	// Else Return sign of cross product
	vec3 cross_p = cross(v1, v2);
	if (cross_p.y > 0) {
		return 1;
	}
	else {
		return -1;
	}
}