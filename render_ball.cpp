#include "render_ball.h"
#include <stdio.h>
#include <fstream>
#include <vector>
#include <filesystem>
#include "Sphere.h"
#include "glhelper.h"


bool GLBase::GLInit()
{
	glfwSetErrorCallback(glfw_error_callback);

	if (glfwInit() == GLFW_FALSE)
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	

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

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	return true;
}


#define SQRT(x) std::sqrt(x)

Renderer::Renderer()
{
	glEnable(GL_DEPTH_TEST);

	InitBall();
	InitGround();


	/*
	 *
	 * init m v p transformation
	 *
	 */
	mMat = glm::mat4(1.0f);
	//mMat = glm::scale(mMat, glm::vec3(0.5,0.5,0.5));

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	auto aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}



void Renderer::InitBall()
{
	auto mySphere = Sphere(48);
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
	glGenVertexArrays(1, &vao_sphere);
	glBindVertexArray(vao_sphere);
	glGenBuffers(3, vbo_sphere);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[0]);
	glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[1]);
	glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere[2]);
	glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	texture_sphere = LoadTexture((CurrentPath().parent_path().string() + std::string("\\resource\\earth.jpg")).c_str());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_sphere);


	std::filesystem::path build_path = std::filesystem::current_path();
	std::filesystem::path shader_dir = build_path.parent_path().string() + ("\\shader");

	std::string vertex_shader = ReadShaderFile(std::string(shader_dir.string() + std::string("\\sphere_vert_shader.glsl")).c_str());
	std::string fragment_shader = ReadShaderFile(std::string(shader_dir.string() + std::string("\\sphere_frag_shader.glsl")).c_str());

	shader_program_sphere = CreateShaderProgram(vertex_shader, fragment_shader);

	glUseProgram(shader_program_sphere);
}

void Renderer::RenderBall(glm::vec3 new_x)
{
	glUseProgram(shader_program_sphere);

	auto mv_loc = glGetUniformLocation(shader_program_sphere, "mv_matrix");
	auto proj_loc = glGetUniformLocation(shader_program_sphere, "proj_matrix");

	mMat = glm::rotate(mMat, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMat = glm::translate(glm::mat4(1.0), new_x);

	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(vMat * mMat));
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindVertexArray(vao_sphere);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	static unsigned int triangle_size = Sphere(48).getNumIndices();
	glDrawArrays(GL_TRIANGLES, 0, triangle_size);
}

void Renderer::InitGround()
{
	float c = 1000;
	float height = -100.0f;

	float ground_position[] = {
		-c, height,c,
		c,height,c,
		c,height,-c,
		-c,height,-c
	};
	float tvalue[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,
	};
	unsigned int vert_index[] = {
		0,1,2,
		2,3,0
	};

	glGenVertexArrays(1, &vao_ground);
	glBindVertexArray(vao_ground);
	glGenBuffers(2, vbo_ground);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_ground[0]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), ground_position, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_ground[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), tvalue, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), vert_index, GL_STATIC_DRAW);



	texture_ground = LoadTexture((CurrentPath().parent_path().string() + std::string("\\resource\\earth.jpg")).c_str());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_ground);


	std::filesystem::path build_path = std::filesystem::current_path();
	std::filesystem::path shader_dir = build_path.parent_path().string() + ("\\shader");

	std::string vertex_shader = ReadShaderFile(std::string(shader_dir.string() + std::string("\\ground_vert_shader.glsl")).c_str());
	std::string fragment_shader = ReadShaderFile(std::string(shader_dir.string() + std::string("\\ground_frag_shader.glsl")).c_str());

	shader_program_ground = CreateShaderProgram(vertex_shader, fragment_shader);

	glUseProgram(shader_program_ground);
}

void Renderer::RenderGround()
{
	glUseProgram(shader_program_ground);

	auto mv_loc = glGetUniformLocation(shader_program_ground, "mv_matrix");
	auto proj_loc = glGetUniformLocation(shader_program_ground, "proj_matrix");

	auto m_mat_ground = glm::mat4(1.0);

	glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(vMat * m_mat_ground));
	glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindVertexArray(vao_ground);

	//glDisable(GL_CULL_FACE);
	//glDisable(GL_CCW);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

bool Renderer::Render(glm::vec3 new_x)
{
	if (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		RenderBall(new_x);
		RenderGround();

		glfwSwapBuffers(window);
		glfwPollEvents();
		return true;
	}
	return false;
}