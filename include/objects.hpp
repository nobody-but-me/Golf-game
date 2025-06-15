
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
	
	glm::vec4 color;
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
	    bool verbose;
	    Object self;
	    
	    Sprite(std::string p_name, std::string p_texture_path, bool p_alpha, bool p_verbose);
	    ~Sprite();
	    
	    void render(Shader *p_shader);
	    
    };
    
    class AnimatedSprite {
	private:
	    std::string texture_path;
	    bool texture_alpha;
	    bool texture_flip;
	    Texture texture;
	public:
	    bool initialized;
	    std::string name;
	    bool verbose;
	    Object self;
	    
	    int sprite_index = 0;
	    float sprite_columns;
	    float sprite_frames;
	    float sprite_rows;
	    
	    AnimatedSprite(std::string p_name, std::string p_texture_path, float p_sprite_frames, float p_sprite_columns, float p_sprite_rows, int p_sprite_index, bool p_alpha, bool p_verbose, Shader *p_shader);
	    ~AnimatedSprite();
	    
	    void render(Shader *p_shader);
    };
    
    class Rectangle {
	public:
	    bool initialized;
	    std::string name;
	    bool verbose;
	    Object self;
	    
	    Rectangle(std::string p_name, bool p_verbose);
	    ~Rectangle();
	    
	    void render(Shader *p_shader);
    };
    
}

#endif//OBJECTS_H
