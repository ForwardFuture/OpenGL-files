#include "shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::string vShaderCode, fShaderCode;
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vShaderCode = vShaderStream.str();
		fShaderCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderHandle = vShaderCode.c_str();
	const char* fShaderHandle = fShaderCode.c_str();

	//Error Log
	int success;
	char errlog[512];

	//Set vertexShader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderHandle, NULL);
	glCompileShader(vertexShader);
	//Print Error Log
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errlog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errlog << std::endl;
	};

	//Set fragmentShader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderHandle, NULL);
	glCompileShader(fragmentShader);
	//Print Error Log
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errlog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << errlog << std::endl;
	};

	//Set Shader Program Object(SPO)
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	//Print Error Log
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, errlog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errlog << std::endl;
	}

	//Delete Shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloatArray(const std::string name, float* arr, int length) const {
	glUniform1fv(glGetUniformLocation(ID, name.c_str()), length, arr);
}

void Shader::setMatrix(const std::string name, glm::mat4 MVP) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(MVP));
}