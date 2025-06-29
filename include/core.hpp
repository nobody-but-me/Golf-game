
#pragma once
#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>

enum KEYCODES {
    
    GOLF_MAIN_JOY = GLFW_JOYSTICK_1,
    
    GOLF_JOY_BUTTON_A = GLFW_GAMEPAD_BUTTON_A,
    GOLF_JOY_BUTTON_B = GLFW_GAMEPAD_BUTTON_B,
    GOLF_JOY_BUTTON_X = GLFW_GAMEPAD_BUTTON_X,
    GOLF_JOY_BUTTON_Y = GLFW_GAMEPAD_BUTTON_Y,
    
    GOLF_ESCAPE = GLFW_KEY_ESCAPE,
    GOLF_SPACE  = GLFW_KEY_SPACE,
    GOLF_ENTER  = GLFW_KEY_ENTER,
    
    GOLF_RIGHT_CTRL = GLFW_KEY_RIGHT_CONTROL,
    GOLF_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
    GOLF_LEFT_CTRL = GLFW_KEY_LEFT_CONTROL,
    GOLF_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    
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

#include <vector>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "./objects.hpp"
#include "./molson.h"

namespace Core {
    
    namespace Input {
	bool is_joystick_button_pressed(unsigned int p_joystick, unsigned int p_button);
	bool is_key_pressed(GLFWwindow *window, int p_key);
	std::vector<float> get_right_axes(int p_controler);
	std::vector<float> get_left_axes(int p_controler);
    }
    
    class Application {
	    private:
	        int TEST_WIDTH  = 1024;
	        int TEST_HEIGHT = 600;
		
	        int WIDTH  = 1280;
	        int HEIGHT = 720;
		
	        bool running = false;
	        GLFWwindow *window;
	        Shader main_shader;
	    public:
		
	        Shader     *get_main_shader() {
		    return &main_shader;
		}
	        bool        is_running() { 
		    return running;
		}
	        GLFWwindow *get_window() {
		    return window;
		}
	        int        *get_window_height() {
		    return &HEIGHT;
		}
	        int        *get_window_width() {
		    return &WIDTH;
		}
		
	        int set_window_height();
	        int set_window_width();
		glm::mat4  *get_view();

	        std::vector<Objects::AnimatedSprite*> &get_level();
	        glm::vec3 build_level(std::string p_level_path, std::string p_level_tiles_path);
	        void render_level(Shader *p_shader);
	        void destroy_level();

	        Application(std::string p_title);
	        ~Application();

        };

    }

    #endif//CORE_H
