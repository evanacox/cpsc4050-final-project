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

  auto dims = dimensions();
  auto vertices = rectangle_vertices(dims.x, dims.y, position());
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  set_vertex_count(static_cast<int>(vertices.size()));

  gl.fill_enable_vbo(vertex, 0, vertices);
}
