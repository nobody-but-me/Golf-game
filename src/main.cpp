
#include <iostream>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/core.hpp"

#include "../include/editor.hpp"
#include "../include/game.hpp"
#include "../include/molson.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

int main(int argc, char *argv[]) {
    Core::Application engine("Golfine << DEBUG");
    
    Editor::setApplication(&engine);
    Game::setApplication(&engine);
    
    // TODO: change the place of this delta time logic.
    double last_time = glfwGetTime();
    double delta = 0.0f;
    
    glfwSetKeyCallback(engine.getWindow(), Game::input);
    Game::ready();
    while (!glfwWindowShouldClose(engine.getWindow())) {
	if (engine.isKeyPressed(GOLF_ESCAPE)) {
	    break;
	}
	glfwPollEvents();
	glClearColor(0.024f, 0.024f, 0.024f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	delta = glfwGetTime() - last_time;
	last_time = glfwGetTime();
	
	Game::process(delta);
	Game::render();
	Editor::process(delta);
	
	glfwSwapBuffers(engine.getWindow());
    }
    Game::destroy();
    return 0;
}
