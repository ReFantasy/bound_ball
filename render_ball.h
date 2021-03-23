#ifndef __RENDER_BALL_H__
#define __RENDER_BALL_H__
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

std::string ReadShaderFile(const char* file_path);
unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
int CompileShader(unsigned int type, const std::string& source);
void glfw_error_callback(int error, const char* description);

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

	virtual void Render() = 0;

private:
	bool GLInit();

protected:
	GLFWwindow* window = nullptr;

};

class Ball:public GLBase
{
public:
	Ball()
	{
		
	}
	virtual ~Ball()
	{

	}

	virtual void Render()
	{
		
	}

private:

};


#endif//__RENDER_BALL_H__