
#pragma once
#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>

enum KEYCODES {
    GOLF_ESCAPE = GLFW_KEY_ESCAPE,
    GOLF_SPACE  = GLFW_KEY_SPACE,
    GOLF_ENTER  = GLFW_KEY_ENTER,
    
    GOLF_RIGHT = GLFW_KEY_RIGHT,
    GOLF_LEFT  = GLFW_KEY_LEFT,
    GOLF_DOWN  = GLFW_KEY_DOWN,
    GOLF_UP    = GLFW_KEY_UP,
    
    GOLF_A = GLFW_KEY_A,
    GOLF_B = GLFW_KEY_B,
    GOLF_C = GLFW_KEY_C,
    GOLF_D = GLFW_KEY_D,
    GOLF_E = GLFW_KEY_E,
    GOLF_F = GLFW_KEY_F,
    GOLF_G = GLFW_KEY_G,
    GOLF_H = GLFW_KEY_H,
    GOLF_I = GLFW_KEY_I,
    GOLF_J = GLFW_KEY_J,
    GOLF_K = GLFW_KEY_K,
    GOLF_L = GLFW_KEY_L,
    GOLF_M = GLFW_KEY_M,
    GOLF_N = GLFW_KEY_N,
    GOLF_O = GLFW_KEY_O,
    GOLF_P = GLFW_KEY_P,
    GOLF_Q = GLFW_KEY_Q,
    GOLF_R = GLFW_KEY_R,
    GOLF_S = GLFW_KEY_S,
    GOLF_T = GLFW_KEY_T,
    GOLF_U = GLFW_KEY_U,
    GOLF_V = GLFW_KEY_V,
    GOLF_W = GLFW_KEY_W,
    GOLF_X = GLFW_KEY_X,
    GOLF_Y = GLFW_KEY_Y,
    GOLF_Z = GLFW_KEY_Z
};

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "./objects.hpp"
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
	        const int  *getWindowHeight();
	        const int  *getWindowWidth();
	        Shader     *getMainShader();
	        GLFWwindow *getWindow();
            glm::mat4  *getView();

	        bool isKeyPressed(int p_key);
	        bool isKeyJustPressed(int p_key);
	        bool isRunning();

	        std::vector<Objects::Rectangle*> &getLevel();
	        void buildLevel(std::string p_level_path);
	        void renderLevel(Shader *p_shader);
	        void destroyLevel();

	        Application(std::string p_title);
	        ~Application();

        };

    }

    #endif//CORE_H
