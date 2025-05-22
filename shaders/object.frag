
#version 330 core

in vec2 texture_coords;
out vec4 object_color;

uniform sampler2D object_image;
uniform bool is_textured;
uniform vec4 color;

void main() {
     if (is_textured) {
     	object_color = color * texture(object_image, texture_coords);
     }
     else {
     	object_color = color;
     }
}