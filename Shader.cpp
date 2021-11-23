/*******************************************************************************************
	Computer Graphics - TCD
	Camille Peltier (peltierc@tcd.ie)
	2021
	-------------------
	Shader.cpp
********************************************************************************************/
#include "Shader.h"

// Constructors
Shader::Shader() {}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	compileShaders(vertexShaderPath, fragmentShaderPath);
}

// Public methods
void Shader::Use() {
	glUseProgram(shaderProgramID);
}

GLuint Shader::GetID() {
	return shaderProgramID;
}

GLint Shader::GetUniformLocation(string uniformName) const {
	GLint gLocation = glGetUniformLocation(shaderProgramID, uniformName.c_str());
	if (gLocation == -1) {
		fprintf(stderr, "Error getting location of uniform '%s'\n", uniformName.c_str());
		exit(1);
	}
	return gLocation;
}

void Shader::SetUniformMat4(string uniformName, mat4 value) const {
	GLint gLocation = GetUniformLocation(uniformName);
	glUniformMatrix4fv(gLocation, 1, GL_FALSE, value_ptr(value));
}

void Shader::SetUniformVec3(string uniformName, vec3 value) const {
	GLint gLocation = GetUniformLocation(uniformName);
	glUniform3fv(gLocation, 1, value_ptr(value));
}

void Shader::SetUniformVec4(string uniformName, vec4 value) const {
	GLint gLocation = GetUniformLocation(uniformName);
	glUniform4fv(gLocation, 1, value_ptr(value));
}

void Shader::SetUniformFloat(string uniformName, float value) const {
	GLint gLocation = GetUniformLocation(uniformName);
	glUniform1f(gLocation, value);
}

void Shader::SetUniformInt(string uniformName, int value) const {
	GLint gLocation = GetUniformLocation(uniformName);
	glUniform1i(gLocation, value);
}

void Shader::SetLight(string uniformName, Light l) const {
	SetUniformVec3(uniformName + ".ambient_colour", l.ambient_colour);
	SetUniformVec3(uniformName + ".diffuse_colour", l.diffuse_colour);
	SetUniformVec3(uniformName + ".specular_colour", l.specular_colour);
	SetUniformVec3(uniformName + ".position", l.position);
}

void Shader::SetMaterial(Material m) const {
	SetUniformFloat("material.Ka", m.Ka);
	SetUniformFloat("material.Kd", m.Kd);
	SetUniformFloat("material.Ks", m.Ks);
	SetUniformFloat("material.phong_exponent", m.phong_exponent);
}

// Private methods
char* Shader::readShaderFile(const char* path) {
	FILE* fp;
	fopen_s(&fp, path, "rb");
	if (fp == NULL) { 
		cerr << "could not open " << path << endl;
		return NULL; 
	}
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';
	fclose(fp);
	return buf;
}

void Shader::addShader(GLuint ShaderProgram, const char* shaderPath, GLenum ShaderType) {
	// Create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		cerr << "Error creating shader..." << endl;
		cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderFile(shaderPath);
	if (pShaderSource != NULL) {
		// Bind the source code to the shader
		glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
		// compile the shader and check for errors
		glCompileShader(ShaderObj);
		GLint success;
		glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar InfoLog[1024] = { '\0' };
			glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
			cerr << "Error compiling "
				<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
				<< " shader program: " << InfoLog << endl;
			cin.get();
			exit(1);
		}
		// Attach the compiled shader object to the program object
		glAttachShader(ShaderProgram, ShaderObj);
	}
}

GLuint Shader::compileShaders(const char* vertexShaderPath, const char* fragmentShaderPath) {
	//Creating a program ID. We will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		cerr << "Error creating shader program..." << endl;
		cin.get();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	addShader(shaderProgramID, vertexShaderPath, GL_VERTEX_SHADER);
	addShader(shaderProgramID, fragmentShaderPath, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };

	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		cerr << "Error linking shader program: " << ErrorLog << endl;
		cin.get();
		exit(1);
	}

	// Needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		cerr << "Invalid shader program: " << ErrorLog << endl;
		cin.get();
		exit(1);
	}
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}

