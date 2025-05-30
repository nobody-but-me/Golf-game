

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

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
    Objects::Rectangle *getPlayerHitbox() {
	return player->player_hitbox;
    }
    int *getPlayerSpriteIndex() {
	return player->getSpriteIndex();
    }
    
    void ready() {
	engine->buildLevel("./assets/test-level.png");
	level = engine->getLevel();
	
	player = new PLAYER::Player(engine);
	player->level = level;
    }
    
    void process(double delta) {
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
