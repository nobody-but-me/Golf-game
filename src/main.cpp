
#include <iostream>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/game.hpp"
#include "../include/core.hpp"

#include "../include/molson.h"

#define TAU (3.1415 * 2.0)

int main(int argc, char *argv[]) {
    Core::Application engine("Golfine << DEBUG");
    Game::setApplication(&engine);
    
    // TODO: change the place of this delta time logic.
    double last_time = glfwGetTime();
    double delta = 0;
    
    glfwSetKeyCallback(engine.getWindow(), Game::input);
    Game::ready();
    // Molson(getImageRGBValues)("./assets/color-image-test.png");
    while (!glfwWindowShouldClose(engine.getWindow())) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.024f, 0.024f, 0.024f, 1.0f);
	if (engine.isKeyPressed(GOLF_ESCAPE)) {
	    break;
	}
	
	delta = glfwGetTime() - last_time;
	last_time = glfwGetTime();
	Game::process(delta);
	
	Game::render();
	
	// if (glfwGetTime() - timer > 1.0) {
	//     timer++;
	//     std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
	//     updates = 0;
	//     frames = 0;
	// }
	glfwSwapBuffers(engine.getWindow());
	glfwPollEvents();
    }
    Game::destroy();
    return 0;
}
