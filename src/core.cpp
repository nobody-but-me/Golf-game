
#include <iostream>
#include <string>
#include <vector>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

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
    }
    
    static void window_resized_callback(GLFWwindow *window, int w, int h) {
	glfwGetFramebufferSize(window, &w, &h);
	// glViewport(w / 2 - (1064 / 2), h / 2 - (600 / 2), 1064, 600);
	glfwSetWindowAspectRatio(window, w, h);
	glViewport(0.0f, 0.0f, w, h);
	return;
    }
    
    // VIEW VARIABLES
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
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	#endif
	
	std::cout << "[INFO] GLSL VERSION :: " << glsl_version << std::endl;

	window = glfwCreateWindow(WIDTH, HEIGHT, p_title.c_str(), NULL, NULL);
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
	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);
	
	glViewport(WIDTH / 2 - (1064 / 2), HEIGHT / 2 - (600 / 2), 1064, 600);
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
    
    std::vector<Objects::AnimatedSprite*> level_tiles;
    std::vector<Objects::Rectangle*> level;
    
    const float TILES_FRAMES = 36.0f;
    const float TILES_COLS = 6.0f;
    const float TILES_ROWS = 6.0f;
    
    std::vector<Objects::Rectangle*> &Application::get_level() {
	return level;
    }
    
    static std::vector<std::vector<std::vector<unsigned int>>> load_level_image(std::string p_image_path, int &p_w, int &p_h) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(p_image_path.c_str(), &width, &height, &channels, 3);
	
	p_h = height;
	p_w = width;
	
	std::vector<std::vector<unsigned int>> RGB;
	if (data != nullptr && width > 0 && height > 0) {
	    for (unsigned int i = 0; i < (unsigned int)(height * width); i++) {
		std::vector<unsigned int> rgb;
		rgb.push_back(data[i * 3]);
		rgb.push_back(data[i * 3 + 1]);
		rgb.push_back(data[i * 3 + 2]);

		RGB.push_back(rgb);
	    }
	    stbi_image_free(data);
	}
	
	// converting 2D vector into a 3D vector;
	std::vector<std::vector<std::vector<unsigned int>>> matrix(height, std::vector<std::vector<unsigned int>>(width, std::vector<unsigned int>(3, 0))); // that's crazy.
	int cursor = 0;
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width; x++) {
		matrix[y][x] = RGB[cursor++];
	    }
	}
	return matrix;
    }
    glm::vec3 Application::build_level(std::string p_level_path, std::string p_level_tiles_path) {
	level.clear();
	
	int bases_height, bases_width;
	int tiles_height, tiles_width;
	std::vector<std::vector<std::vector<unsigned int>>> tiles = load_level_image(p_level_tiles_path, tiles_width, tiles_height);
	std::vector<std::vector<std::vector<unsigned int>>> bases = load_level_image(p_level_path, bases_width, bases_height);
	
	// std::vector<std::vector<char>> bases = {
	//     {'0', '0', '0', '0', '0', '0', '0', '0'},
	//     {'1', '0', '0', '0', '1', '0', '0', '0'},
	//     {'1', '0', '1', '1', '1', '0', '1', '1'},
	//     {'1', '1', '1', '1', '1', '1', '1', '1'},
	// };
	
	
	// // -- create of the actual blocks --.
	// glm::vec3 player_position;
	// for (int y = 0; y < static_cast<int>(bases.size()); ++y) {
	//     for (int x = 0; x < static_cast<int>(bases[0].size()); ++x) {
	// 	if (bases[y][x] == '0') continue;
	// 	// else if (bases[y][x][0] == 0 && bases[y][x][1] == 255 && bases[y][x][2] == 0) {
	// 	//     player_position = glm::vec3((x * 3.0f) - static_cast<int>(bases[0][0].size()) / 2, y * 3.0f, 0.0f);
	// 	//     continue;
	// 	// }
	// 	Objects::Rectangle *block = new Objects::Rectangle("block", false);
		
	// 	block->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	// 	block->self.color = glm::vec4(42.0f, 93.0f, 232.0f, 255.0f);
	// 	block->self.scale = glm::vec2(3.0f, 3.0f);
		
	// 	block->self.position = glm::vec3((x * block->self.scale.x) - static_cast<int>(bases[0].size()) / 2, (y * block->self.scale.y) * -1, -0.99f);
		
	// 	// if (bases[y][x][0] == 255 && bases[y][x][1] == 0 && bases[y][x][2] == 0) {
	// 	//     block->self.color = glm::vec4(255.0f, 0.0f, 0.0f, 255.0f);
	// 	//     block->name = "idk";
	// 	// }
	// 	level.push_back(block);
	//     }
	// } 
	glm::vec3 player_position = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int y = 0; y < bases_height; ++y) {
	    for (int x = 0; x < bases_width; ++x) {
		if (bases[y][x][0] == 0 && bases[y][x][1] == 0 && bases[y][x][2] == 0) continue;
		else if (bases[y][x][0] == 255 && bases[y][x][1] == 255 && bases[y][x][2] == 255) {
		    player_position = glm::vec3((x * 3.0f) - bases_width / 2, y * 3.0f, 0.0f);
		    continue;
		}
		Objects::Rectangle *block = new Objects::Rectangle("block", false);
		
		block->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		block->self.color = glm::vec4(42.0f, 93.0f, 232.0f, 255.0f);
		block->self.scale = glm::vec2(3.0f, 3.0f);
		
		block->self.position = glm::vec3((x * block->self.scale.x) - bases_width / 2, y * block->self.scale.y, 0.0f);
		
		if (bases[y][x][0] == 255 && bases[y][x][1] == 0 && bases[y][x][2] == 0) {
		    block->self.color = glm::vec4(255.0f, 0.0f, 0.0f, 255.0f);
		    block->name = "idk";
		}
		level.push_back(block);
	    }
	}
	
	// -- create the tiles -- 
	for (int y = 0; y < tiles_height; ++y) {
	    for (int x = 0; x < tiles_width; ++x) {
		if (tiles[y][x][0] == 0 && tiles[y][x][1] == 0 && tiles[y][x][2] == 0) continue;
		else if (tiles[y][x][1] == 255 && tiles[y][x][2] == 255) {
		    Objects::AnimatedSprite *tile = new Objects::AnimatedSprite("tile", "../assets/tiles.png", 36.0f, 6.0f, 6.0f, 8, true, false, &main_shader);
		    
		    tile->self.color = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
		    tile->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		    tile->self.scale = glm::vec2(3.0f, 3.0f);
		    
		    tile->self.position = glm::vec3((x * tile->self.scale.x) - tiles_width / 2, y * tile->self.scale.y, 0.01f);
		    switch (tiles[y][x][0]) {
		        case 50:
		            tile->sprite_index = 6;
		            level_tiles.push_back(tile);
		            continue;
		        case 100:
		            tile->sprite_index = 7;
		            level_tiles.push_back(tile);
		            continue;
		        case 150:
		            tile->sprite_index = 8;
		            level_tiles.push_back(tile);
		            continue;
		        case 160:
		            tile->sprite_index = 16;
		            level_tiles.push_back(tile);
		            continue;
		        case 170:
		            tile->sprite_index = 22;
		            level_tiles.push_back(tile);
		            continue;
		    }
		    // level_tiles.push_back(tile);
		}
	    }
	}
	
	std::cout << "[INFO] level had been built. \n";
	return player_position;
    }
    
    void Application::render_level(Shader *p_shader) {
	
	for (int i = 0; i < (int)level.size(); i++) {
	    level[i]->render(p_shader);
	}
	
	Molson(_set_int)("SPRITE_FRAMES", TILES_FRAMES, true, p_shader);
	Molson(_set_int)("SPRITE_COLUMNS", TILES_COLS, true, p_shader);
	Molson(_set_int)("SPRITE_ROWS", TILES_ROWS, true, p_shader);
	for (int i = 0; i < (int)level_tiles.size(); i++) {
	    Molson(_set_int)("index", level_tiles[i]->sprite_index, true, p_shader);
	    level_tiles[i]->render(p_shader);
	}
	return;
    }
    
    void Application::destroy_level() {
	for (int i = 0; i < (int)level.size(); i++) {
	    delete level[i];
	}
	for (int i = 0; i < (int)level_tiles.size(); i++) {
	    delete level_tiles[i];
	}
	return;
    }
}
