#ifndef __RENDER_BALL_H__
#define __RENDER_BALL_H__
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

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

	virtual bool Render() = 0;

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
		glDeleteProgram(shader_program);
	}

	virtual bool Render();

private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int shader_program;

private:
	float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 0.0f;
	glm::mat4 pMat, vMat, mMat;

};

std::string ReadShaderFile(const char* file_path);
unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
int CompileShader(unsigned int type, const std::string& source);
void glfw_error_callback(int error, const char* description);

#endif//__RENDER_BALL_H__
