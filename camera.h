#pragma once

#include "Global.h"

const float SPEED = 3.5f;
const float SENSITIVITY = 0.1f;
const float EXPOSUREMAX = 5.0f;
const float EXPOSURESENSITIVITY = 0.2f;

class Camera {
private:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;

	float pitch, yaw;
	float fov = 60.0f;

	float currentFrame = 0.0, lastFrame = 0.0;
	bool firstmouse = true;
	float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;
	float exposure = 1.0f;

public:
	float width = SCR_WIDTH, height = SCR_HEIGHT;
	float Speed = SPEED;
	float Sensitivity = SENSITIVITY;
	float Exposure_Sensitivity = EXPOSURESENSITIVITY;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, SunSize * 2.5f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 getPosition();

	void ProcessKeyboard(GLFWwindow* window, glm::vec3 Sun, glm::vec3 Earth, float r);
	void ProcessMouse(double xpos, double ypos);
	void ProcessScroll(double xoffset, double yoffset);

	glm::mat4 GetViewMatrix()const;
	float GetFOV()const;
	float GetExposure()const;
};