
#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include <vector>

#include <GLFW/glfw3.h>
#include "./core.hpp"

namespace Editor {
    void set_application(Core::Application *p_engine);

    void process(float delta);
    void destroy();
    void init(const char *p_glsl_version, GLFWwindow *p_window);
}

#endif//EDITOR_H
#ifdef  EDITOR_IMPLEMENTATION

#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>

#include "./game.hpp"
#include "./core.hpp"
#include "./molson.h"

namespace Editor {
    
    Core::Application *engine;
    
    std::vector<float> player_float_variables;
    std::vector<int>   player_int_variables;
    
    void set_application(Core::Application *p_engine) {
        engine = p_engine;
    }
    
    void process(float delta) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::Begin("Player");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        return;
    }

    void destroy() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
        return;
    }

    void init(const char *p_glsl_version, GLFWwindow *p_window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(p_window, true);
	ImGui_ImplOpenGL3_Init(p_glsl_version);
	ImGui::StyleColorsDark();
        return;
    }
}

#endif//EDITOR_IMPLEMENTATION
