//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./scene.h"
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void GameObject::load_uniforms(GLContext& gl, const glm::mat4& proj,
                               const glm::mat4& view) noexcept {
  auto shader = gl.use_program(shader_name_);
  auto model = glm::translate(glm::mat4{1.0f}, -position_);

  auto model_uniform = glGetUniformLocation(shader, "model");
  auto view_uniform = glGetUniformLocation(shader, "view");
  auto proj_uniform = glGetUniformLocation(shader, "proj");

  glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(proj_uniform, 1, GL_FALSE, glm::value_ptr(proj));
}

GameObject* Scene::add_object(std::unique_ptr<GameObject> object) noexcept {
  objects_.push_back(std::move(object));

  return objects_.back().get();
}

void Scene::setup(GLContext& gl) noexcept {
  for (auto& object : objects_) {
    object->setup(gl);
  }
}

void Scene::draw_everything(GLContext& gl, const glm::mat4& proj) noexcept {
  assert(player_ != nullptr && "must have player object set before first draw");

  auto view = glm::lookAt(camera_position_, player_->position(), camera_up_);

  for (auto& object : objects_) {
    object->load_uniforms(gl, proj, view);
    object->draw(gl);
  }
}
