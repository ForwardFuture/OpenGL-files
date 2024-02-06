#include "camera.h"

//NOTE: You must ensure x & y are not equal to 0
static float CalcAngle(float x, float y, int quadrant) {
	float Angle = 0.0f;
	
	switch (quadrant) {
		case 1:
			Angle = atan(y / x) / PI * 180.0f;
			break;
		case 2:
			Angle = 180.0f + atan(y / x) / PI * 180.0f;
			break;
		case 3:
			Angle = 180.0f + atan(y / x) / PI * 180.0f;
			break;
		case 4:
			Angle = atan(y / x) / PI * 180.0f;
			break;
	}
	return Angle;
}

static std::pair<float, float> Update(glm::vec3 front) {

	float pitch_new = asin(front.y) / PI * 180.0f;
	if (pitch_new > 89.0f)pitch_new = 89.0f;
	if (pitch_new < -89.0f)pitch_new = -89.0f;

	float yaw_new = 0.0f;

	if (fabs(front.z) < eps) {
		if (front.x > 0)yaw_new = 90.0f;
		else yaw_new = -90.0f;
	}
	else if (fabs(front.x) < eps) {
		if (front.z > 0)yaw_new = 0.0f;
		else yaw_new = 180.0f;
	}
	else {
		int quadrant = 0;
		if (front.x > 0) {
			if (front.z > 0) quadrant = 1;
			else quadrant = 2;
		}
		else {
			if (front.z < 0)quadrant = 3;
			else quadrant = 4;
		}
		yaw_new = CalcAngle(front.z, front.x, quadrant);
	}

	return std::make_pair(pitch_new, yaw_new);
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) {
	Position = position;
	Front = glm::normalize(front);
	Up = glm::normalize(up);
	
	std::pair<float, float> result = Update(Front);
	pitch = result.first;
	yaw = result.second;
}

glm::vec3 Camera::getPosition() {
	return Position;
}

void Camera::ProcessKeyboard(GLFWwindow* window, glm::vec3 Sun, glm::vec3 Earth, float r) {
	currentFrame = glfwGetTime();
	float deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = Speed * deltaTime;

	glm::vec3 newPosition = Position;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		newPosition += cameraSpeed * Front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		newPosition -= cameraSpeed * Front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		newPosition -= cameraSpeed * glm::normalize(glm::cross(Front, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		newPosition += cameraSpeed * glm::normalize(glm::cross(Front, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		newPosition += cameraSpeed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		newPosition -= cameraSpeed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		glm::vec3 Dir = glm::normalize(Earth - Sun) * r;
		newPosition = Earth + Dir * 3.0f;
		Front = glm::normalize(-Dir);

		std::pair<float, float> result = Update(Front);
		pitch = result.first;
		yaw = result.second;

		firstmouse = true;
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		newPosition = glm::vec3(0.0f, 0.0f, SunSize * 2.5f);
		Front = glm::vec3(0.0f, 0.0f, -1.0f);

		std::pair<float, float> result = Update(Front);
		pitch = result.first;
		yaw = result.second;

		firstmouse = true;
	}

	if (glm::length(newPosition) < StarsSize - 20.0f)
		Position = newPosition;
}

void Camera::ProcessMouse(double xpos, double ypos) {

	if (firstmouse) {
		lastX = xpos;
		lastY = ypos;
		firstmouse = false;
	}

	float xoffset = lastX - xpos;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;
	pitch += yoffset;
	yaw += xoffset;

	if (pitch > 89.0f)pitch = 89.0f;
	if (pitch < -89.0f)pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	Front = glm::normalize(front);
}

void Camera::ProcessScroll(double xoffset, double yoffset) {

	exposure += yoffset * Exposure_Sensitivity;
	if (exposure <= 0.1f)exposure = 0.1f;
	if (exposure >= EXPOSUREMAX)exposure = EXPOSUREMAX;
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(Position, Position + Front, Up);
}

float Camera::GetFOV() const {
	return fov;
}

float Camera::GetExposure() const {
	return exposure;
}