
#version 330 core

in vec2 texture_coords;
out vec4 object_color;

uniform sampler2D object_image;
uniform vec3 color;
uniform bool is_textured;

void main() {
     if (is_textured) {
     	object_color = vec4(color, 1.0f) * texture(object_image, texture_coords);
     }
     else {
     	object_color = vec4(color, 1.0f);
     }
}