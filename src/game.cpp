

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
	engine->buildLevel("./assets/test-level.png");
	
	player = new Objects::Rectangle("Player", false);
	ground = new Objects::Rectangle("Ground", false);
	wall   = new Objects::Rectangle("Wall", false);
	
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
    const int MAX_JUMP_QUANTITY = 3;
    const float JUMP_FORCE = 0.4f;
    const float FRICTION = 0.07f;
    const float SPEED = 0.26f;
    
    const float NORMAL_GRAVITY = 0.008f;
    const float WALL_GRAVITY = 0.0008f;
    
    float GRAVITY = NORMAL_GRAVITY;
    
    int jump_number = 1;
    
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
    static void move(double delta) {
	float direction = 0.0f;
	if (engine->isKeyPressed(GOLF_D)) {
	    direction = 1.0f;
	}
	else if (engine->isKeyPressed(GOLF_A)) {
	    direction = -1.0f;
	}
	
	// TODO: find a better and more precise way to do that.
	Objects::Rectangle forecast_player("forecastPlayer", false);
	forecast_player.self.position = player->self.position;
	forecast_player.self.scale = player->self.scale;
	float forecast_velocity;
	
	if (direction == 1.0f || direction == -1.0f) {
	    forecast_velocity = Math::lerp(velocity.x, SPEED * direction, ACCELERATION);
	} else if (direction == 0.0f) {
	    forecast_velocity = Math::lerp(velocity.x, 0.0f, FRICTION);
	}
	forecast_player.self.position.x += forecast_velocity * delta;
	// found out this method. It works, but for sure has problems. That's the X-axis move and collide system.
	if (Physics::isColliding(&forecast_player, wall)) {
	    if (velocity.x > 0.0f) {
		player->self.position.x = wall->self.position.x - wall->self.scale.x;
		velocity.x = 0.0f;
		
		// wall jump;
		if (!Physics::isRectOnFloor(player, ground)) {
		    velocity.y = 0.0f;
		    GRAVITY = WALL_GRAVITY;
		    velocity.y -= GRAVITY * delta;
		    if (jump_number == MAX_JUMP_QUANTITY) {
			jump_number = 2;
		    }
		    return;
		}
	    } else if (velocity.x < 0.0f) {
		player->self.position.x = wall->self.position.x + wall->self.scale.x;
		velocity.x = 0.0f;
	    }
	    return;
	} else {
	    GRAVITY = NORMAL_GRAVITY;
	}
	velocity.x = forecast_velocity;
	return;
    }
    
    void process(double delta) {
	move(delta);
	player->self.position.x += velocity.x * delta;
	player->self.position.y += velocity.y;
	
	// this reminds me of the game maker times...
	if (!Physics::isRectOnFloor(player, ground)){
	    velocity.y -= GRAVITY * delta;
	} else {
	    jump_number = 1;
	    
	    player->self.position.y = ground->self.position.y + ground->self.scale.y;
	    velocity.y = 0.0f;
	    return;
	}
	return;
    }
    void input(GLFWwindow *p_window, int key, int scancode, int action, int mods) {
	if (key == GOLF_UP && action == GLFW_PRESS) {
	    if (jump_number < MAX_JUMP_QUANTITY) {
		GRAVITY = NORMAL_GRAVITY;
		jump_number++;
		velocity.y = JUMP_FORCE * jump_number / 2;
	    }
	}
	return;
    }
    
    void render() {
	engine->renderLevel(engine->getMainShader());
	
	player->render(engine->getMainShader());
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
