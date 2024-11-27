//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./player.h"

std::vector<glm::vec3> vertices = {
    // clang-format off
    {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {-1.0f, 1.0f, 0.0f},
    {-1.0f, 1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}
    // clang-format on
};

void Player::setup(GLContext& gl) noexcept {
  gl.create_shader_program("player", "shaders/player.vert", "shaders/player.frag");
  gl.create_and_bind_vao("player");

  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  vertex_len_ = static_cast<int>(vertices.size());
  uv_len_ = 0;

  gl.fill_enable_vbo(vertex, 0, vertices);
}

void Player::draw(GLContext& gl) noexcept {
  gl.bind_vao("player");

  glPolygonMode(GL_FRONT, GL_FILL);
  glDrawArrays(GL_POINTS, 0, vertex_len_);
}
