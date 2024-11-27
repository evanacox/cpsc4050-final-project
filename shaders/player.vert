//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_uv;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec4 model_vertex;
out vec4 world_vertex;
out vec2 texture_uv;

void main() {
    model_vertex = vec4(position, 1.0);
    world_vertex = model * model_vertex;
    texture_uv = tex_uv;

    gl_Position = proj * view * world_vertex;
}
