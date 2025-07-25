
#ifndef MOLSON_H
#define MOLSON_H
#define Molson(x) molson##x

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <stdbool.h>
#include <string>


// --------------------------------------------------

typedef struct {
    unsigned int internal_format;
    unsigned int image_format;
    unsigned int filter_min;
    unsigned int filter_max;
    unsigned int wrap_s;
    unsigned int wrap_t;
    unsigned int height;
    unsigned int width;
    unsigned int id;
    
    std::string path;
} Texture;

typedef struct {
    unsigned int _fragment_shader;
    unsigned int _vertex_shader;
    unsigned int ID;
} Shader;

Texture Molson(_load_texture)(const char *_file_path, bool _alpha);
const char *Molson(_file_to_string)(const char *_file_path);

void Molson(_init_shader)(const char *_vertex_path, const char *_fragment_path, Shader *_shader);
void Molson(_set_bool)(const char *_name, bool _value, Shader *_shader);
void Molson(_destroy)(Shader *_shader);
void Molson(_use)(Shader *_shader);

void Molson(_set_matrix4)(const char *_name, const glm::mat4 *_value, bool _use_shader, Shader *_shader);
void Molson(_set_vector2_f)(const char *_name, glm::vec2 _value, bool _use_shader, Shader *_shader);
void Molson(_set_vector3_f)(const char *_name, glm::vec3 _value, bool _use_shader, Shader *_shader);
void Molson(_set_vector4_f)(const char *_name, glm::vec4 _value, bool _use_shader, Shader *_shader);
void Molson(_set_int)(const char *_name, int _value, bool _use_shader, Shader *_shader);
void Molson(_set_float)(const char *_name, float _value, bool _use_shader, Shader *_shader);

// --------------------------------------------------

#endif//MOLSON_H
#ifdef MOLSON_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "./glad.h"
#include <GLFW/glfw3.h>


const char *Molson(_file_to_string)(const char *_file_path) {
    long _buffer_size = 0;
    char *_buffer;
    FILE *_file;
    
    _file = fopen(_file_path, "r");
    if (_file == NULL) {
	printf("[FAILED]: File could not be loaded. \n");
	fclose(_file);
	return NULL;
    }
    if (fseek(_file, 0, SEEK_END) < 0) goto BLANK;
    _buffer_size = ftell(_file);
    _buffer = (char *)malloc(_buffer_size + 1);
    if (_buffer == NULL) {
	printf("[INFO]: Buffer could not be read. \n ");
	return NULL;
    }
    
    if (fseek(_file, 0, SEEK_SET) < 0) goto BLANK;
    fread(_buffer, 1, _buffer_size, _file);
    if (ferror(_file)) {
	printf("[INFO]: File could not be read. \n");
	return NULL;
    }
    fclose(_file);
    
    printf("[INFO]: File have been loaded sucessfully. \n");
    return _buffer;
    
BLANK:
    printf("[INFO]: File is blank. \n");
    fclose(_file);
    
    _buffer = (char *)malloc(sizeof(char));
    free(_buffer);
    return NULL;
}

static void init_texture(Texture *texture) {
    texture->height = 0;
    texture->width = 0;
    texture->image_format = GL_RGB;
    texture->wrap_s = GL_REPEAT;
    texture->wrap_t = GL_REPEAT;
    texture->filter_min = GL_LINEAR;
    texture->filter_max = GL_LINEAR;
    
    glGenTextures(1, &texture->id);
    return;
}

static void generate_texture(unsigned int width, unsigned int height, unsigned char *data, Texture *texture) {
    texture->height = height;
    texture->width = width;
    
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->internal_format, width, height, 0, texture->image_format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filter_max);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    return;
}

Texture Molson(_load_texture)(const char *_file_path, bool _alpha) {
    Texture _new_texture;
    init_texture(&_new_texture);
    if (_alpha) {
	_new_texture.internal_format = GL_RGBA;
	_new_texture.image_format = GL_RGBA;
    }
    int _channels;
    int _height, _width;
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char *_data = stbi_load(_file_path, &_width, &_height, &_channels, 0);
    generate_texture(_width, _height, _data, &_new_texture);
    stbi_image_free(_data);
    
    _new_texture.path = _file_path;
    // printf("[INFO]: Texture had been loaded successfully. \n");
    return _new_texture;
}

