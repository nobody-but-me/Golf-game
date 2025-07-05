
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
	    // const float NORMAL_GRAVITY = 0.f;
	    const float WALKING_SPEED = 15.0f;
	    const float NORMAL_SPEED = 25.0f;
	    
	    const float ACCELERATION = 0.2;
	    const float JUMP_FORCE = 43.0f;
	    const float FRICTION = 0.07f;
	    
	    float GRAVITY = NORMAL_GRAVITY;
	    float SPEED = 25.0f;
	    
	    bool is_on_ceiling = false;
	    bool is_on_floor   = false;
	    bool is_on_wall    = false;
	    bool is_on_roll    = false;
	    bool walking       = false;
	    int sprite_index   = 0;
	    
	    std::vector<Animator::Animation*> animations;
	    
	public:
	    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
	    
	    int *getSpriteIndex() {
		return &sprite_index;
	    }
	    
	    std::vector<Objects::AnimatedSprite*> level;
	    Objects::Rectangle *player_hitbox;
	    Objects::AnimatedSprite *player;
	    Core::Application *engine;
	    
	    Player(Core::Application *p_engine);
	    ~Player();
	    
	    void process(double delta);
	    void animation_manager();
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
    std::vector<int> ruun_frames = { 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 };
    std::vector<int> roll_frames = { 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 };
    std::vector<int> walk_frames = { 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 1 };
    std::vector<int> fall_frames = { 25, 26, 27, 28, 29 };
    std::vector<int> jump_frames = { 21, 22, 23, 24 };
    std::vector<int> idle_frames = { 8, 8, 8 };

    Player::Player(Core::Application *p_engine) {
	Animator::Animation *IDLE;
	Animator::Animation *RUUN;
	Animator::Animation *WALK;
	Animator::Animation *JUMP;
	Animator::Animation *FALL;
	Animator::Animation *ROLL;
	
	IDLE = new Animator::Animation(&idle_frames, false, 5, &sprite_index);
	RUUN = new Animator::Animation(&ruun_frames, true , 1, &sprite_index);
	WALK = new Animator::Animation(&walk_frames, true , 1, &sprite_index);
	JUMP = new Animator::Animation(&jump_frames, false, 1, &sprite_index);
	FALL = new Animator::Animation(&fall_frames, false, 1, &sprite_index);
	ROLL = new Animator::Animation(&roll_frames, false, 3, &sprite_index);
	
	animations.push_back(IDLE);
	animations.push_back(RUUN);
	animations.push_back(JUMP);
	animations.push_back(FALL);
	animations.push_back(ROLL);
	animations.push_back(WALK);
	
	engine = p_engine;
	
	player = new Objects::AnimatedSprite("PlayerSprite", "../assets/player/sprite_sheet.png", 64.0f, 8.0f, 8.0f, sprite_index, true, false, engine->get_main_shader());
	player_hitbox = new Objects::Rectangle("PlayerHitbox", false);
	
	player_hitbox->self.color = glm::vec4(255.0f, 0.0f, 0.0f, 173.5f);
	player_hitbox->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	player_hitbox->self.position = glm::vec3(-3.0f, 62.0f, -1.0f);
	// player_hitbox->self.scale = glm::vec2(3.0f, 6.0f);
	player_hitbox->self.scale = glm::vec2(3.0f, 4.0f);
	
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
	
	std::vector<float> left_joystick = Core::Input::get_left_axes(GOLF_MAIN_JOY);
	
	if (is_on_roll == false) {
	    if (Core::Input::is_key_pressed(engine->get_window(), GOLF_LEFT_SHIFT) || Core::Input::is_joystick_button_pressed(GOLF_MAIN_JOY, GOLF_JOY_BUTTON_B)) {
		// player_hitbox->self.scale.y = 1.0f;
		is_on_roll = true;
	    }
	    
	    if (Core::Input::is_key_pressed(engine->get_window(), GOLF_D) || left_joystick[0] == 1) {
		player->self.rotation.y = 0.0f;
		direction = 1.0f;
	    } else if (Core::Input::is_key_pressed(engine->get_window(), GOLF_A) || left_joystick[0] == -1) {
		player->self.rotation.y = 180.0f;
		direction = -1.0f;
	    }
	}
	
	// TODO: this is a good start, but needs to be replaced.
	Objects::Rectangle FORECASTING_PLAYER("PlayerForecast", false);
	FORECASTING_PLAYER.self.position = player_hitbox->self.position;
	FORECASTING_PLAYER.self.scale = player_hitbox->self.scale;
	float FORECASTING_VELOCITY = 0.0f;
	
	if (is_on_roll == false) {
	    if (direction == 1.0f || direction == -1.0f) {
		FORECASTING_VELOCITY = Math::lerp(velocity.x, SPEED * direction, ACCELERATION);
	    }
	    else if (direction == 0.0f) {
		FORECASTING_VELOCITY = Math::lerp(velocity.x, 0.0f, FRICTION);
	    }
	} else FORECASTING_VELOCITY = velocity.x;
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
	}
	return;
    }
    
    void Player::animation_manager() {
	// TODO: find a better way to manage these states;
	if (is_on_roll == false) {
	    if (is_on_floor == true) {
		if (velocity.x >= 2.0f || velocity.x <= -2.0f) {
		    if (walking == false) {
			animations[1]->play();
		    } else {
			animations[5]->play();
		    }
		}
		else {
		    animations[0]->play();
		    
		}
	    } else {
		animations[0]->stop();
		animations[1]->stop();
		if (velocity.y > 5.0f || velocity.y < -5.0f) {
		    if (velocity.y > 10.0f) {
			if (!animations[2]->played) {
			    animations[2]->play();
			}
		    } else if (velocity.y < -10.0f) {
			if (!animations[3]->played) {
			    animations[3]->play();
			}
		    }
		} else {
		    animations[2]->played = false;
		    animations[3]->played = false;
		}
	    }
	} else {
	    animations[0]->stop();
	    animations[1]->stop();
	    animations[2]->stop();
	    animations[3]->stop();
	    
	    if (animations[4]->played == false) {
		animations[4]->play();
	    } else {
		animations[4]->played = false;
		is_on_roll = false;
	    }
	}
    }
    
    void Player::process(double delta) {
	player->self.position = glm::vec3(player_hitbox->self.position.x - 2.5f, player_hitbox->self.position.y - 1.1f, player_hitbox->self.position.z + 0.5f);
	
	// TODO: find a better way to do all of these.
	if (is_on_roll == true) {
	    player_hitbox->self.scale.y = 3.0f;
	    if (player->self.rotation.y == 180.0f) velocity.x = -35.0f;
	    else velocity.x = 35.0f;
	} else player_hitbox->self.scale.y = 4.0f;
	
	if (is_on_floor == true) {
	    if (Core::Input::is_key_pressed(engine->get_window(), GOLF_LEFT_CTRL) || Core::Input::is_joystick_button_pressed(GOLF_MAIN_JOY, GOLF_JOY_BUTTON_Y)) {
		SPEED = WALKING_SPEED;
		walking = true;
	    } else {
		SPEED = NORMAL_SPEED;
		walking = false;
	    }
	}
	
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
	
	if (!on_collision) {
	    velocity.y -= GRAVITY;
	    is_on_floor = false;
	} else {
	    if (Core::Input::is_key_pressed(engine->get_window(), GOLF_UP) || Core::Input::is_joystick_button_pressed(GOLF_MAIN_JOY, GOLF_JOY_BUTTON_A)) {
		if (is_on_floor) {
		    velocity.y = JUMP_FORCE;
		}
	    }
	}
	animation_manager();
	return;
    }
    
    void Player::render() {
	Molson(_set_int)("SPRITE_COLUMNS", player->sprite_columns, true, engine->get_main_shader());
	Molson(_set_int)("SPRITE_FRAMES", player->sprite_frames, true, engine->get_main_shader());
	Molson(_set_int)("SPRITE_ROWS", player->sprite_rows, true, engine->get_main_shader());
	Molson(_set_int)("index", sprite_index, true, engine->get_main_shader());
	#if DEBUG_MODE == true
	player_hitbox->render(engine->get_main_shader());
	#endif
	player->render(engine->get_main_shader());
	return;
    }
}

#endif//PLAYER_IMPLEMENTATION
