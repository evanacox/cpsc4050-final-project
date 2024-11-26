//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./scene.h"
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>

void GameObject::load_uniforms(GLContext& gl, const glm::mat4& proj,
                               const glm::mat4& view) noexcept {
  // TODO: load projection and view matrix
}

void GameObject::translate(glm::vec3 by) noexcept {}

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
