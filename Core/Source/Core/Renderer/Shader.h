#pragma once

#include <glad/glad.h>

#include <string>

class Shader
{
public:
	// the program ID
	unsigned int ID;

	Shader() = default;
	// contructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);

	// Utility functions
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};