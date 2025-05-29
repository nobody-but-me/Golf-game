
#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#define ANIMATOR_IMPLEMENTATION
#include "./animator.hpp"
#include "./objects.hpp"
#include "./core.hpp"

namespace PLAYER {
    class Player {
	private:
	    const float NORMAL_GRAVITY = 0.08f;
	    const float ACCELERATION = 0.2;
	    const float JUMP_FORCE = 40.0f;
	    const float FRICTION = 0.07f;
	    const float SPEED = 25.0f;
	    
	    float GRAVITY = NORMAL_GRAVITY;
	    bool is_on_ceiling = false;
	    bool is_on_floor = false;
	    bool is_on_wall = false;
	    int sprite_index = 0;
	    
	    std::vector<Animator::Animation*> animations;
	    
	    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
	public:
	    std::vector<Objects::Rectangle*> level;
	    Objects::Rectangle *player_hitbox;
	    Objects::AnimatedSprite *player;
	    Core::Application *engine;
	    
	    
	    Player(Core::Application *p_engine);
	    ~Player();
	    
	    void render(double delta);
	    void move(double delta);
	    
	};
}

#endif//PLAYER_H
#ifdef PLAYER_IMPLEMENTATION

#include <iostream>
#include <string>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#define ANIMATOR_IMPLEMENTATION
#include "./animator.hpp"
#define PHYSICS_IMPLEMENTATION
#include "./physics.hpp"
#include "./objects.hpp"
#define MATH_IMPLEMENTATION
#include "./math.hpp"
#include "./core.hpp"
#include "./molson.h"

namespace PLAYER {

    // TODO: find a better place to put these.
    // --- ANIMATIONS --- 
    std::vector<int> walk_frames = { 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    std::vector<int> falling_frames = { 25, 26, 27, 28, 29 };
    std::vector<int> jump_frames = { 20, 21, 22, 23, 24 };
    std::vector<int> idle_frames = { 6, 6 };

    Player::Player(Core::Application *p_engine) {
	Animator::Animation *FALLING;
	Animator::Animation *JUMP;
	Animator::Animation *IDLE;
	Animator::Animation *WALK;
	
	animations.push_back(FALLING);
	animations.push_back(JUMP);
	animations.push_back(IDLE);
	animations.push_back(WALK);
	
	engine = p_engine;
	
	player = new Objects::AnimatedSprite("PlayerSprite", "./assets/player/sprite_sheet.png", 35.0, 6.0, 6.0, sprite_index, true, false, engine->getMainShader());
	player_hitbox = new Objects::Rectangle("PlayerHitbox", false);
	
	player_hitbox->self.color = glm::vec4(255.0f, 0.0f, 0.0f, 0.0f);
	player_hitbox->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player->self.position = glm::vec3(-2.0f, -2.0f, -5.0f);
	player_hitbox->self.scale = glm::vec2(3.0f, 6.0f);
	
	player->self.color = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
	player->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player->self.scale = glm::vec2(8.5f, 8.5f);
	
	sprite_index = idle_frames[0];
	
	animations[0] = new Animator::Animation(&falling_frames, false, 1, &sprite_index);
	animations[1] = new Animator::Animation(&jump_frames, false, 1, &sprite_index);
	animations[2] = new Animator::Animation(&idle_frames, true, 0, &sprite_index);
	animations[3] = new Animator::Animation(&walk_frames, true, 1, &sprite_index);
	
	return;
    }

    Player::~Player() {
	delete player_hitbox;
	delete player;
	return;
    }

    void Player::move(double delta) {
	float direction = 0.0f;
	if (engine->isKeyPressed(GOLF_D)) {
	    player->self.rotation.y = 0.0f;
	    direction = 1.0f;
	} else if (engine->isKeyPressed(GOLF_A)) {
	    player->self.rotation.y = 180.0f;
	    direction = -1.0f;
	}
	
	// TODO: this is a good start, but needs to be replaced.
	Objects::Rectangle FORECASTING_PLAYER("PlayerForecast", false);
	FORECASTING_PLAYER.self.position = player_hitbox->self.position;
	FORECASTING_PLAYER.self.scale = player_hitbox->self.scale;
	float FORECASTING_VELOCITY;
	if (direction == 1.0f || direction == -1.0f) {
	    FORECASTING_VELOCITY = Math::lerp(velocity.x, SPEED * direction, ACCELERATION);
	    if (is_on_floor) animations[3]->play();
	} else if (direction == 0.0f) {
	    FORECASTING_VELOCITY = Math::lerp(velocity.x, 0.0f, FRICTION);
	    if (is_on_floor) animations[2]->play();
	}
	FORECASTING_PLAYER.self.position.x += FORECASTING_VELOCITY * delta;
	
	for (int i = 0; i < (int)level.size(); i++) {
	    if (level[i]->name == "block") {
		if (Physics::isColliding(&FORECASTING_PLAYER.self, &level[i]->self)) {
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
	velocity.x = FORECASTING_VELOCITY;
	return;
    }
    void Player::render(double delta) {
	Molson(_set_int)("index", sprite_index, true, engine->getMainShader());
	
	player->self.position = glm::vec3(player_hitbox->self.position.x - 2.5f, player_hitbox->self.position.y - 1.1f, player_hitbox->self.position.z + 0.5f);
	
	move(delta);
	
	player_hitbox->self.position.x += velocity.x * delta;
	player_hitbox->self.position.y += velocity.y * delta;
	
	// this reminds me of the good game maker times...
	for (int i = 0; i < (int)level.size(); i++) {
	    if (level[i]->name == "block") {
		if (!Physics::isOnFloor(&player_hitbox->self, &level[i]->self)) {
		    velocity.y -= GRAVITY;
		    is_on_floor = false;
		} else {
		    bool ground = false;
		    if (velocity.y > 0.0f) player_hitbox->self.position.y = level[i]->self.position.y - player_hitbox->self.scale.y;
		    else if (velocity.y < 0.0f) {
			player_hitbox->self.position.y = level[i]->self.position.y + level[i]->self.scale.y;
			ground = true;
		    }
		    velocity.y = 0.0f;
		    
		    if (ground) {
			is_on_floor = true;
			if (engine->isKeyPressed(GOLF_UP)) {
			    velocity.y = JUMP_FORCE;
			}
		    }
		    break;
		}
	    }
	}
	player_hitbox->render(engine->getMainShader());
	player->render(engine->getMainShader());
	return;
    }
}

#endif//PLAYER_IMPLEMENTATION
