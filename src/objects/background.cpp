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
  gl.create_and_bind_vao(vao_name());
  gl.create_texture(texture_name());

  auto buffers = rectangle_vertices(dimension_.x, dimension_.y, glm::vec3{0.0f});
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  set_vertex_count(static_cast<int>(buffers.vertices.size()));

  gl.fill_enable_vbo(vertex, 0, buffers.vertices);
  gl.fill_enable_vbo(uv, 1, buffers.uv_coords);
}

GLuint Background::load_uniforms(GLContext& gl, const glm::mat4& proj,
                                 const glm::mat4& view) noexcept {
  auto shader = GameObject::load_uniforms(gl, proj, view);
  gl.load_texture(texture_name());

  auto width_interval = glGetUniformLocation(shader, "width_interval");
  auto height_interval = glGetUniformLocation(shader, "height_interval");
  auto tex = glGetUniformLocation(shader, "background_image");

  glUniform2f(width_interval, -50.0f, 50.0f);
  glUniform2f(height_interval, -5.0f, 5.0f);
  glUniform1i(tex, 0);

  return shader;
}
