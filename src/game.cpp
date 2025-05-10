

#include <iostream>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "../include/objects.hpp"
#include "../include/game.hpp"
#include "../include/core.hpp"

namespace Game {
    
    Core::Application *engine;
    void setApplication(Core::Application *p_engine) {
	engine = p_engine;
	return;
    }
    
    Objects::Rectangle *player;
    Objects::Rectangle *ground;
    void ready() {
	player = new Objects::Rectangle("Player");
	ground = new Objects::Rectangle("Ground");
	
	player->self.position = glm::vec3(-2.0f, -2.0f, 0.0f);
	player->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player->self.color = glm::vec3(240.0f, 58.0f, 46.0f);
	player->self.scale = glm::vec2(1.0f, 2.0f) * 3.0f;
	
	ground->self.position = glm::vec3(0.0f - *engine->getWindowHeight() / 2, -10.0f, 0.0f);
	ground->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ground->self.color = glm::vec3(42.0f, 93.0f, 232.0f);
	ground->self.scale = glm::vec2(*engine->getWindowHeight(), 3.0f);
	return;
    }
    
    void process(float delta) {
	player->render(engine->getMainShader());
	ground->render(engine->getMainShader());
	
	if (engine->isKeyPressed(GOLF_D)) {
	    player->self.position[0] += 0.5f;
	}
	else if (engine->isKeyPressed(GOLF_A)) {
	    player->self.position[0] -= 0.5f;
	}
	if (engine->isKeyPressed(GOLF_W)) {
	    player->self.position[1] += 0.5f;
	}
	else if (engine->isKeyPressed(GOLF_S)) {
	    player->self.position[1] -= 0.5f;
	}
	player->isCollidingRect(ground, true);
	
	return;
    }
    
    void destroy() {
	delete player;
	delete ground;
    }
}
