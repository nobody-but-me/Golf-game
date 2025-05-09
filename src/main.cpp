
#include <iostream>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/core.hpp"

int main(int argc, char *argv[]) {
    Core::Application engine("Golfine << DEBUG");
    
    while (!glfwWindowShouldClose(engine.getWindow())) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.024f, 0.024f, 0.024f, 1.0f);
	
	glfwSwapBuffers(engine.getWindow());
	glfwPollEvents();
    }
    return 0;
}
