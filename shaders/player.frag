//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#version 410

uniform uint animation_frame;
uniform bool mirror;
uniform vec2 sprite_dimensions;
uniform sampler2D sprite_sheet;

in vec4 model_vertex;
in vec4 world_vertex;
in vec2 texture_uv;

out vec4 fragment_color;

void main() {
    float frame = float(animation_frame);
    float u;
    float v = texture_uv.y;

    if (mirror) {
        u = sprite_dimensions.x * (frame + 1.0) - texture_uv.x;
    } else {
        u = texture_uv.x + sprite_dimensions.x * frame;
    }

    fragment_color = texture(sprite_sheet, vec2(u, v));
}

