//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./scene.h"
#include "./objects/background.h"
#include "./objects/player.h"
#include "./objects/rectangle.h"
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

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

void Scene::setup(GLContext& gl) noexcept {
  create_scene();

  view_matrix_ = glm::lookAt(camera_position_, player().position(), camera_up_);

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
    player().translate(LEFT);
    view_matrix_ = glm::translate(view_matrix_, LEFT);
    break;
  case GLFW_KEY_RIGHT:
    player().translate(RIGHT);
    view_matrix_ = glm::translate(view_matrix_, RIGHT);
    break;
  case GLFW_KEY_DOWN:
    player().translate(DOWN);
    view_matrix_ = glm::translate(view_matrix_, DOWN);
    break;
  case GLFW_KEY_UP:
    player().translate(UP);
    view_matrix_ = glm::translate(view_matrix_, UP);
    break;
  default:
    break;
  }
}

namespace {

// this array has pairs of (center point, dimension). dimension = (width, height)
auto backgrounds = std::array{
    // clang-format off
    std::pair{glm::vec3{0.0f, 0.0f, 0.5f}, glm::vec2{500.0f, 30.0f}},
    std::pair{glm::vec3{0.0f, 10.0f, 5.0f}, glm::vec2{500.0f, 80.0f}}
    // clang-format on
};

// same structure here for the rectangular obstacles
auto obstacles = std::array{
    // clang-format off
    std::pair{glm::vec3{0.0f, -5.0f, 0.0f}, glm::vec2{500.0f, 10.0f}}
    // clang-format on
};

} // namespace

void Scene::create_scene() noexcept {
  // create the player
  objects_.push_back(std::make_unique<Player>(glm::vec3{0.0f, 0.0f, 0.0f}));

  for (const auto& [center, dimension] : backgrounds) {
    auto background = std::make_unique<Background>(center, dimension);

    objects_.push_back(std::move(background));
  }

  obstacles_begin_ = objects_.size();

  for (const auto& [center, dimension] : obstacles) {
    auto obstacle = std::make_unique<Rectangle>(center, dimension);

    objects_.push_back(std::move(obstacle));
  }
}

NonUniqueGameObject::NonUniqueGameObject(glm::vec3 position,
                                         const char* object_name) noexcept
    : GameObject{position, object_name, object_name + std::to_string(counter++)} {}

int NonUniqueGameObject::counter = 0;
