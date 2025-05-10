

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
    
    // TODO: put it somewhere else. That's not the right place for such function.
    static inline float lerp(float a, float b, float t) {
	return a * (1.0 - t) + (b * t);
    }
    
    const float ACCELERATION = 0.2f;
    const float FRICTION = 0.07f;
    const float SPEED = 0.2f;
    
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
    static void move() {
	float direction = 0.0f;
	if (engine->isKeyPressed(GOLF_D)) {
	    direction = 1.0f;
	}
	else if (engine->isKeyPressed(GOLF_A)) {
	    direction = -1.0f;
	}
	
	if (direction >= 0.5f || direction < -0.5f) {
	    velocity.x = lerp(velocity.x, direction * SPEED, ACCELERATION);
	} else {
	    velocity.x = lerp(velocity.x, 0.0f, FRICTION);
	}
	return;
    }
    
    void process(double delta) {
	move();
	player->self.position.x += velocity.x * delta;
	player->self.position.y += velocity.y * delta;
	return;
    }
    void render() {
	player->render(engine->getMainShader());
	ground->render(engine->getMainShader());
	player->isCollidingRect(ground, true);
    }
    
    void destroy() {
	delete player;
	delete ground;
    }
}
