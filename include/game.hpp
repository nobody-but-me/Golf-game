
#pragma once
#ifndef GAME_H
#define GAME_H

#include "./core.hpp"

namespace Game {
    
    void setApplication(Core::Application *p_engine);
    void process(double delta);
    void render();
    void ready();
    
    void destroy();
    
}

#endif//GAME_H