// --------------------------------------------------

void Molson(_init_shader)(const char *_vertex_path, const char *_fragment_path, Shader *_shader) {

    long _buffer_length = 0;
    FILE *_fragment_code;
    FILE *_vertex_code;
    
    char *_fragment_buffer;
    char *_vertex_buffer;

    _fragment_code = fopen(_fragment_path, "r");
    if (_fragment_code == NULL) {
	printf("[FAILED]: Fragment shader file could not be loaded. \n");
	fclose(_fragment_code);
	_shader->ID = -1;
	return;
    }
    if (fseek(_fragment_code, 0, SEEK_END) < 0) goto BLANK;
    
    _buffer_length = ftell(_fragment_code);
    _fragment_buffer = (char *)malloc(_buffer_length + 1);
    if (_fragment_buffer == NULL) {
	printf("[FAILED]: Fragment shader variable could not be allocated. \n");
	fclose(_fragment_code);
	_shader->ID = -1;
	return;
    }
    if (fseek(_fragment_code, 0, SEEK_SET) < 0) goto BLANK;
    fread(_fragment_buffer, 1, _buffer_length, _fragment_code);
    if (ferror(_fragment_code)) {
	printf("[FAILED]: Fragment shader file could not be read. \n");
	fclose(_fragment_code);
	_shader->ID = -1;
	return;
    }
    _fragment_buffer[_buffer_length] = '\0';

    
    _vertex_code = fopen(_vertex_path, "r");
    if (_vertex_code == NULL) {
	printf("[FAILED]: Vertex shader file could not be loaded. \n");
	fclose(_vertex_code);
	_shader->ID = -1;
	return;
    }
    if (fseek(_vertex_code, 0, SEEK_END) < 0) goto BLANK;
    
    _buffer_length = ftell(_vertex_code);
    _vertex_buffer = (char *)malloc(_buffer_length + 1);
    if (_vertex_buffer == NULL) {
	printf("[FAILED]: Vertex shader variable could not be allocated. \n");
	fclose(_vertex_code);
	_shader->ID = -1;
	return;
    }
    
    if (fseek(_vertex_code, 0, SEEK_SET < 0)) goto BLANK;
    fread(_vertex_buffer, 1, _buffer_length, _vertex_code);
    if (ferror(_vertex_code)) {
	printf("[FAILED]: Fragment shader file could not be read. \n");
	fclose(_vertex_code);
	_shader->ID = -1;
	return;
    }
    _vertex_buffer[_buffer_length] = '\0';
    
    if /*yet*/(_fragment_code) {
	fclose(_fragment_code);
    }
    if /*yet*/(_vertex_code) {
	fclose(_vertex_code);
    }
    
    char _info_log[512];
    int _success;
    
    _shader->_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_shader->_vertex_shader, 1, (GLchar const * const *)&_vertex_buffer, NULL);
    glCompileShader(_shader->_vertex_shader);
    glGetShaderiv(_shader->_vertex_shader, GL_COMPILE_STATUS, &_success);
    if (!_success) {
	glGetShaderInfoLog(_shader->_vertex_shader, 512, NULL, _info_log);
	printf("[FAILED]: Vertex shader compilation failed. \n");
	printf("[INFO_LOG]: %s. \n", _info_log);
	return;
    }
    printf("[INFO]: Vertex shader compiled successfully. \n");
    
    _shader->_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_shader->_fragment_shader, 1, (GLchar const * const *)&_fragment_buffer, NULL);
    glCompileShader(_shader->_fragment_shader);
    glGetShaderiv(_shader->_fragment_shader, GL_COMPILE_STATUS, &_success);
    if (!_success) {
	glGetShaderInfoLog(_shader->_fragment_shader, 512, NULL, _info_log);
	printf("[FAILED]: Fragment shader compilation failed. \n");
	printf("[INFO_LOG]: %s. \n", _info_log);
	return;
    }
    printf("[INFO]: Fragment shader compiled successfully. \n");
    
    _shader->ID = glCreateProgram();
    glAttachShader(_shader->ID, _shader->_fragment_shader);
    glAttachShader(_shader->ID, _shader->_vertex_shader);
    glLinkProgram(_shader->ID);
    glGetProgramiv(_shader->ID, GL_LINK_STATUS, &_success);
    if (!_success) {
	glGetProgramInfoLog(_shader->ID, 512, NULL, _info_log);
	printf("[FAILED]: Shader program linking failed. \n");
	printf("[INFO_LOG]: %s. \n", _info_log);
	return;
    }
    printf("[INFO]: Shader program had been linked successfully. \n");
    return;
    
