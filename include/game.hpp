
#pragma once
#ifndef GAME_H
#define GAME_H

#include "./core.hpp"

namespace Game {
    
    void input(GLFWwindow *p_window, int key, int scancode, int action, int mods);
    void setApplication(Core::Application *p_engine);
    void process(double delta);
    void render();
    void ready();
    
    void destroy();
    
}

#endif//GAME_H
