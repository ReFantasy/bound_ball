#include "render_ball.h"
#include <stdio.h>
#include <fstream>


void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

std::string ReadShaderFile(const char* file_path)
{
	std::string content;
	std::ifstream fileStream(file_path, std::ios::in);
	std::string line;
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;

}

unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int shader_id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader_id, 1, &src, nullptr);
	glCompileShader(shader_id);

	// Error handling
	int result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length] {};
		glGetShaderInfoLog(shader_id, length, &length, message);
		std::cout << "Failed to compile "
			<< ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(shader_id);
		delete[]message;
		return -1;
	}


	return shader_id;

}

bool GLBase::GLInit()
{
	glfwSetErrorCallback(glfw_error_callback);

	if (glfwInit() == GLFW_FALSE)
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(1280, 960, "Bounding Ball", nullptr, nullptr);
	if (window == NULL)
	{
		glfwTerminate();
		return false;
	}
		

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	return true;
}



Ball::Ball()
{
	float vertexPositions[9] =
	{
		-0.5f,-0.5f,0.0f,
		 0.5f,-0.5f,0.0f,
		 0.0f, 0.5f,0.0f
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);


	std::string vertex_shader = ReadShaderFile("F:/Simulation/bound_ball/vertShader.glsl");
	std::string fragment_shader = ReadShaderFile("F:/Simulation/bound_ball/fragShader.glsl");

	shader_program = CreateShaderProgram(vertex_shader, fragment_shader);

	glUseProgram(shader_program);

	// m v p
	mMat = glm::mat4(1.0f);
	

	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 6.0f;
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	auto aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void Ball::Render()
{
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		auto mv_loc = glGetUniformLocation(shader_program, "mv_matrix");
		auto proj_loc = glGetUniformLocation(shader_program, "proj_matrix");
		glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(vMat * mMat));
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(pMat));

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
}
