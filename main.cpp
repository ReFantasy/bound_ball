#include <iostream>
#include "render_ball.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


float cameraX, cameraY, cameraZ;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat;


int main()
{
	Ball ball;

	ball.Render();

	std::cout << "ok." << std::endl;

	return 0;
}

