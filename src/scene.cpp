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
  // we compute our first camera matrix here, and store it
  static auto view = glm::lookAt(camera_position_, player_->position(), camera_up_);
  static auto prev_position = camera_position_;

  assert(player_ != nullptr && "must have player object set before first draw");

  // if we've moved the camera, we apply a translation for it and store that
  // as the new matrix. this ensures the translations are consistent
  if (prev_position != camera_position_) {
    view = glm::translate(view, camera_position_ - prev_position);
    prev_position = camera_position_;
  }

  for (auto& object : objects_) {
    object->load_uniforms(gl, proj, view);
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
    camera_position_ = camera_position_ + LEFT;
    break;
  case GLFW_KEY_RIGHT:
    player_->translate(RIGHT);
    camera_position_ = camera_position_ + RIGHT;
    break;
  case GLFW_KEY_DOWN:
    player_->translate(DOWN);
    camera_position_ = camera_position_ + DOWN;
    break;
  case GLFW_KEY_UP:
    player_->translate(UP);
    camera_position_ = camera_position_ + UP;
    break;
  default:
    break;
  }
}