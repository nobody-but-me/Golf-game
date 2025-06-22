
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
	    const float NORMAL_GRAVITY = 2.3f;

	    const float ACCELERATION = 0.2;
	    const float JUMP_FORCE = 43.0f;
	    const float FRICTION = 0.07f;
	    
	    float GRAVITY = NORMAL_GRAVITY;
	    float SPEED = 25.0f;
	    
	    bool is_on_ceiling = false;
	    bool is_on_floor = false;
	    bool is_on_wall = false;
	    int sprite_index = 0;
	    
	    std::vector<Animator::Animation*> animations;
	    
	    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
	public:
	    
	    int *getSpriteIndex() {
		return &sprite_index;
	    }
	    
	    std::vector<Objects::Rectangle*> level;
	    Objects::Rectangle *player_hitbox;
	    Objects::AnimatedSprite *player;
	    Core::Application *engine;
	    
	    Player(Core::Application *p_engine);
	    ~Player();
	    
	    void process(double delta);
	    void move(double delta);
	    void render();
	    
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
    std::vector<int> fall_frames = { 25, 26, 27, 28, 29 };
    std::vector<int> jump_frames = { 21, 22, 23, 24 };
    std::vector<int> idle_frames = { 6, 6, 6 };

    Player::Player(Core::Application *p_engine) {
	Animator::Animation *IDLE;
	Animator::Animation *WALK;
	Animator::Animation *JUMP;
	Animator::Animation *FALL;
	
	IDLE = new Animator::Animation(&idle_frames, false, 5, &sprite_index);
	WALK = new Animator::Animation(&walk_frames, true, 1, &sprite_index);
	JUMP = new Animator::Animation(&jump_frames, false, 1, &sprite_index);
	FALL = new Animator::Animation(&fall_frames, false, 1, &sprite_index);
	
	animations.push_back(IDLE);
	animations.push_back(WALK);
	animations.push_back(JUMP);
	animations.push_back(FALL);
	
	engine = p_engine;
	
	player = new Objects::AnimatedSprite("PlayerSprite", "../assets/player/sprite_sheet.png", 36.0f, 6.0f, 6.0f, sprite_index, true, false, engine->get_main_shader());
	player_hitbox = new Objects::Rectangle("PlayerHitbox", false);
	
	player_hitbox->self.color = glm::vec4(255.0f, 0.0f, 0.0f, 173.5f);
	player_hitbox->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player_hitbox->self.position = glm::vec3(-3.0f, 62.0f, -1.0f);
	player_hitbox->self.scale = glm::vec2(3.0f, 6.0f);
	
	player->self.color = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
	player->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player->self.scale = glm::vec2(8.5f, 8.5f);
	
	sprite_index = idle_frames[0];
	
	return;
    }

    Player::~Player() {
	delete player_hitbox;
	delete player;
	return;
    }

    void Player::move(double delta) {
	float direction = 0.0f;
	
	int ax;
	const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &ax);
	
	if (engine->is_key_pressed(GOLF_D) || *axes == 1) {
	    player->self.rotation.y = 0.0f;
	    direction = 1.0f;
	} else if (engine->is_key_pressed(GOLF_A) || *axes == -1) {
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
	    if (is_on_floor) {
		animations[1]->play();
	    } else animations[1]->stop(); // walk animation
	} else if (direction == 0.0f) {
	    FORECASTING_VELOCITY = Math::lerp(velocity.x, 0.0f, FRICTION);
	    if (is_on_floor) {
		animations[0]->play();
	    } else animations[0]->stop(); // idle animation
	}
	FORECASTING_PLAYER.self.position.x += FORECASTING_VELOCITY * delta;
	
	bool on_collision = false;
	for (int i = 0; i < (int)level.size(); i++) {
	    if (level[i]->name == "block") {
		if (Physics::is_colliding(&FORECASTING_PLAYER.self, &level[i]->self) 
		 && Physics::is_horizontally_aligned(&FORECASTING_PLAYER.self, &level[i]->self)) {
		    // if (velocity.x > 0.0f) {
		    if (FORECASTING_VELOCITY > 0.0f) {
			player_hitbox->self.position.x = level[i]->self.position.x - player_hitbox->self.scale.x;
			velocity.x = 0.0f;
		    // } else if (velocity.x < 0.0f) {
		    } else if (FORECASTING_VELOCITY < 0.0f) {
			player_hitbox->self.position.x = level[i]->self.position.x + level[i]->self.scale.x;
			velocity.x = 0.0f;
		    }
		    on_collision = true;
		}
	    }
	}
	
	if (!on_collision) {
	    velocity.x = FORECASTING_VELOCITY;
	    
	    if (velocity.y < -3.0f || velocity.y > 3.0f) {
		if (velocity.y > 0.0f) {
		    if (!animations[2]->played) {
			animations[2]->play();
		    }
		} else {
		    if (!animations[3]->played) {
			animations[3]->play();
		    }
		}
	    } else {
		animations[2]->played = false;
		animations[3]->played = false;
	    }
	}
	return;
    }
    
    void Player::process(double delta) {
	
	player->self.position = glm::vec3(player_hitbox->self.position.x - 2.5f, player_hitbox->self.position.y - 1.1f, player_hitbox->self.position.z + 0.5f);
	
	move(delta);
	
	player_hitbox->self.position.x += velocity.x * delta;
	player_hitbox->self.position.y += velocity.y * delta;
	
	// // this reminds me of the good game maker times...
	bool on_collision = false;
	for (int i = 0; i < (int)level.size(); i++) {
	    if (level[i]->name == "block") {
		if (Physics::is_colliding(&player_hitbox->self, &level[i]->self) && Physics::is_vertically_aligned(&player_hitbox->self, &level[i]->self)) {
		    if (velocity.y > 0.0f) player_hitbox->self.position.y = level[i]->self.position.y - player_hitbox->self.scale.y;
		    else if (velocity.y < 0.0f) {
			player_hitbox->self.position.y = level[i]->self.position.y + level[i]->self.scale.y;
			is_on_floor = true;
		    }
		    velocity.y = 0.0f;
		    on_collision = true;
		}
	    }
	}
	
	GLFWgamepadstate state;
	glfwGetGamepadState(GLFW_JOYSTICK_1, &state);
	
	if (!on_collision) {
	    velocity.y -= GRAVITY;
	    is_on_floor = false;
	} else {
	    if (engine->is_key_pressed(GOLF_UP) || state.buttons[GLFW_GAMEPAD_BUTTON_A]) {
		if (is_on_floor) {
		    velocity.y = JUMP_FORCE;
		}
	    }
	}
	return;
    }
    
    void Player::render() {
	Molson(_set_int)("index", sprite_index, true, engine->get_main_shader());
	Molson(_set_int)("SPRITE_COLUMNS", player->sprite_columns, true, engine->get_main_shader());
	Molson(_set_int)("SPRITE_FRAMES", player->sprite_frames, true, engine->get_main_shader());
	Molson(_set_int)("SPRITE_ROWS", player->sprite_rows, true, engine->get_main_shader());
	// player_hitbox->render(engine->get_main_shader());
	player->render(engine->get_main_shader());
	return;
    }
}

#endif//PLAYER_IMPLEMENTATION
