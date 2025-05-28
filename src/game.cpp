

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#define ANIMATOR_IMPLEMENTATION
#include "../include/animator.hpp"
#define PHYSICS_IMPLEMENTATION
#include "../include/physics.hpp"
#define MATH_IMPLEMENTATION
#include "../include/math.hpp"

#include "../include/objects.hpp"
#include "../include/game.hpp"
#include "../include/core.hpp"

namespace Game {
    
    Core::Application *engine;
    std::vector<Objects::Rectangle*> level;
    
    void setApplication(Core::Application *p_engine) {
	engine = p_engine;
	return;
    }
    
    Objects::Rectangle *player_hitbox;
    Objects::AnimatedSprite *player;
    
    // --- animations --- 
    std::vector<int> walk_frames = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    std::vector<int> falling_frames = { 25, 26, 27, 28, 29 };
    std::vector<int> jump_frames = { 20, 21, 22, 23, 24 };
    std::vector<int> idle_frames = { 6, 6 };
    
    Animator::Animation *FALLING;
    Animator::Animation *IDLE;
    Animator::Animation *WALK;
    Animator::Animation *JUMP;
    
    Objects::Rectangle *getPlayerHitbox() {
	return player_hitbox;
    }
    
    json player_json_data;
    int sprite_index = 0;
    void ready() {
	engine->buildLevel("./assets/test-level.png");
	level = engine->getLevel();
	
	player = new Objects::AnimatedSprite("Player", "./assets/player/sprite_sheet.png", 35.0, 6.0, 6.0, sprite_index, true, false, engine->getMainShader());
	player_hitbox = new Objects::Rectangle("Player", false);
	
	sprite_index = idle_frames[0];
	FALLING = new Animator::Animation(&falling_frames, true, 3, &sprite_index);
	IDLE = new Animator::Animation(&idle_frames, true, 0, &sprite_index);
	WALK = new Animator::Animation(&walk_frames, true, 1, &sprite_index);
	JUMP = new Animator::Animation(&jump_frames, true, 2, &sprite_index);
	
	// TODO: i don't know, it seems ugly and hardcoded.
	player_hitbox->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player_hitbox->self.color = glm::vec4(255.0f, 0.0f, 0.0f, 0.0f);
	player->self.position = glm::vec3(-2.0f, -2.0f, -5.0f);
	player_hitbox->self.scale = glm::vec2(3.0f, 6.0f);
	
	player->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player->self.color = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
	player->self.scale = glm::vec2(8.5f, 8.5f);
	return;
    }
    
    const float ACCELERATION = 0.2f;
    const int MAX_JUMP_QUANTITY = 3;
    const float JUMP_FORCE = 40.0f;
    const float FRICTION = 0.07f;
    const float SPEED = 25.0f;
    
    const float NORMAL_GRAVITY = 0.08f;
    float GRAVITY = NORMAL_GRAVITY;
    bool is_on_floor = false;
    
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
    static void move(double delta) {
	float direction = 0.0f;
	if (engine->isKeyPressed(GOLF_D)) {
	    player->self.rotation.y = 0.0f;
	    direction = 1.0f;
	}
	else if (engine->isKeyPressed(GOLF_A)) {
	    player->self.rotation.y = 180.0f;
	    direction = -1.0f;
	}
	
	// TODO: find a better and more precise way to do that.
	Objects::Rectangle forecast_player("forecastPlayer", false);
	forecast_player.self.position = player_hitbox->self.position;
	forecast_player.self.scale = player_hitbox->self.scale;
	float forecast_velocity;
	
	if (direction == 1.0f || direction == -1.0f) {
	    forecast_velocity = Math::lerp(velocity.x, SPEED * direction, ACCELERATION);
	    if (is_on_floor) {
		WALK->play();
	    }
	} else if (direction == 0.0f) {
	    forecast_velocity = Math::lerp(velocity.x, 0.0f, FRICTION);
	    if (is_on_floor) {
		// sprite_index = idle_frames[0];
		IDLE->play();
	    }
	}
	forecast_player.self.position.x += forecast_velocity * delta;
	
	// found out this method. It works, but for sure has problems. That's the X-axis move and collide system.
	for (int i = 0; i < (int)level.size(); i++) {
	    if (level[i]->name == "block") {
		if (Physics::isColliding(&forecast_player.self, &level[i]->self )) {
		    if (velocity.x > 0.0f) {
			player_hitbox->self.position.x = level[i]->self.position.x - player_hitbox->self.scale.x;
			velocity.x = 0.0f;
			
		    } else if (velocity.x < 0.0f) {
			player_hitbox->self.position.x = level[i]->self.position.x + level[i]->self.scale.x;
			velocity.x = 0.0f;
		    }
		    return;
		}
	    }
	}
	velocity.x = forecast_velocity;
	return;
    }
    
    int delay = 0;
    void process(double delta) {
	Molson(_set_int)("index", sprite_index, true, engine->getMainShader());
	
	player->self.position = glm::vec3(player_hitbox->self.position.x - 2.5f, player_hitbox->self.position.y - 1.1f, player_hitbox->self.position.z + 0.5f);
	move(delta);
	player_hitbox->self.position.x += velocity.x * delta;
	player_hitbox->self.position.y += velocity.y * delta;
	
	// this reminds me of the game maker times...
	for (int i = 0; i < (int)level.size(); i++) {
	    if (level[i]->name == "block") {
		if (!Physics::isOnFloor(&player_hitbox->self, &level[i]->self)) {
		    velocity.y -= GRAVITY;
		    is_on_floor = false;
		} else if (Physics::isOnFloor(&player_hitbox->self, &level[i]->self) == true) {
		    // TODO: bad way to do that
		    is_on_floor = true;
		    bool ground = false;
		    if (velocity.y > 0.0f) { // collision ceiling
			player_hitbox->self.position.y = level[i]->self.position.y - player_hitbox->self.scale.y;
		    } else if (velocity.y < 0.0f) {
			player_hitbox->self.position.y = level[i]->self.position.y + level[i]->self.scale.y;
			ground = true;
		    }
		    velocity.y = 0.0f;
		    
		    if (ground) {
			if (engine->isKeyPressed(GOLF_UP)) {
			    velocity.y = JUMP_FORCE;
			}
		    }
		    return;
		}
	    }
	}
	return;
    }
    void input(GLFWwindow *p_window, int key, int scancode, int action, int mods) {
	// detect inputs
	return;
    }
    
    void render() {
	engine->renderLevel(engine->getMainShader());
	
	player_hitbox->render(engine->getMainShader());
	player->render(engine->getMainShader());
    }
    
    void destroy() {
	engine->destroyLevel();
	delete player_hitbox;
	delete player;
    }
}
