
#include <iostream>
#include <string>
#include <math.h>

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
    Sprite::Sprite(std::string p_name, std::string p_texture_path, bool p_alpha) {
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
	    texture = Molson(_load_texture)(texture_path.c_str(), p_alpha);
	} else {
	    texture_path = "";
	}
	
	initialized = 1;
	std::cout << "[INFO]: Sprite " << name << " had been created." << std::endl;
	return;
    }
    
    static void adjustPositionRect(Object *p_self, Rectangle *p_rect) {
	// TODO: if object or p_rect Z position change, this will not be recognized. What it means: if an object is closer to the camera, the follow script will not consider this, counting only the scale of the both objects, causing the "main object" to "enter" the p_rect -- it's not really entering since since they are in different layers of Z position.
	const glm::vec2 SELF_CENTER = glm::vec2( p_self->position[0] + p_self->scale[0] / 2, p_self->position[1] + p_self->scale[1] / 2);
	const glm::vec2 RECT_CENTER = glm::vec2( p_rect->self.position[0] + p_rect->self.scale[0] / 2, p_rect->self.position[1] + p_rect->self.scale[1] / 2);
	
	const glm::vec2 SELF_HALF_SIZE = glm::vec2( p_self->scale[0]      * 0.5f, p_self->scale[1]      * 0.5f );
	const glm::vec2 RECT_HALF_SIZE = glm::vec2( p_rect->self.scale[0] * 0.5f, p_rect->self.scale[1] * 0.5f );
	const glm::vec2 DISTANCE = SELF_CENTER - RECT_CENTER;
	const float X_MIN_DIST = SELF_HALF_SIZE[0] + RECT_HALF_SIZE[0] - fabsf(DISTANCE[0]);
	const float Y_MIN_DIST = SELF_HALF_SIZE[1] + RECT_HALF_SIZE[1] - fabsf(DISTANCE[1]);
	
	if (X_MIN_DIST < Y_MIN_DIST) {
	    p_self->position[0] += copysignf(X_MIN_DIST, DISTANCE[0]);
	} else {
	    p_self->position[1] += copysignf(Y_MIN_DIST, DISTANCE[1]);
	}
    }
    static bool rectCollisionDetection(Object *p_self, Rectangle *p_rect) {
	if (
	    p_self->position[0] + p_self->scale[0] > p_rect->self.position[0]         &&
	    p_self->position[0] < p_rect->self.position[0] + p_rect->self.scale[0] &&
	    p_self->position[1] + p_self->scale[1] > p_rect->self.position[1]         &&
	    p_self->position[1] < p_rect->self.position[1] + p_rect->self.scale[1]
	) {
	    return true;
	}
	return false;
    }
    bool Sprite::isCollidingRect(Rectangle *p_rect, bool p_adjust_position) {
	if (rectCollisionDetection(&self, p_rect)) {
	    if (p_adjust_position == true) {
		adjustPositionRect(&self, p_rect);
	    }
	    return true;
	}
	return false;
    }
    
    void Sprite::render(Shader *p_shader) {
	Molson(_use)(p_shader);
	
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(self.position));
	
	transform = glm::translate(transform, glm::vec3(0.5f * self.scale[0], 0.5f * self.scale[1], 0.0f));
	transform = glm::rotate(transform, glm::radians(self.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(self.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(self.rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, glm::vec3(-0.5f * self.scale[0], -0.5f * self.scale[1], 0.0f));
	
	transform = glm::scale(transform, glm::vec3(self.scale, 1.0f));
	
	Molson(_set_matrix4)("transform", &transform, false, p_shader);
	Molson(_set_vector3_f)("color", self.color / 255.0f, false, p_shader);
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
    
    bool Rectangle::isCollidingRect(Rectangle *p_rect, bool p_adjust_position) {
	if (rectCollisionDetection(&self, p_rect)) {
	    if (p_adjust_position == true) {
		adjustPositionRect(&self, p_rect);
	    }
	    return true;
	}
	return false;
    }
    
    void Rectangle::render(Shader *p_shader) {
	Molson(_use)(p_shader);
	
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(self.position));
	
	transform = glm::translate(transform, glm::vec3(0.5f * self.scale[0], 0.5f * self.scale[1], 0.0f));
	transform = glm::rotate(transform, glm::radians(self.rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(self.rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(self.rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, glm::vec3(-0.5f * self.scale[0], -0.5f * self.scale[1], 0.0f));
	
	transform = glm::scale(transform, glm::vec3(self.scale, 1.0f));
	
	Molson(_set_matrix4)("transform", &transform, false, p_shader);
	Molson(_set_vector3_f)("color", self.color / 255.0f, false, p_shader);
	Molson(_set_bool)("is_textured", false, p_shader);
	
	glBindVertexArray(self.vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	return;
    }
}
