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
	~Ball()
	{

	}

	void Render()
	{
		//while (!glfwWindowShouldClose(window))
		//{
		//	/* Render here */
		//	glClear(GL_COLOR_BUFFER_BIT);

		//	glUseProgram(shader_program);
		//	glBindVertexArray(vao);

		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		//	//glUniform4f(location, RandomNumber(0.0f, 1.0f), 1.0f, 0.0f, 1.0f);

		//	/* Swap front and back buffers */
		//	glfwSwapBuffers(window);

		//	/* Poll for and process events */
		//	glfwPollEvents();
		//}
	}
public:
	/*unsigned int vao;
	unsigned int buffer;
	unsigned int ibo;
	unsigned int shader_program;*/
};


#endif//__RENDER_BALL_H__