//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#version 410

uniform bool is_transparent;
uniform sampler2D background_image;

in vec4 model_vertex;
in vec4 world_vertex;
in vec2 texture_uv;

out vec4 fragment_color;

void main() {
    if (is_transparent) {
        fragment_color = vec4(1.0, 1.0, 1.0, 0.0);
    } else {
        fragment_color = texture(background_image, texture_uv);
    }
}