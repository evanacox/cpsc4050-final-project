//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./player.h"
#include "../utility.h"

void Player::setup(GLContext& gl) noexcept {
  gl.create_shader_program("player", "shaders/player.vert", "shaders/player.frag");
  gl.create_and_bind_vao("player");

  auto vertices = rectangle_vertices(5.0f, 5.0f, 0.0f, glm::vec3{0.0f});
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  vertex_len_ = static_cast<int>(vertices.size() * 3);
  uv_len_ = 0;

  gl.fill_enable_vbo(vertex, 0, vertices);
}

void Player::draw(GLContext& gl) noexcept {
  gl.bind_vao("player");

  glDrawArrays(GL_TRIANGLES, 0, vertex_len_);
}
