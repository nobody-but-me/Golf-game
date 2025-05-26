
#version 330 core

in vec2 texture_coords;
out vec4 object_color;

uniform sampler2D object_image;
uniform bool is_textured;
uniform vec4 color;

uniform int time;

const float SPRITE_COLUMNS = 6.0;
const float SPRITE_ROWS = 6.0;
const int NUM_OF_SPRITES = 36;

void main() {
     int sprite_idx = time;
     vec2 pos = vec2(sprite_idx % int(SPRITE_COLUMNS), int(sprite_idx / SPRITE_COLUMNS));
     
     if (is_textured) {
     	// object_color = color * texture(object_image, texture_coords);
	// object_color = color * texture(object_image, vec2((texture_coords.x / SPRITE_COLUMNS) + pos.x * (1.0 / SPRITE_COLUMNS), (texture_coords.y / SPRITE_ROWS) + pos.y * (1.0 / SPRITE_ROWS)));
	
        object_color = color * texture(object_image, vec2((texture_coords.x / SPRITE_COLUMNS) + pos.x * (1.0 / SPRITE_COLUMNS), (texture_coords.y / SPRITE_ROWS) + pos.y * (1.0 / SPRITE_ROWS)));
	
     }
     else {
     	object_color = color;
     }
}
