
#include <iostream>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/objects.hpp"
#include "../include/core.hpp"

#include "../include/molson.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

int main(int argc, char *argv[]) {
    Core::Application engine("Golfine << DEBUG");
    
    Objects::Sprite sprite("Sprite1", "texture_path");
    sprite.self.color = glm::vec3(0.0f, 1.0f, 0.0f);
    sprite.self.position = glm::vec2(0.0f, 0.0f);
    sprite.self.scale = glm::vec2(1.0f, 1.0f);
    
    while (!glfwWindowShouldClose(engine.getWindow())) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.024f, 0.024f, 0.024f, 1.0f);
	
	sprite.render(engine.getMainShader());
	sprite.self.rotation = (float)glfwGetTime() * 100.0f;
	
	glfwSwapBuffers(engine.getWindow());
	glfwPollEvents();
    }
    return 0;
}
