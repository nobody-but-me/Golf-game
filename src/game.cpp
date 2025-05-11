

#include <iostream>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#define PHYSICS_IMPLEMENTATION
#include "../include/physics.hpp"
#define MATH_IMPLEMENTATION
#include "../include/math.hpp"

#include "../include/objects.hpp"
#include "../include/game.hpp"
#include "../include/core.hpp"

#include <stdlib.h>

namespace Game {
    
    Core::Application *engine;
    void setApplication(Core::Application *p_engine) {
	engine = p_engine;
	return;
    }
    
    Objects::Rectangle *player;
    Objects::Rectangle *ground;
    Objects::Rectangle *wall;
    
    void ready() {
	player = new Objects::Rectangle("Player");
	ground = new Objects::Rectangle("Ground");
	wall   = new Objects::Rectangle("Wall");
	
	// TODO: hardcoded.
	player->self.position = glm::vec3(-2.0f, -2.0f, 0.0f);
	player->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player->self.color = glm::vec3(240.0f, 58.0f, 46.0f);
	player->self.scale = glm::vec2(3.0f, 3.0f);
	
	ground->self.position = glm::vec3(0.0f - *engine->getWindowHeight() / 2, -10.0f, 0.0f);
	ground->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	ground->self.color = glm::vec3(42.0f, 93.0f, 232.0f);
	ground->self.scale = glm::vec2(*engine->getWindowHeight(), 3.0f);
	
	wall->self.position = glm::vec3(8.0f, -8.0f, 0.0f);
	wall->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	wall->self.color = glm::vec3(42.0f, 93.0f, 232.0f);
	wall->self.scale = glm::vec2(3.0f, *engine->getWindowWidth());
	return;
    }
    
    const float ACCELERATION = 0.2f;
    const float FRICTION = 0.07f;
    const float GRAVITY = 0.002f;
    const float SPEED = 0.2f;
    
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
    
    static void move(double delta) {
	float direction = 0.0f;
	if (engine->isKeyPressed(GOLF_D)) {
	    direction = 1.0f;
	}
	else if (engine->isKeyPressed(GOLF_A)) {
	    direction = -1.0f;
	}
	
	if (direction >= 0.5f || direction < -0.5f) {
	    velocity.x = Math::lerp(velocity.x, direction * SPEED, ACCELERATION);
	} else {
	    velocity.x = Math::lerp(velocity.x, 0.0f, FRICTION);
	}
	return;
    }
    
    void process(double delta) {
	move(delta);
	
	player->self.position.x += velocity.x * delta;
	player->self.position.y += velocity.y;
	return;
    }
    void render() {
	player->render(engine->getMainShader());
	ground->render(engine->getMainShader());
	wall->render(engine->getMainShader());
    }
    
    void destroy() {
	delete player;
	delete ground;
	delete wall;
    }
}
