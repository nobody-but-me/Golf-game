
#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include <GLFW/glfw3.h>
#include "./core.hpp"

namespace Editor {
    void setApplication(Core::Application *p_engine);

    void process(float delta);
    void destroy();
    void init(const char *p_glsl_version, GLFWwindow *p_window);
}

#endif//EDITOR_H
#ifdef  EDITOR_IMPLEMENTATION

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

    static glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    static glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 1.0f);
    static float camera_position[] = {0.0f, 0.0f, 50.0f};
    static int sprite_index = 0;
    Core::Application *engine;
    glm::mat4 view;

    void setApplication(Core::Application *p_engine) {
        engine = p_engine;
    }

    void process(float delta) {
	    ImGui_ImplOpenGL3_NewFrame();
	    ImGui_ImplGlfw_NewFrame();
	    ImGui::NewFrame();

	    ImGui::Begin("Camera");

	    ImGui::SliderFloat3("Position", camera_position, -90.0f, 90.0f);
	    view = glm::lookAt(glm::vec3(camera_position[0], camera_position[1], camera_position[2]), glm::vec3(camera_position[0], camera_position[1], camera_position[2]) + camera_front, camera_up);
	    Molson(_set_matrix4)("view", &view, true, engine->getMainShader());

	    ImGui::End();

	    ImGui::Begin("Player");

	    ImGui::SliderFloat("Position X", &Game::getPlayerHitbox()->self.position.x, -200.0f, 200.0f);
	    ImGui::SliderFloat("Position Y", &Game::getPlayerHitbox()->self.position.y, -200.0f, 200.0f);
	    ImGui::SliderFloat("Position Z", &Game::getPlayerHitbox()->self.position.z, -200.0f, 200.0f);

	    ImGui::SliderInt("Sprite Index", &sprite_index, 0, 35);

	    ImGui::End();
	    Molson(_set_int)("time", sprite_index, true, engine->getMainShader());

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
        view = *engine->getView();

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
