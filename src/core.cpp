
#include <iostream>
#include <string>
#include <vector>

#include "../include/glad.h"
#include <GLFW/glfw3.h>


#include "../include/objects.hpp"
#include "../include/stb_image.h"
#include "../include/core.hpp"
#define MOLSON_IMPLEMENTATION
#include "../include/molson.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace Core {
    
    const int *Application::getWindowHeight() {
	return &HEIGHT;
    }
    const int *Application::getWindowWidth() {
	return &WIDTH;
    }
    Shader *Application::getMainShader() {
	return &main_shader;
    }
    GLFWwindow *Application::getWindow() {
	return window;
    }
    
    bool Application::isKeyPressed(int p_key) {
	int state = glfwGetKey(window, p_key);
	if (state == GLFW_PRESS) {
	    return true;
	}
	return false;
    }
    bool Application::isKeyJustPressed(int p_key) {
	int state = glfwGetKey(window, p_key);
	if (state == GLFW_RELEASE) {
	    return true;
	}
	return false;
    }
    bool Application::isRunning() {
	return running;
    }
    
    static void window_resized_callback(GLFWwindow *window, int w, int h) {
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	return;
    }
    Application::Application(std::string p_title) {
	if (!glfwInit()) {
	    std::cout << "[FAILED]: OpenGL library could not be loaded. \n" << std::endl;
	    return;
	}
	// Decide GL+GLSL versions
	#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100 (WebGL 1.0)
	// const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	#elif defined(IMGUI_IMPL_OPENGL_ES3)
	// GL ES 3.0 + GLSL 300 es (WebGL 2.0)
	// const char* glsl_version = "#version 300 es";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	// const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
	#else
	// GL 3.0 + GLSL 130
	// const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	#endif
	
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
	glEnable(GL_DEPTH_TEST);
	
	Molson(_init_shader)("./shaders/object.vert", "./shaders/object.frag", &main_shader);
	
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
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
    
    
    std::vector<Objects::Rectangle*> level;
    
    std::vector<Objects::Rectangle*> &Application::getLevel() {
	return level;
    }
    void Application::buildLevel(std::string p_level_path) {
	int width, height, channels;
	unsigned char *data = stbi_load(p_level_path.c_str(), &width, &height, &channels, 3);
	
	/* 
	a[ 
	    b[ c[255, 255, 255], c[255, 0, 0], c[0, 0, 255] ],
	    b[ c[255, 255, 255], c[255, 0, 0], c[0, 0, 255] ],
	    b[ c[255, 255, 255], c[255, 0, 0], c[0, 0, 255] ],
	]; 
	*/
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
	std::vector<std::vector<std::vector<unsigned int>>> matrix(height, std::vector<std::vector<unsigned int>>(width, std::vector<unsigned int>(3, 0)));
	int cursor = 0;
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width; x++) {
		matrix[y][x] = RGB[cursor++];
	    }
	}
	
	for (int y = 0; y < height; ++y) {
	    for (int x = 0; x < width; ++x) {
		// std::cout << "R :: " << matrix[y][x][0] << std::endl; yeah;
		// std::cout << "G :: " << matrix[y][x][1] << std::endl; yeah;
		// std::cout << "B :: " << matrix[y][x][2] << std::endl; yeah;
		if (matrix[y][x][0] == 0 && matrix[y][x][1] == 0 && matrix[y][x][2] == 255) {
		    Objects::Rectangle *block = new Objects::Rectangle("block", false);
	
		    block->self.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		    block->self.color = glm::vec3(42.0f, 93.0f, 232.0f);
		    block->self.scale = glm::vec2(3.0f, 3.0f);
		
		    block->self.position = glm::vec3((x * block->self.scale.x) * -1, (y * block->self.scale.y) * -1, 0.0f);
		    level.push_back(block);
		}
	    }
	}
	std::cout << "[INFO] level had been built.";
	return;
    }
    void Application::renderLevel(Shader *p_shader) {
	for (int i = 0; i < (int)level.size(); i++) {
	    level[i]->render(p_shader);
	}
	return;
    }
    void Application::destroyLevel() {
	for (int i = 0; i < (int)level.size(); i++) {
	    delete level[i];
	}
	return;
    }
}
