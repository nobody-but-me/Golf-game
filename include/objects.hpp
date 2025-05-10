
#pragma once
#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <string>

#include "./molson.h"

namespace Objects {
    
    typedef struct Object {
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	
	glm::mat4 transform;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec2 scale;
	
	glm::vec3 color;
    } Object;
    
    class Rectangle;
    class Sprite {
	private:
	    std::string texture_path;
	    bool texture_alpha;
	    bool texture_flip;
	    Texture texture;
	public:
	    bool initialized;
	    std::string name;
	    Object self;
	    
	    Sprite(std::string p_name, std::string p_texture_path, bool p_alpha);
	    ~Sprite();
	    
	    bool isCollidingRect(Rectangle *p_rect, bool p_adjust_position = false);
	    void render(Shader *p_shader);
	    
    };
    
    class Rectangle {
	public:
	    bool initialized;
	    std::string name;
	    Object self;
	    
	    Rectangle(std::string p_name);
	    ~Rectangle();
	    
	    bool isCollidingRect(Rectangle *p_object, bool p_adjust_position = false);
	    void render(Shader *p_shader);
    };
    
}

#endif//OBJECTS_H
