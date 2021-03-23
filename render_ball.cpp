#include "render_ball.h"
#include <stdio.h>
#include <fstream>
#include <vector>
#include "Sphere.h"


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


#define SQRT(x) std::sqrt(x)

Ball::Ball()
{
	// 创建球模型顶点数据
	auto mySphere = Sphere(48);
	triangle_size = mySphere.getNumIndices();
	std::vector<int> ind = mySphere.getIndices();
	std::vector<glm::vec3> vert = mySphere.getVertices();
	std::vector<glm::vec2> tex = mySphere.getTexCoords();
	std::vector<glm::vec3> norm = mySphere.getNormals();

	std::vector<float> pvalues;
	std::vector<float> tvalues;
	std::vector<float> nvalues;

	int numIndices = mySphere.getNumIndices();
	for (int i = 0; i < numIndices; i++) {
		pvalues.push_back((vert[ind[i]]).x);
		pvalues.push_back((vert[ind[i]]).y);
		pvalues.push_back((vert[ind[i]]).z);
		tvalues.push_back((tex[ind[i]]).s);
		tvalues.push_back((tex[ind[i]]).t);
		nvalues.push_back((norm[ind[i]]).x);
		nvalues.push_back((norm[ind[i]]).y);
		nvalues.push_back((norm[ind[i]]).z);
	}
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(3, vbo);

	// 绑定顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTexture);*/
	
	// 初始化 m v p 矩阵
	mMat = glm::mat4(1.0f);
	mMat = glm::scale(mMat, glm::vec3(0.5,0.5,0.5));

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	auto aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);



	// 加载着色器
	std::string vertex_shader = ReadShaderFile("F:/Simulation/bound_ball/vertShader.glsl");
	std::string fragment_shader = ReadShaderFile("F:/Simulation/bound_ball/fragShader.glsl");

	shader_program = CreateShaderProgram(vertex_shader, fragment_shader);

	glUseProgram(shader_program);
}

bool Ball::Render()
{
	if (!glfwWindowShouldClose(window))
	{
		// 清除缓存
		glClear(GL_COLOR_BUFFER_BIT);

		// 使用着色器
		glUseProgram(shader_program);

		// 设置模型变换矩阵
		auto mv_loc = glGetUniformLocation(shader_program, "mv_matrix");
		auto proj_loc = glGetUniformLocation(shader_program, "proj_matrix");
		mMat = glm::rotate(mMat, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(vMat * mMat));
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(pMat));

		// 绘制模型
		glBindVertexArray(vao);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDrawArrays(GL_TRIANGLES, 0, triangle_size);

		glfwSwapBuffers(window);
		glfwPollEvents();
		return true;
	}
	return false;
}

