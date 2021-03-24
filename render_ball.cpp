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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

	return true;
}


#define SQRT(x) std::sqrt(x)

Ball::Ball()
{
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
	

	mMat = glm::mat4(1.0f);
	//mMat = glm::scale(mMat, glm::vec3(0.5,0.5,0.5));

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	auto aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);



	std::filesystem::path build_path = std::filesystem::current_path();
	std::filesystem::path shader_dir = build_path.parent_path().string() + ("\\shader");

	std::string vertex_shader = ReadShaderFile(std::string( shader_dir.string()+std::string( "\\vertShader.glsl")).c_str());
	std::string fragment_shader = ReadShaderFile(std::string(shader_dir.string() + std::string("\\fragShader.glsl")).c_str());

	shader_program = CreateShaderProgram(vertex_shader, fragment_shader);

	glUseProgram(shader_program);
}

bool Ball::Render(glm::vec3 new_x)
{
	if (!glfwWindowShouldClose(window))
	{

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);

		auto mv_loc = glGetUniformLocation(shader_program, "mv_matrix");
		auto proj_loc = glGetUniformLocation(shader_program, "proj_matrix");

		mMat = glm::rotate(mMat, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		mMat = glm::translate(glm::mat4(1.0), new_x);

		glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(vMat * mMat));
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(pMat));

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

