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

Turtle::Turtle(const Turtle& t) {
	shell = t.shell;
	la = t.la;
	ra = t.ra;
	ll = t.ll;
	rl = t.rl;
	shell_colour = t.shell_colour;
	body_colour = t.body_colour;
}

void Turtle::Draw(Shader turtle_shader, mat4 ground_model) {


	turtle_shader.SetUniformVec3("object_color", shell_colour);
	glBindTexture(GL_TEXTURE_2D, shell.GetTexture());

	glBindVertexArray(shell.GetVao());
	mat4 turtle_shell_model = shell.GetModelLocalTransformationMatrix();

	// Update local shell model
	turtle_shell_model = ground_model * turtle_shell_model;
	turtle_shader.SetUniformMat4("model", turtle_shell_model);
	glDrawArrays(GL_TRIANGLES, 0, shell.GetMeshData().mPointCount);

	turtle_shader.SetUniformVec3("object_color", body_colour);
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
}