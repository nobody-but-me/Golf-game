
#pragma once
#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>

#include "./molson.h"

namespace Core {
    
    class Application {
	private:
	    const int HEIGHT = 600;
	    const int WIDTH  = 800;
	    
	    bool running = false;
	    GLFWwindow *window;
	    Shader main_shader;
	    
	public:
	    const int *getWindowHeight();
	    const int *getWindowWidth();
	    Shader *getMainShader();
	    GLFWwindow *getWindow();
	    
	    bool isKeyPressed(int p_key);
	    bool isRunning();
	    
	    Application(std::string p_title);
	    ~Application();
	    
    };
    
}

#endif//CORE_H

