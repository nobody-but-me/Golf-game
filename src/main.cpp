
#include <iostream>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/core.hpp"

#include "../include/game.hpp"
#include "../include/molson.h"


int main(int argc, char *argv[]) {
    Core::Application engine("Golfine << DEBUG");
    Game::set_application(&engine);
    
    // TODO: change the place of this delta time logic.
    double last_time = glfwGetTime();
    double delta = 0.0f;
    
    glfwSetKeyCallback(engine.get_window(), Game::input);
    Game::ready();
    while (!glfwWindowShouldClose(engine.get_window())) {
	if (engine.is_key_pressed(GOLF_ESCAPE)) {
	    break;
	}
	glfwPollEvents();
	// glClearColor(0.074f, 0.074f, 0.074f, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// int w, h;
	// glfwGetWindowSize(engine.get_window(), &w, &h);
	// glEnable(GL_SCISSOR_TEST);
	// glScissor(w / 2 - (1064 / 2), h / 2 - (600 / 2), 1064, 600);
	// glClearColor(159.0f / 255, 218.0f / 255, 242.0f / 255, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT);
	// glDisable(GL_SCISSOR_TEST);
	
	// int count;
	// const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	// std::cout << *axes << " :: " << count << std::endl;
	
	Game::process(delta);
	
	delta = glfwGetTime() - last_time;
	last_time = glfwGetTime();
	
	Game::render();
	
	glfwSwapBuffers(engine.get_window());
    }
    Game::destroy();
    return 0;
}
