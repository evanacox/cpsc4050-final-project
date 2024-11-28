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

GameObject* Scene::add_object(std::unique_ptr<GameObject> object) noexcept {
  objects_.push_back(std::move(object));

  return objects_.back().get();
}

void Scene::setup(GLContext& gl) noexcept {
  assert(player_ != nullptr && "must have player object set before first draw");

  view_matrix_ = glm::lookAt(camera_position_, player_->position(), camera_up_);

  for (auto& object : objects_) {
    object->setup(gl);
  }
}

void Scene::draw_everything(GLContext& gl, const glm::mat4& proj) noexcept {
  for (auto& object : objects_) {
    object->load_uniforms(gl, proj, view_matrix_);
    object->draw(gl);
  }
}

void Scene::handle_keypress(int key) noexcept {
  constexpr auto LEFT = glm::vec3{-1.0f, 0.0f, 0.0f};
  constexpr auto RIGHT = glm::vec3{1.0f, 0.0f, 0.0f};
  constexpr auto DOWN = glm::vec3{0.0f, -1.0f, 0.0f};
  constexpr auto UP = glm::vec3{0.0f, 1.0f, 0.0f};

  switch (key) {
  case GLFW_KEY_LEFT:
    player_->translate(LEFT);
    view_matrix_ = glm::translate(view_matrix_, LEFT);
    break;
  case GLFW_KEY_RIGHT:
    player_->translate(RIGHT);
    view_matrix_ = glm::translate(view_matrix_, RIGHT);
    break;
  case GLFW_KEY_DOWN:
    player_->translate(DOWN);
    view_matrix_ = glm::translate(view_matrix_, DOWN);
    break;
  case GLFW_KEY_UP:
    player_->translate(UP);
    view_matrix_ = glm::translate(view_matrix_, UP);
    break;
  default:
    break;
  }
}

NonUniqueGameObject::NonUniqueGameObject(glm::vec3 position,
                                         const char* object_name) noexcept
    : GameObject{position, object_name, object_name + std::to_string(counter++)} {}

int NonUniqueGameObject::counter = 0;
