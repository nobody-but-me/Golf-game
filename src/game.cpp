

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
    
    Core::Application *engine;
    std::vector<Objects::Rectangle*> level;
    
    void setApplication(Core::Application *p_engine) {
	engine = p_engine;
	return;
    }
    
    PLAYER::Player *player;
    // TODO: take these functions out of here. This annoys me.
    Objects::Rectangle *getPlayerHitbox() {
	return player->player_hitbox;
    }
    int *getPlayerSpriteIndex() {
	return player->getSpriteIndex();
    }
    
    
    glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::mat4 view;
    
    void ready() {
	engine->buildLevel("./assets/test-level.png");
	level = engine->getLevel();
	
	view = *engine->getView();
	
	player = new PLAYER::Player(engine);
	player->level = level;
    }
    
    void process(double delta) {
	view = glm::lookAt(glm::vec3(player->player_hitbox->self.position.x, player->player_hitbox->self.position.y + 5.0f, 50.0f), glm::vec3(player->player_hitbox->self.position.x, player->player_hitbox->self.position.y + 5.0f, 50.0f) + camera_front, camera_up);
	Molson(_set_matrix4)("view", &view, true, engine->getMainShader());
	
	engine->renderLevel(engine->getMainShader());
	player->render(delta);
	return;
    }
    
    void input(GLFWwindow *p_window, int key, int scancode, int action, int mods) {
	return;
    }
    
    void render() {
	return;
    }
    void destroy() {
	engine->destroyLevel();
    }
}
