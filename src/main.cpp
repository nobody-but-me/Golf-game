
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

#include "../include/nlohmann/json_fwd.hpp"
#include "../include/nlohmann/json.hpp"
#include <fstream>

// static int sprite_index_delay = 0;
// static int sprite_index = 0;

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    Core::Application engine("Golfine << DEBUG");

    Editor::setApplication(&engine);
    Game::setApplication(&engine);
    
    // TODO: change the place of this delta time logic.
    double last_time = glfwGetTime();
    double delta = 0;
    
    glfwSetKeyCallback(engine.getWindow(), Game::input);
    Game::ready();
    
    std::ifstream f("./assets/player/sprite_sheet.json");
    json data = json::parse(f);
    std::cout << data.dump(4) << std::endl;
    
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

    // ---------------for purposes of test---------------
    // if (sprite_index_delay < 5) {
    //     sprite_index_delay++;
    // } else {
	//     if (sprite_index < 35) {
    //         std::cout << sprite_index << std::endl;
	//         sprite_index++;
	//     } else {
	//         sprite_index = 0;
	//     }
    //     sprite_index_delay = 0;
    // }
	// Molson(_set_int)("time", sprite_index, true, engine.getMainShader());
	// --------------------------------------------------

	glfwSwapBuffers(engine.getWindow());
    }
    Game::destroy();
    return 0;
}
