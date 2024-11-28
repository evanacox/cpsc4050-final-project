//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#version 410

uniform vec4 color;

in vec4 model_vertex;
in vec4 world_vertex;
in vec2 texture_uv;

out vec4 fragment_color;

void main() {
    fragment_color = color;
}
