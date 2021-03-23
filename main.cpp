#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(800,600,"LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    std::cout<<glfwGetVersionString()<<std::endl;

    if(glewInit()!=GLEW_OK)
    {
        std::cout<<"error"<<std::endl;
    }

    float positions[] = {
            -0.5f,0.0f,
            0.5f,0.0f,
            0.0f, 0.5f
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f,-0.5f);
        glVertex2f(0.5f,-0.5f);
        glVertex2f(0.0f,0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

