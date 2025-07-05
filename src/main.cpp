
#include <iostream>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/core.hpp"

#include "../include/game.hpp"
#include "../include/molson.h"


int main(int argc, char *argv[]) {
    Core::Application engine("Golfine -- or dolphine? -- << DEBUG");
    Game::set_application(&engine);
    
    // TODO: change the place of this delta time logic.
    double last_time = glfwGetTime();
    double delta = 0.0f;
    
    glfwSetKeyCallback(engine.get_window(), Game::input);
    Game::ready();
    while (!glfwWindowShouldClose(engine.get_window())) {
	if (Core::Input::is_key_pressed(engine.get_window(), GOLF_ESCAPE)) {
	    break;
	}
	glfwPollEvents();
	// glClearColor(0.074f, 0.074f, 0.074f, 1.0f);
	glClearColor(0.024f, 0.024f, 0.024f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	int w, h;
	glfwGetWindowSize(engine.get_window(), &w, &h);
	glEnable(GL_SCISSOR_TEST);
	glScissor(w / 2 - (*engine.get_window_width() / 2), h / 2 - (*engine.get_window_height() / 2), *engine.get_window_width(), *engine.get_window_height());
	// glClearColor(159.0f / 255, 218.0f / 255, 242.0f / 255, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	
	delta = glfwGetTime() - last_time;
	last_time = glfwGetTime();
	
	Game::process(delta);
	Game::render();
	
	glfwSwapBuffers(engine.get_window());
    }
    Game::destroy();
    return 0;
}
