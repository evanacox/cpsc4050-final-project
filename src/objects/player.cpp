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

  auto props = rectangle_vertices(5.0f, 5.0f, position());
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  for (auto i = 0u; i < props.corners.size(); ++i) {
    corners_[i] = glm::vec2(props.corners[i]);
  }

  set_vertex_count(static_cast<int>(props.triangle_vertices.size()));

  gl.fill_enable_vbo(vertex, 0, props.triangle_vertices);
}
