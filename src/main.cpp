
#include <iostream>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/game.hpp"
#include "../include/core.hpp"

#include "../include/molson.h"


int main(int argc, char *argv[]) {
    Core::Application engine("Golfine << DEBUG");
    Game::setApplication(&engine);
    
    Game::ready();
    while (!glfwWindowShouldClose(engine.getWindow())) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.024f, 0.024f, 0.024f, 1.0f);
	if (engine.isKeyPressed(GOLF_ESCAPE)) {
	    break;
	}
	Game::process((float)glfwGetTime());
	
	glfwSwapBuffers(engine.getWindow());
	glfwPollEvents();
    }
    Game::destroy();
    return 0;
}
