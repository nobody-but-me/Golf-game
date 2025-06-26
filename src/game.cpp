

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "../include/objects.hpp"
#define PLAYER_IMPLEMENTATION
#include "../include/player.hpp"
#include "../include/game.hpp"
#include "../include/core.hpp"

namespace Game {
    
    std::vector<Objects::AnimatedSprite*> level;
    Core::Application *engine;
    
    PLAYER::Player *player;
    
    void set_application(Core::Application *p_engine) {
	engine = p_engine;
	return;
    }
    
    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::mat4 view;
    
    void ready() {
	glm::vec3 pp = engine->build_level("../assets/test-level.png", "../assets/test-level-tiles.png");
	level = engine->get_level();
	
	view = *engine->get_view();
	
	player = new PLAYER::Player(engine);
	player->level = level;
	player->player_hitbox->self.position = pp;
    }
    
    void process(double delta) {
	view = glm::lookAt(glm::vec3(player->player_hitbox->self.position.x, player->player_hitbox->self.position.y + 5.0f, 50.0f), glm::vec3(player->player_hitbox->self.position.x, player->player_hitbox->self.position.y + 5.0f, 50.0f) + camera_front, camera_up);
	
	Molson(_set_matrix4)("view", &view, true, engine->get_main_shader());
	
	if (Core::Input::is_key_pressed(engine->get_window(), GOLF_R)) {
	    glm::vec3 pp = engine->build_level("../assets/test-level.png", "../assets/test-level-tiles.png");
	    level = engine->get_level();
	    
	    player->level = level;
	    player->player_hitbox->self.position = pp;
	}
	
	player->process(delta);
	return;
    }
    
    void input(GLFWwindow *p_window, int key, int scancode, int action, int mods) {
	return;
    }
    
    void render() {
	engine->render_level(engine->get_main_shader());
	player->render();
	return;
    }
    void destroy() {
	engine->destroy_level();
    }
}
