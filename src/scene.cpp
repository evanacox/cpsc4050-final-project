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

  auto translation = glm::vec3{};

  switch (key) {
  case GLFW_KEY_LEFT:
    translation = LEFT;
    break;
  case GLFW_KEY_RIGHT:
    translation = RIGHT;
    break;
  case GLFW_KEY_DOWN:
    translation = DOWN;
    break;
  case GLFW_KEY_UP:
    translation = UP;
    break;
  default:
    return;
  }

  player().set_color(glm::vec4{0.7f, 0.3f, 0.6f, 1.0f});
  player().translate(translation);

  for (auto i = obstacles_begin_; i < objects_.size(); ++i) {
    // we know this is safe, every obstacle is a rectangle
    auto& rect = static_cast<Rectangle&>(*objects_[i]);
    auto [location, amount_less_to_move] = rect.collides_with(player(), translation);

    if (location != CollisionLocation::none && amount_less_to_move != 0.0f) {
      // undo our original translation, because it collides
      player().translate(-translation);

      switch (location) {
      case CollisionLocation::bottom:
        player().set_color(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
        break;
      case CollisionLocation::top:
        player().set_color(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
        break;
      case CollisionLocation::left:
        player().set_color(glm::vec4{0.0f, 0.0f, 1.0f, 1.0f});
        break;
      case CollisionLocation::right:
        player().set_color(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
        break;
      }

      // the collision detection tells us how much less we need to move, so we move by
      // exactly that much. since moving_by will always be 1.0 or -1.0, this effectively
      // replaces the single non-zero component with the distance with however far we have
      translation = translation * (1.0f - amount_less_to_move);
      player().translate(translation);
    }
  }

  view_matrix_ = glm::translate(view_matrix_, translation);
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
    // ground
    std::pair{glm::vec3{0.0f, -10.0f, 0.0f}, glm::vec2{50.0f, 10.0f}}
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
