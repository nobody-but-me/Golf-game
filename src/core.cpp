
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../include/glad.h"
#include <GLFW/glfw3.h>
#include <ctype.h>

#include "../include/objects.hpp"
#include "../include/stb_image.h"
#define EDITOR_IMPLEMENTATION
#include "../include/core.hpp"
#include "../include/game.hpp"
#define MOLSON_IMPLEMENTATION
#include "../include/molson.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>


namespace Core {
    
    namespace Input {
	std::vector<float> get_left_axes(int p_controler) {
	    GLFWgamepadstate state;
	    if (glfwGetGamepadState(p_controler, &state)) {
		float x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
		float y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
		
		std::vector<float> left_axes;
		left_axes.push_back(x);
		left_axes.push_back(y);
		return left_axes;
	    }
	    std::vector<float> zero;
	    zero.push_back(0.0f);
	    zero.push_back(0.0f);
	    return zero;
	}
	std::vector<float> get_right_axes(int p_controler) {
	    GLFWgamepadstate state;
	    if (glfwGetGamepadState(p_controler, &state)) {
		float x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
		float y = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
		
		std::vector<float> right_axes;
		right_axes.push_back(x);
		right_axes.push_back(y);
		return right_axes;
	    }
	    std::vector<float> zero;
	    zero.push_back(0.0f);
	    zero.push_back(0.0f);
	    return zero;
	}
	
	bool is_key_pressed(GLFWwindow *window, int p_key) {
	    int state = glfwGetKey(window, p_key);
	    if (state == GLFW_PRESS) {
		return true;
	    }
	    return false;
	}
	bool is_joystick_button_pressed(unsigned int p_joystick, unsigned int p_button) {
	    GLFWgamepadstate state;
	    if (glfwGetGamepadState(p_joystick, &state)) {
		return state.buttons[p_button];
	    }
	    return false;
	}
    }
    
    static void window_resized_callback(GLFWwindow *window, int w, int h) {
	// glfwSetWindowAspectRatio(window, 1064, 600);
	glfwGetFramebufferSize(window, &w, &h);
	
	glViewport(w / 2 - (1280 / 2), h / 2 - (720 / 2), 1280, 720);
	// glViewport(0.0f, 0.0f, w, h);
	return;
    }
    
    // PROJECTION VARIABLES
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    
    glm::mat4 *Application::get_view() {
	return &view;
    }
    
    Application::Application(std::string p_title) {
	if (!glfwInit()) {
	    std::cout << "[FAILED]: OpenGL library could not be loaded. \n" << std::endl;
	    return;
	}
	// Decide GL+GLSL versions
	#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100 (WebGL 1.0)
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	#elif defined(IMGUI_IMPL_OPENGL_ES3)
	// GL ES 3.0 + GLSL 300 es (WebGL 2.0)
	const char* glsl_version = "#version 300 es";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150-
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
	#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	#endif
	
	std::cout << "[INFO] GLSL VERSION :: " << glsl_version << std::endl;
	
	window = glfwCreateWindow(TEST_WIDTH, TEST_HEIGHT, p_title.c_str(), NULL, NULL);
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
	// glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);
	
	glViewport(TEST_WIDTH / 2 - (WIDTH / 2), TEST_HEIGHT / 2 - (HEIGHT / 2), WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	GLFWimage icons[1];
	icons[0].pixels = stbi_load("../assets/m.png", &icons[0].width, &icons[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, icons); 
	stbi_image_free(icons[0].pixels);
	
	// glfwSetWindowIcon(window, 1, icons);
	
	Molson(_init_shader)("../shaders/object.vert", "../shaders/object.frag", &main_shader);
	
	view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -50.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	// projection = glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
	
	Molson(_set_matrix4)("projection", &projection, true, &main_shader);
	Molson(_set_matrix4)("view", &view, true, &main_shader);
	Molson(_set_int)("object_image", 0, true, &main_shader);
	
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

    // --------------------------------------------------
    // For my family: sorry for the code below, I regret it.
    // now it seems more clean, but I am still sorry.
    
    std::vector<Objects::AnimatedSprite*> level;
    
    const float TILES_FRAMES = 36.0f;
    const float TILES_COLS = 6.0f;
    const float TILES_ROWS = 6.0f;
    
    std::vector<Objects::AnimatedSprite*> &Application::get_level() {
	return level;
    }
    
    static std::vector<std::vector<std::string>> read_file(std::string text_file) {
	std::ifstream file(text_file);
	std::string line;
	
	std::vector<std::vector<std::string>> base;
	if (file.is_open())
	{
	    while (std::getline(file, line))
	    {
		std::vector<std::string> l;
		
		std::string str;
		for (unsigned int i = 0; i < (unsigned int)line.length(); ++i)
		{
		    if (line[i] != ' ') {
			str += line[i];
			continue;
		    } else {
			l.push_back(str);
			str = "";
			continue;
		    }
		}
		base.push_back(l);
	    }
	    file.close();
	}
	
	return base;
    }
    
    glm::vec3 Application::build_level(std::string p_level_path, std::string p_level_tiles_path) {
	level.clear();
	std::vector<std::vector<std::string>> bases = read_file("../assets/level0.txt");
	
	// -- create of the actual tiles --.
	glm::vec3 player_position;
	for (int y = 0; y < static_cast<int>(bases.size()); ++y) {
	    for (int x = 0; x < static_cast<int>(bases[0].size()); ++x) {
		if (bases[y][x] == "E") continue;
		else if (bases[y][x] == "P") {
		    player_position = glm::vec3((x * 3.0f) - static_cast<int>(bases[0].size()) / 2, y * 3.0f, 0.0f);
		    continue;
		}
		Objects::AnimatedSprite *tile = new Objects::AnimatedSprite("block", "../assets/tiles.png", 36.0f, 6.0f, 6.0f, 10, true, false, &main_shader);
		tile->self.color = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
		tile->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		
		tile->self.scale = glm::vec2(3.0f, 3.0f);
		tile->self.position = glm::vec3((x * tile->self.scale.x) - static_cast<int>(bases[0].size()) / 2, (y * tile->self.scale.y) * -1, 0.0f);
		
		tile->sprite_index = atoi(bases[y][x].c_str());
		switch (atoi(bases[y][x].c_str())) {
		    case 4:
		    case 5:
		    case 11:
		    case 13:
		    case 16:
		    case 17:
		    case 22:
		    case 23:
		    case 27:
		    case 29:
		    case 34:
		    case 35: {
			tile->name = "non-collidable-block"; // search for a better name;
		    } break;
		}
		level.push_back(tile);
	    }
	}
	std::cout << "[INFO] level had been built. \n";

	return player_position;
    }
    
    void Application::render_level(Shader *p_shader) {
	Molson(_set_int)("SPRITE_FRAMES", TILES_FRAMES, true, p_shader);
	Molson(_set_int)("SPRITE_COLUMNS", TILES_COLS, true, p_shader);
	Molson(_set_int)("SPRITE_ROWS", TILES_ROWS, true, p_shader);
	for (int i = 0; i < (int)level.size(); i++) {
	    Molson(_set_int)("index", level[i]->sprite_index, true, p_shader);
	    level[i]->render(p_shader);
	}
	return;
    }
    
    void Application::destroy_level() {
	for (int i = 0; i < (int)level.size(); i++) {
	    delete level[i];
	}
	// for (int i = 0; i < (int)level_tiles.size(); i++) {
	//     delete level_tiles[i];
	// }
	return;
    }
}
