//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./background.h"
#include "../utility.h"

void Background::setup(GLContext& gl) noexcept {
  gl.create_shader_program("background", "shaders/background.vert",
                           "shaders/background.frag");
  gl.create_and_bind_vao("background");

  auto vertices = rectangle_vertices(50.0f, 5.0f, 0.0f, glm::vec3{0.0f});
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  vertex_len_ = static_cast<int>(vertices.size() * 3);
  uv_len_ = 0;

  gl.fill_enable_vbo(vertex, 0, vertices);
}

void Background::draw(GLContext& gl) noexcept {
  gl.bind_vao("background");

  glDrawArrays(GL_TRIANGLES, 0, vertex_len_);
}

GLuint Background::load_uniforms(GLContext& gl, const glm::mat4& proj,
                                 const glm::mat4& view) noexcept {
  auto shader = GameObject::load_uniforms(gl, proj, view);

  auto width_interval = glGetUniformLocation(shader, "width_interval");
  auto height_interval = glGetUniformLocation(shader, "height_interval");

  glUniform2f(width_interval, -50.0f, 50.0f);
  glUniform2f(height_interval, -5.0f, 5.0f);

  return shader;
}
