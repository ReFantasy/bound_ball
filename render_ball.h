#ifndef __RENDER_BALL_H__
#define __RENDER_BALL_H__
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "Sphere.h"


class GLBase
{
public:
	GLBase()
	{
		GLInit();
	}
	virtual ~GLBase()
	{
		glfwTerminate();
	}

	virtual bool Render(glm::vec3) = 0;

	GLFWwindow* window = nullptr;

private:
	bool GLInit();


};

class Ball:public GLBase
{
public:
	Ball();
	virtual ~Ball()
	{
		glDeleteProgram(shader_program_sphere);
	}

	virtual bool Render(glm::vec3);

private:
	unsigned int vao_sphere;
	unsigned int vbo_sphere[3];
	unsigned int shader_program_sphere;

private:
	float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 14.0f;
	glm::mat4 pMat, vMat, mMat;
	int triangle_size = 0;
};



#endif//__RENDER_BALL_H__
