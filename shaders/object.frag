
#version 330 core

in vec2 texture_coords;
out vec4 object_color;

uniform sampler2D object_image;
uniform bool is_textured;
uniform bool is_animated;
uniform vec4 color;


uniform int SPRITE_COLUMNS;
uniform int SPRITES_FRAMES;
uniform int SPRITE_ROWS;

uniform int index;

void main() {
     int sprite_idx = index;
     vec2 pos = vec2(sprite_idx % int(SPRITE_COLUMNS), int(sprite_idx / SPRITE_COLUMNS) * -1);
     
     if (is_textured) {
     	if (!is_animated) {
	  object_color = color * texture(object_image, texture_coords);
	} else {
	  object_color = color * texture(object_image, vec2((texture_coords.x / SPRITE_COLUMNS) + pos.x * (1.0 / SPRITE_COLUMNS), (texture_coords.y / SPRITE_ROWS) + pos.y * (1.0 / SPRITE_ROWS)));
	}
     }
     else {
     	object_color = color;
     }
}
