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
#include <algorithm>
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <tuple>

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

namespace {

// this array has pairs of (center point, dimension). dimension = (width, height)
auto backgrounds = std::array{
    // clang-format off
  // NOTE THAT THESE ARE IN BACK TO FRONT ORDER!!!!
  std::tuple{glm::vec3{0.0f, 10.0f, 5.0f}, glm::vec2{500.0f, 500.0f}, "assets/backgrounds/1.png"},
  std::tuple{glm::vec3{0.0f, 10.0f, 3.0f}, glm::vec2{500.0f, 500.0f}, "assets/backgrounds/2.png"},
  std::tuple{glm::vec3{0.0f, 10.0f, 1.5f}, glm::vec2{500.0f, 500.0f}, "assets/backgrounds/3.png"},
  std::tuple{glm::vec3{0.0f, 0.0f, 0.5f}, glm::vec2{500.0f, 500.0f}, "assets/backgrounds/4.png"}
    // clang-format on
};

// same structure here for the rectangular obstacles
auto obstacles = std::array{
    // ground
    std::pair{glm::vec3{0.0f, -10.0f, 0.0f}, glm::vec2{50.0f, 10.0f}}
    // clang-format on
};

constexpr auto LEFT = glm::vec3{-1.0f, 0.0f, 0.0f};
constexpr auto RIGHT = glm::vec3{1.0f, 0.0f, 0.0f};
constexpr auto DOWN = glm::vec3{0.0f, -1.0f, 0.0f};
constexpr auto UP = glm::vec3{0.0f, 1.0f, 0.0f};

} // namespace

void Scene::create_scene() noexcept {
  // create the player
  for (const auto& [center, dimension, texture] : backgrounds) {
    auto background = std::make_unique<Background>(center, dimension, texture);

    objects_.push_back(std::move(background));
  }

  obstacles_begin_ = objects_.size();

  for (const auto& [center, dimension] : obstacles) {
    auto obstacle = std::make_unique<Rectangle>(center, dimension);

    objects_.push_back(std::move(obstacle));
  }

  objects_.push_back(std::make_unique<Player>(glm::vec3{0.0f, 0.0f, 0.0f}));
}

void Scene::update_scene(const std::vector<int>& keys_pressed) noexcept {
  on_ground_ = false;
  jumping_for_n_frames_ = 0;

  player().set_color(glm::vec4{0.7f, 0.3f, 0.6f, 1.0f});

  // handle all our movement key-presses. if we happen to touch the ground here, it will
  // figure it out and update `on_ground_`
  for (auto key : keys_pressed) {
    handle_keypress(key);
  }

  // if we didn't hit ground due to movement, check if we're already on it
  if (!on_ground_) {
    update_on_ground();
  }

  // if we still aren't on it, we apply gravity. otherwise, we stop the player
  if (!on_ground_ && jumping_for_n_frames_ == 0) {
    player().set_color(glm::vec4{0.3f, 0.6f, 0.9f, 1.0f});
    translate_player_by(DOWN);
  }
}

void Scene::handle_keypress(int key) noexcept {
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
    jumping_for_n_frames_ = 1;
    break;
  default:
    return;
  }

  translate_player_by(translation);
}

void Scene::translate_player_by(glm::vec3 translation) noexcept {
  player().translate(translation);

  for (auto i = obstacles_begin_; i < objects_.size() - 1; ++i) {
    // we know this is safe, every obstacle is a rectangle
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    auto& rect = static_cast<Rectangle&>(*objects_[i]);
    auto [location, amount_less_to_move] = rect.collides_with(player(), translation);

    if (location != CollisionLocation::none && amount_less_to_move != 0.0f) {
      // undo our original translation, because it collides
      player().translate(-translation);

      if (location == CollisionLocation::top) {
        on_ground_ = true;
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

void Scene::update_on_ground() noexcept {
  for (auto i = obstacles_begin_; i < objects_.size() - 1; ++i) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    auto& rect = static_cast<Rectangle&>(*objects_[i]);
    auto [location, _] = rect.collides_with(player(), glm::vec3{0.0f});

    if (location == CollisionLocation::top) {
      on_ground_ = true;
    }
  }
}
