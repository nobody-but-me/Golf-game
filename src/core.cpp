
#include <iostream>
#include <string>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include "../include/core.hpp"

namespace Core {
    
    GLFWwindow *Application::getWindow() {
	return window;
    }
    bool Application::isRunning() {
	return running;
    }
    

    static void window_resized_callback(GLFWwindow *window, int w, int h) {
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	return;
    }
    Application::Application(std::string title) {
	if (!glfwInit()) {
	    std::cout << "[FAILED]: OpenGL library could not be loaded. \n" << std::endl;
	    return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), NULL, NULL);
	if (window == NULL) {
	    std::cout << "[FAILED]: Application's window could not be created. \n" << std::endl;
	    return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	    std::cout << "[FAILED]: Glad header library could not be loaded. \n" << std::endl;
	    return;
	}
	glfwSetFramebufferSizeCallback(window, window_resized_callback);
	glEnable(GL_DEPTH_TEST);
	running = true;
	std::cout << "\n[INFO]: Hello, Golf.\n" << std::endl;
	return;
    }
    Application::~Application() {
	running = false;
	
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "\n[INFO]: Application destroyed. \n" << std::endl;
	return;
    }
    
}
