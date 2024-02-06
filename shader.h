#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void use();

	void setBool(const std::string name, bool value) const;
	void setInt(const std::string name, int value) const;
	void setFloat(const std::string name, float value) const;
	void setFloatArray(const std::string name, float* arr, int length) const;
	void setMatrix(const std::string name, glm::mat4 MVP) const;
};


#endif