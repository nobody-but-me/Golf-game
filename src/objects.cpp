
#include <iostream>
#include <string>

#include "../include/glad.h"
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "../include/objects.hpp"
#include "../include/molson.h"
#define MOLSON_IMPLEMENTATION

namespace Objects {
    
    Sprite::~Sprite() {
	glDeleteVertexArrays(1, &self.vao);
	glDeleteBuffers(1, &self.vbo);
	glDeleteBuffers(1, &self.ebo);
	
	std::cout << "[INFO]: Sprite had been destroyed successfully. " << std::endl;
	return;
    }
    Sprite::Sprite(std::string p_name, std::string p_texture_path, bool alpha) {
	float vertices[] = {
	    0.0f, 1.0f, 0.0f, 1.0f,
	    1.0f, 0.0f, 1.0f, 0.0f,
	    0.0f, 0.0f, 0.0f, 0.0f, 
	    
	    0.0f, 1.0f, 0.0f, 1.0f,
	    1.0f, 1.0f, 1.0f, 1.0f,
	    1.0f, 0.0f, 1.0f, 0.0f
	};
	name = p_name;
	
	glGenVertexArrays(1, &self.vao);
	glGenBuffers(1, &self.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, self.vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(self.vao);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	if (p_texture_path != "") {
	    texture_path = p_texture_path;
	    texture = Molson(_load_texture)(texture_path.c_str(), alpha);
	} else {
	    texture_path = "";
	}
	
	initialized = 1;
	std::cout << "[INFO]: Sprite " << name << " had been created." << std::endl;
	return;
    }
    
    void Sprite::render(Shader *p_shader) {
	Molson(_use)(p_shader);
	
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(self.position, 0.0f));
	
	transform = glm::translate(transform, glm::vec3(0.5f * self.scale[0], 0.5f * self.scale[1], 0.0f));
	transform = glm::rotate(transform, glm::radians(self.rotation), glm::vec3(0.0f, 1.0f, 1.0f));
	transform = glm::translate(transform, glm::vec3(-0.5f * self.scale[0], -0.5f * self.scale[1], 0.0f));
	
	transform = glm::scale(transform, glm::vec3(self.scale, 1.0f));
	
	Molson(_set_matrix4)("transform", &transform, false, p_shader);
	Molson(_set_vector3_f)("color", self.color, false, p_shader);
	Molson(_set_bool)("is_textured", true, p_shader);
	
	if (texture_path != "") {
	    glActiveTexture(GL_TEXTURE0);
	    glBindTexture(GL_TEXTURE_2D, texture.id);
	}
	
	glBindVertexArray(self.vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	return;
    }
    
    Rectangle::Rectangle(std::string p_name) {
	float vertices[] = {
	    0.0f, 1.0f, 0.0f, 1.0f,
	    1.0f, 0.0f, 1.0f, 0.0f,
	    0.0f, 0.0f, 0.0f, 0.0f, 
	    
	    0.0f, 1.0f, 0.0f, 1.0f,
	    1.0f, 1.0f, 1.0f, 1.0f,
	    1.0f, 0.0f, 1.0f, 0.0f
	};
	name = p_name;
	
	glGenVertexArrays(1, &self.vao);
	glGenBuffers(1, &self.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, self.vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindVertexArray(self.vao);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	initialized = 1;
	std::cout << "[INFO]: Rectangle " << name << " had been created." << std::endl;
	return;
    }
    
    Rectangle::~Rectangle() {
	glDeleteVertexArrays(1, &self.vao);
	glDeleteBuffers(1, &self.vbo);
	glDeleteBuffers(1, &self.ebo);
	
	std::cout << "[INFO]: Rectangle had been destroyed successfully. " << std::endl;
	return;
    }
    
    void Rectangle::render(Shader *p_shader) {
	Molson(_use)(p_shader);
	
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(self.position, 0.0f));
	
	transform = glm::translate(transform, glm::vec3(0.5f * self.scale[0], 0.5f * self.scale[1], 0.0f));
	transform = glm::rotate(transform, glm::radians(self.rotation), glm::vec3(0.0f, 1.0f, 1.0f));
	transform = glm::translate(transform, glm::vec3(-0.5f * self.scale[0], -0.5f * self.scale[1], 0.0f));
	
	transform = glm::scale(transform, glm::vec3(self.scale, 1.0f));
	
	Molson(_set_matrix4)("transform", &transform, false, p_shader);
	Molson(_set_vector3_f)("color", self.color, false, p_shader);
	Molson(_set_bool)("is_textured", false, p_shader);
	
	glBindVertexArray(self.vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	return;
    }
}
