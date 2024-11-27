//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#version 410

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec4 fragment_color;

void main() {
    fragment_color = vec4(1.0, 0.5, 0.0, 1.0);
}
