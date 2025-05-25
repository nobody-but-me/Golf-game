
#version 330 core

layout (location = 0) in vec4 object_vertex;

uniform mat4 projection;
uniform mat4 transform;
uniform mat4 view;

uniform int time;

out vec2 texture_coords;

void main() {
     // for test purposes.
     texture_coords = object_vertex.zw / 6;
     gl_Position = projection * view * transform * vec4(object_vertex.xy, 0.0f, 1.0f);
}
