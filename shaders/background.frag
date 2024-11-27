//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#version 410

uniform vec2 height_interval;
uniform vec2 width_interval;

in vec4 model_vertex;
in vec4 world_vertex;
in vec2 texture_uv;

out vec4 fragment_color;

float interval_to_color(vec2 interval, float value) {
    return (value - interval.x) / abs(interval.y - interval.x);
}

float interval_to_color_modded(vec2 interval, float value) {
    return (mod(value, interval.x / 5) - interval.x) / abs(interval.y - interval.x);
}

void main() {
    float r = interval_to_color_modded(width_interval, model_vertex.x);
    float g = interval_to_color(height_interval, model_vertex.y);

    fragment_color = vec4(r, g, 0.0, 1.0);
}