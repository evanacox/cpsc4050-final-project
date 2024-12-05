//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./player.h"
#include "../utility.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Player::setup(GLContext& gl) noexcept {
  gl.create_shader_program("player", "shaders/player.vert", "shaders/player.frag");
  gl.create_and_bind_vao("player");

  load_animations(gl);

  auto dims = dimensions();
  auto buffers = rectangle_vertices(dims.x, dims.y, position());
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  uv_vbo_ = buf[1];

  set_vertex_count(static_cast<int>(buffers.vertices.size()));

  gl.fill_enable_vbo(vertex, 0, buffers.vertices);
  gl.fill_enable_vbo(uv_vbo_, 1, current_animation_->uv_coords);
}

GLuint Player::load_uniforms(GLContext& gl, const glm::mat4& proj,
                             const glm::mat4& view) noexcept {
  auto shader = GameObject::load_uniforms(gl, proj, view);
  auto sprite_sheet = glGetUniformLocation(shader, "sprite_sheet");
  auto sprite_dimensions = glGetUniformLocation(shader, "sprite_dimensions");
  auto animation_frame = glGetUniformLocation(shader, "animation_frame");
  auto mirror = glGetUniformLocation(shader, "mirror");

  gl.load_texture(current_animation_->texture_name);

  if (update_uv_on_next_draw_) {
    update_uv_on_next_draw_ = false;

    update_uv_buffer();
  }

  // set the uniform to use GL_TEXTURE0 (the sprite we loaded)
  glUniform1i(sprite_sheet, 0);
  glUniform1i(mirror, should_mirror_);
  glUniform2fv(sprite_dimensions, 1, glm::value_ptr(current_animation_->frame_size));
  glUniform1ui(animation_frame, static_cast<unsigned>(current_animation_->current_frame));

  return shader;
}

/// Constructor for Animation
Animation::Animation(int frames, glm::vec2 size, std::string texture_name) noexcept
    : frame_count(frames), frame_size(size), texture_name(std::move(texture_name)) {
  uv_coords.reserve(6); // 6 vertices (2 triangles) per frame

  // get the UV coordinates for the first sprite in the sheet
  // loaded into the VBO, we then just offset from this inside the shader
  // to move between sprite states.
  //
  // we use the order from utility.cpp:20 to be consistent
  uv_coords.emplace_back(0.0f, 1.0f);   // Top-left
  uv_coords.emplace_back(0.0f, 0.0f);   // Bottom-left
  uv_coords.emplace_back(size.x, 0.0f); // Bottom-right
  uv_coords.emplace_back(0.0f, 1.0f);   // Top-left
  uv_coords.emplace_back(size.x, 0.0f); // Bottom-right
  uv_coords.emplace_back(size.x, 1.0f); // Top-right
}

void Player::load_animations(GLContext& gl) {
  // Load textures for animations
  gl.create_texture("idle", "assets/character/1_Cat_Idle-Sheet.png");
  gl.create_texture("run", "assets/character/2_Cat_Run-Sheet.png");
  gl.create_texture("jump", "assets/character/3_Cat_Jump-Sheet.png");
  gl.create_texture("fall", "assets/character/4_Cat_Fall-Sheet.png");

  animations_["idle"] = Animation(8, glm::vec2(0.125f, 0.25f), "idle");
  animations_["run"] = Animation(10, glm::vec2(0.1f, 0.25f), "run");
  animations_["jump"] = Animation(4, glm::vec2(0.25f, 0.25f), "jump");
  animations_["fall"] = Animation(4, glm::vec2(0.25f, 0.25f), "fall");
  current_animation_ = &animations_["idle"];
}

void Player::update_animation() noexcept {
  // Update elapsed time
  current_animation_->elapsed_frames += 1;

  // If enough time has passed, move to the next frame
  if (current_animation_->elapsed_frames >= current_animation_->frame_count) {
    current_animation_->elapsed_frames = 0; // Reset elapsed time
    current_animation_->current_frame =
        (current_animation_->current_frame + 1) % current_animation_->frame_count;
  }
}

void Player::set_animation(const std::string& anim_name) {
  // Find the animation by name
  auto it = animations_.find(anim_name);

  if (it != animations_.end() && current_animation_ != &it->second) {
    current_animation_ = &it->second;

    // Reset animation state
    current_animation_->current_frame = 0;
    current_animation_->elapsed_frames = 0;

    // Update the UV buffer for the new animation
    update_uv_on_next_draw_ = true;
  }
}

void Player::update_uv_buffer() {
  const auto& uv_data = current_animation_->uv_coords;

  glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * uv_data.size(), uv_data.data());
}