#pragma once
#include <glm.hpp>
#ifndef __CAMERA2D_H_
#define __CAMERA2D_H_

class Camera2D
{
public:
	glm::vec3 position;
	glm::vec3 orientation = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);

	int width;
	int height;

	float speed = 0.1f;
	float sensitivity = 0.1f;
};


#endif // __CAMERA2D_H_
