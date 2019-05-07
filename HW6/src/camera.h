#ifndef Camera_h
#define Camera_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		GLfloat pitch = 0.0f,
		GLfloat yaw = -90.0f
	) :
		cameraPos(cameraPos),
		cameraFront(cameraFront),
		cameraUp(cameraUp),
		worldUp(worldUp),
		pitch(pitch),
		yaw(yaw)
	{}
	void moveForward(GLfloat const distance)
	{
		cameraPos += distance * cameraFront;
	}
	void moveBack(GLfloat const distance)
	{
		cameraPos -= distance * cameraFront;
	}
	void moveRight(GLfloat const distance)
	{
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
		cameraPos += distance * cameraRight;
	}
	void moveLeft(GLfloat const distance)
	{
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
		cameraPos -= distance * cameraRight;
	}
	void rotate(GLfloat const x, GLfloat const y)
	{
		pitch += y;
		yaw -= x;
		pitch = pitch > 89.0f ? 89.0f : pitch;
		pitch = pitch < -89.0f ? -89.0f : pitch;
		cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		cameraFront.y = sin(glm::radians(pitch));
		cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(cameraFront);
		glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
		cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));
	}
	glm::vec3 getCameraPos()
	{
		return cameraPos;
	}
	glm::mat4 getView()
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 worldUp;
	GLfloat pitch;
	GLfloat yaw;
};

#endif
