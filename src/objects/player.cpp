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
}

GLuint Player::load_uniforms(GLContext& gl, const glm::mat4& proj,
                             const glm::mat4& view) noexcept {
  auto shader = GameObject::load_uniforms(gl, proj, view);
  auto color_uniform = glGetUniformLocation(shader, "color");

  glUniform4fv(color_uniform, 1, glm::value_ptr(color_));

  return shader;
}

void Player::load_animations() {
    animations["idle"] = Animation(8, 0.2f, glm::vec2(0.25f, 0.25f), "assets/character/1_Cat_Idle-Sheet.png");
    animations["run"] = Animation(10, 0.1f, glm::vec2(0.166f, 0.25f), "assets/character/1_Cat_Run-Sheet.png");
    animations["jump"] = Animation(4, 0.3f, glm::vec2(0.5f, 0.25f), "assets/character/1_Cat_Jump-Sheet.png");
    animations["fall"] = Animation(4, 0.3f, glm::vec2(0.5f, 0.25f), "assets/character/1_Cat_Fall-Sheet.png");
    current_animation = &animations["idle"];
}
