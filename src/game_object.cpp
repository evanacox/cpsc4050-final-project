//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./game_object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint GameObject::load_uniforms(GLContext& gl, const glm::mat4& proj,
                                 const glm::mat4& view) noexcept {
  auto shader = gl.use_program(shader_name_);
  auto model = glm::translate(glm::mat4{1.0f}, -position_);

  auto model_uniform = glGetUniformLocation(shader, "model");
  auto view_uniform = glGetUniformLocation(shader, "view");
  auto proj_uniform = glGetUniformLocation(shader, "proj");

  glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, glm::value_ptr(proj));

  return shader;
}

void GameObject::draw(GLContext& gl) noexcept {
  gl.bind_vao(vao_name_);

  glDrawArrays(GL_TRIANGLES, 0, vertex_count() * 3);
}

NonUniqueGameObject::NonUniqueGameObject(glm::vec3 position,
                                         const char* object_name) noexcept
    : GameObject{position, object_name, object_name + std::to_string(counter++)} {}

int NonUniqueGameObject::counter = 0;