BLANK:
    printf("[INFO]: Shader file is blank. \n");
    if (_fragment_code) {
	fclose(_fragment_code);
    }
    else if (_vertex_code) {
	fclose(_vertex_code);
    }
    _shader->ID = -1;
    return;
}

void Molson(_destroy)(Shader *_shader) {
    glDeleteShader(_shader->_fragment_shader);
    glDeleteShader(_shader->_vertex_shader);
    glDeleteProgram(_shader->ID);
    printf("[INFO]: Shaders deleted successfully. \n");
    return;
}

void Molson(_use)(Shader *_shader) {
    glUseProgram(_shader->ID);
    return;
}

void Molson(_set_matrix4)(const char *_name, const glm::mat4 *_value, bool _use_shader, Shader *_shader) {
    if (_use_shader) {
	glUseProgram(_shader->ID);
    }
    glUniformMatrix4fv(glGetUniformLocation(_shader->ID, _name), 1, false, (float *)_value);
    return;
}

void Molson(_set_vector3_f)(const char *_name, glm::vec3 _value, bool _use_shader, Shader *_shader) {
    if (_use_shader) {
	glUseProgram(_shader->ID);
    }
    glUniform3f(glGetUniformLocation(_shader->ID, _name), _value[0], _value[1], _value[2]);
    return;
}
void Molson(_set_vector4_f)(const char *_name, glm::vec4 _value, bool _use_shader, Shader *_shader) {
    if (_use_shader) {
	glUseProgram(_shader->ID);
    }
    glUniform4f(glGetUniformLocation(_shader->ID, _name), _value[0], _value[1], _value[2], _value[3]);
    return;
}

void Molson(_set_int)(const char *_name, int _value, bool _use_shader, Shader *_shader) {
    if (_use_shader) {
	glUseProgram(_shader->ID);
    }
    glUniform1i(glGetUniformLocation(_shader->ID, _name), _value);
    return;
}
void Molson(_set_float)(const char *_name, float _value, bool _use_shader, Shader *_shader) {
    if (_use_shader) {
	glUseProgram(_shader->ID);
    }
    glUniform1f(glGetUniformLocation(_shader->ID, _name), _value);
    return;
}
void Molson(_set_bool)(const char *_name, bool _value, Shader *_shader) {
    glUniform1i(glGetUniformLocation(_shader->ID, _name), _value);
    return;
}

// --------------------------------------------------

#include <vector>
std::vector<std::vector<unsigned int>> Molson(readRGBImage)(std::string p_filepath) {
    int width, height, channels;
    unsigned char *data = stbi_load(p_filepath.c_str(), &width, &height, &channels, 3);
    
    // const unsigned int RGB_SIZE = width * height;
    std::vector<std::vector<unsigned int>> d;
    if (data != nullptr && width > 0 && height > 0) {
	for (unsigned int i = 0; i < (unsigned int)(width * height); i++) {
	    std::vector<unsigned int> rgb;
	    rgb.push_back(data[i * 3]);
	    rgb.push_back(data[i * 3 + 1]);
	    rgb.push_back(data[i * 3 + 2]);
	    d.push_back(rgb);
	}
	//     printf("[MOLSON]: x: %d, y: %d; \n", x, y); // printing again
	//     // printf("[MOLSON]: R: %d; G: %d; B: %d; \n", data[i], data[i + 1], data[i + 2]); // printing
	stbi_image_free(data);
	return d;
    }
    printf("[MOLSON]: something went wrong about getting image colours.");
    stbi_image_free(data);
    return d;
}


#endif




