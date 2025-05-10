
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
    
    Objects::Sprite sprite("Sprite1", "./assets/m.png", true);
    sprite.self.color = glm::vec3(1.0f, 1.0f, 1.0f);
    sprite.self.position = glm::vec3(0.0f, 0.0f, 0.0f);
    sprite.self.scale = glm::vec2(2.0f, 2.0f);
    sprite.self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    
    Objects::Rectangle rect("Rectangle1");
    rect.self.color = glm::vec3(1.0f, 0.0f, 0.0f);
    rect.self.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    rect.self.scale = glm::vec2(1.0f, 1.0f);
    // rect.self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    
    while (!glfwWindowShouldClose(engine.getWindow())) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.024f, 0.024f, 0.024f, 1.0f);
	
	sprite.render(engine.getMainShader());
	rect.render(engine.getMainShader());
	// sprite.self.rotation[0] = (float)glfwGetTime() * 50.0f;
	// sprite.self.rotation[2] = (float)glfwGetTime() * 50.0f;
	
	if (engine.isKeyPressed(GLFW_KEY_D)) {
	    sprite.self.position[0] += 0.05f;
	}
	else if (engine.isKeyPressed(GLFW_KEY_A)) {
	    sprite.self.position[0] -= 0.05f;
	}
	if (engine.isKeyPressed(GLFW_KEY_W)) {
	    sprite.self.position[1] += 0.05f;
	}
	else if (engine.isKeyPressed(GLFW_KEY_S)) {
	    sprite.self.position[1] -= 0.05f;
	}
	sprite.isCollidingRect(&rect, true);
	
	glfwSwapBuffers(engine.getWindow());
	glfwPollEvents();
    }
    return 0;
}
