
#pragma once
#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <string>

#include "./molson.h"

namespace Objects {
    
    typedef struct {
	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	
	glm::mat4 transform;
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;
	
	glm::vec3 color;
    } Object; 
    
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
	    
	    Sprite(std::string p_name, std::string p_texture_path);
	    ~Sprite();
	    
	    void render(Shader *p_shader);
	    
    };
    
}

#endif//OBJECTS_H
