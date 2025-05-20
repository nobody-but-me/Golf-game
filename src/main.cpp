
#include <iostream>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/game.hpp"
#include "../include/core.hpp"
#include "../include/molson.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#define TAU (3.1415 * 2.0)

int main(int argc, char *argv[]) {
    Core::Application engine("Golfine << DEBUG");
    Game::setApplication(&engine);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(engine.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();
    
    // TODO: change the place of this delta time logic.
    double last_time = glfwGetTime();
    double delta = 0;
    
    glfwSetKeyCallback(engine.getWindow(), Game::input);
    Game::ready();
    // Molson(getImageRGBValues)("./assets/color-image-test.png");
    while (!glfwWindowShouldClose(engine.getWindow())) {
	if (engine.isKeyPressed(GOLF_ESCAPE)) {
	    break;
	}
	glfwPollEvents();
	glClearColor(0.024f, 0.024f, 0.024f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	delta = glfwGetTime() - last_time;
	last_time = glfwGetTime();
	
	Game::process(delta);
	
	ImGui::Begin("Hello Window");
	ImGui::Button("Hello!");
	ImGui::End();
	
	Game::render();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	glfwSwapBuffers(engine.getWindow());
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    Game::destroy();
    return 0;
}
