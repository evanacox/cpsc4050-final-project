//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./player.h"
#include "../utility.h"
#include <glm/gtc/type_ptr.hpp>

void Player::setup(GLContext& gl) noexcept {
  gl.create_shader_program("player", "shaders/player.vert", "shaders/player.frag");
  gl.create_and_bind_vao("player");

  auto dims = dimensions();
  auto buffers = rectangle_vertices(dims.x, dims.y, position());
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  set_vertex_count(static_cast<int>(buffers.vertices.size()));

  gl.fill_enable_vbo(vertex, 0, buffers.vertices);
  gl.fill_enable_vbo(uv, 1, buffers.uv_coords);

  load_animations(gl);
}

GLuint Player::load_uniforms(GLContext& gl, const glm::mat4& proj,
                             const glm::mat4& view) noexcept {
  auto shader = GameObject::load_uniforms(gl, proj, view);
  auto color_uniform = glGetUniformLocation(shader, "color");

  glUniform4fv(color_uniform, 1, glm::value_ptr(color_));

  if (current_animation) {
      gl.load_texture(current_animation->texture_name);
  }

  return shader;
}

/// Constructor for Animation
Animation::Animation(int frames, float time_per_frame, glm::vec2 size)
    : frame_count(frames), frame_time(time_per_frame), frame_size(size) {
    uv_coords.reserve(frames * 6); // 6 vertices (2 triangles) per frame

    for (int i = 0; i < frames; ++i) {
        float x = (i % static_cast<int>(1.0f / size.x)) * size.x;
        float y = 1.0f - (i / static_cast<int>(1.0f / size.x)) * size.y;

        // Define two triangles for the frame
        uv_coords.push_back(glm::vec2(x, y - size.y));          // Bottom-left
        uv_coords.push_back(glm::vec2(x, y));                  // Top-left
        uv_coords.push_back(glm::vec2(x + size.x, y));         // Top-right
        uv_coords.push_back(glm::vec2(x, y - size.y));         // Bottom-left
        uv_coords.push_back(glm::vec2(x + size.x, y));         // Top-right
        uv_coords.push_back(glm::vec2(x + size.x, y - size.y)); // Bottom-right
    }
}

void Player::load_animations(GLContext& gl) {
    // Load textures for animations
    gl.create_texture("idle", "assets/character/1_Cat_Idle-Sheet.png");
    gl.create_texture("run", "assets/character/1_Cat_Run-Sheet.png");
    gl.create_texture("jump", "assets/character/1_Cat_Jump-Sheet.png");
    gl.create_texture("fall", "assets/character/1_Cat_Fall-Sheet.png");

    animations["idle"] = Animation(8, 0.2f, glm::vec2(0.125f, 0.25f));
    animations["run"] = Animation(10, 0.1f, glm::vec2(0.1f, 0.25f));
    animations["jump"] = Animation(4, 0.3f, glm::vec2(0.5f, 0.25f));
    animations["fall"] = Animation(4, 0.3f, glm::vec2(0.5f, 0.25f));
    current_animation = &animations["idle"];
}

void Player::update_animation(float delta_time) {
    if (!current_animation) return;

    // Update elapsed time
    current_animation->elapsed_time += delta_time;

    // If enough time has passed, move to the next frame
    if (current_animation->elapsed_time >= current_animation->frame_time) {
        current_animation->elapsed_time = 0.0f; // Reset elapsed time
        current_animation->current_frame =
            (current_animation->current_frame + 1) % current_animation->frame_count;

        // Update the UV buffer to reflect the new frame
        update_uv_buffer();
    }
}

void Player::set_animation(const std::string& anim_name) {
    // Find the animation by name
    auto it = animations.find(anim_name);
    if (it != animations.end() && current_animation != &it->second) {
        current_animation = &it->second;

        // Reset animation state
        current_animation->current_frame = 0;
        current_animation->elapsed_time = 0.0f;

        // Update the UV buffer for the new animation
        update_uv_buffer();
    }
}

void Player::update_uv_buffer() {
    if (!current_animation) return;

    const auto& uv_data = current_animation->uv_coords;
    int start_index = current_animation->current_frame * 6; // 6 vertices per frame

    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * 6,
                    &uv_data[start_index]);
}