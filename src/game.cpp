

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
    
    Objects::Rectangle *rect;
    Objects::Sprite *sprite;
    void ready() {
	sprite = new Objects::Sprite("Sprite1", "./assets/m.png", true);
	rect = new Objects::Rectangle("Rectangle1");
	sprite->self.color = glm::vec3(1.0f, 1.0f, 1.0f);
	sprite->self.position = glm::vec3(0.0f, 0.0f, 0.0f);
	sprite->self.scale = glm::vec2(2.0f, 2.0f);
	sprite->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	
	rect->self.color = glm::vec3(1.0f, 0.0f, 0.0f);
	rect->self.position = glm::vec3(-1.0f, -1.0f, 0.0f);
	rect->self.scale = glm::vec2(1.0f, 1.0f);
	
	std::cout << "Hello Golf Game." << std::endl;
	return;
    }
    
    void process(float delta) {
	sprite->render(engine->getMainShader());
	rect->render(engine->getMainShader());
	sprite->self.rotation[0] = delta * 50.0f;
	sprite->self.rotation[2] = delta * 50.0f;
	
	if (engine->isKeyPressed(GOLF_D)) {
	    sprite->self.position[0] += 0.05f;
	}
	else if (engine->isKeyPressed(GOLF_A)) {
	    sprite->self.position[0] -= 0.05f;
	}
	if (engine->isKeyPressed(GOLF_W)) {
	    sprite->self.position[1] += 0.05f;
	}
	else if (engine->isKeyPressed(GOLF_S)) {
	    sprite->self.position[1] -= 0.05f;
	}
	sprite->isCollidingRect(rect, true);
	
	return;
    }
    
    void destroy() {
	delete sprite;
	delete rect;
    }
}
