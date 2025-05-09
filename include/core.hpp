
#pragma once
#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>

namespace Core {
    
    class Application {
	private:
	    const int HEIGHT = 600;
	    const int WIDTH  = 800;
	    
	    bool running = false;
	    GLFWwindow *window;
	    
	public:
	    GLFWwindow *getWindow();
	    bool isRunning();
	    
	    Application(std::string title);
	    ~Application();
	    
    };
    
}

#endif//CORE_H

