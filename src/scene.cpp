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
  view_matrix_ = glm::translate(view_matrix_, glm::vec3{0.0f, 15.0f, 0.0f});

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
// clang-format off
auto backgrounds = std::array{
    // pair of ({center of the rectangle}, {x and y dimensions}, asset file to
    // load for texture)
    // NOTE THAT THESE ARE IN BACK TO FRONT ORDER!!!!
    std::tuple{glm::vec3{0.0f, -20.0f, 5.0f}, glm::vec2{300.0f, 250.0f}, "assets/backgrounds/1.png"},
    std::tuple{glm::vec3{0.0f, -10.0f, 3.0f}, glm::vec2{300.0f, 160.0f}, "assets/backgrounds/2.png"},
    std::tuple{glm::vec3{0.0f, 15.0f, 1.5f}, glm::vec2{300.0f, 120.0f}, "assets/backgrounds/3.png"},
    std::tuple{glm::vec3{-75.0f, 10.0f, 0.5f}, glm::vec2{150.0f, 80.0f}, "assets/backgrounds/4.png"},
    std::tuple{glm::vec3{75.0f, 10.0f, 0.5f}, glm::vec2{150.0f, 80.0f}, "assets/backgrounds/4.png"}
};
// clang-format on

// same structure here for the rectangular obstacles
auto invisible_walls = std::array{
    // pair of ({center of the rectangle}, {x and y dimensions})
    std::pair{glm::vec3{-90.0f, 0.0f, 0.1f}, glm::vec2{1.0f, 500.0f}},
    std::pair{glm::vec3{90.0f, 0.0f, 0.1f}, glm::vec2{1.0f, 500.0f}},
};

// same structure here for the rectangular obstacles
// clang-format off
auto textured_obstacles = std::array{
    // pair of ({center of the rectangle}, {x and y dimensions})
    std::tuple{
      glm::vec3{0.0f, -32.5f, 0.0f},
      glm::vec2{86.0f, 10.0f},
      glm::vec2{8.0f, 1.0f},
      "assets/backgrounds/dirt.png"
    },
    std::tuple{
      glm::vec3{-120.0f, -32.5f, 0.0f},
      glm::vec2{135.0f, 10.0f},
      glm::vec2{8.0f, 1.0f},
      "assets/backgrounds/dirt.png"
    },
    std::tuple{
      glm::vec3{120.0f, -32.5f, 0.0f},
      glm::vec2{135.0f, 10.0f},
      glm::vec2{8.0f, 1.0f},
      "assets/backgrounds/dirt.png"
    },
    std::tuple{
      glm::vec3{-34.0f, -25.0f, 0.0f},
      glm::vec2{6.0f, 6.0f},
      glm::vec2{1.0f, 1.0f},
      "assets/backgrounds/brick.png"
    },
    std::tuple{
      glm::vec3{-40.0f, -25.0f, 0.0f},
      glm::vec2{6.0f, 6.0f},
      glm::vec2{1.0f, 1.0f},
      "assets/backgrounds/brick.png"
    },
    std::tuple{
      glm::vec3{-40.0f, -19.0f, 0.0f},
      glm::vec2{6.0f, 6.0f},
      glm::vec2{1.0f, 1.0f},
      "assets/backgrounds/brick.png"
    },
    std::tuple{
      glm::vec3{48.0f, -25.0f, 0.0f},
      glm::vec2{24.0f, 6.0f},
      glm::vec2{4.0f, 1.0f},
      "assets/backgrounds/brick.png"
    },
};
// clang-format on

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

  for (const auto& [center, dimension] : invisible_walls) {
    auto obstacle = std::make_unique<Rectangle>(center, dimension);

    objects_.push_back(std::move(obstacle));
  }

  for (const auto& [center, dimension, tile_count, texture] : textured_obstacles) {
    auto obstacle = std::make_unique<Rectangle>(center, dimension, tile_count, texture);

    objects_.push_back(std::move(obstacle));
  }

  objects_.push_back(std::make_unique<Player>(glm::vec3{0.0f, .0f, 0.0f}));
}

void Scene::update_scene(const std::vector<int>& keys_pressed) noexcept {
  on_ground_ = false;
  jumped_this_frame_ = false;
  player_velocity_ = glm::vec3{0.0f};

  // handle all our movement key-presses. if we happen to touch the ground here, it will
  // figure it out and update `on_ground_`
  for (auto key : keys_pressed) {
    handle_keypress(key);
  }

  // if we didn't hit ground due to movement, check if we're already on it
  update_on_ground();

  // if we still aren't on it, we apply gravity. otherwise, we stop the player
  if (!on_ground_ && !jumped_this_frame_) {
    player().set_animation("fall");
    player_velocity_ += DOWN;

    // disallow jumping until we hit the ground
    jumping_for_n_frames_ = -1;
  }

  // choose our next animation based on our velocity
  if (player_velocity_.y > 0.0f) {
    player().set_animation("jump");
  } else if (player_velocity_.y < 0.0f) {
    player().set_animation("fall");
  } else if (std::fabs(player_velocity_.x) > 0.0f) {
    player().set_animation("run");
  } else {
    player().set_animation("idle");
  }

  // if we're moving left/right at all (regardless of jump/fall), we update this
  if (std::fabs(player_velocity_.x) > 0.0f) {
    player().set_facing_left(player_velocity_.x < 0.0f);
  }

  // we do two translations (one horizontal, one vertical) to allow the player
  // to collide in one direction but still move in another.
  //
  // ex: if player is holding UP and LEFT and collides on LEFT, we still want up
  translate_player_by(glm::vec3{player_velocity_.x, 0.0f, 0.0f});
  translate_player_by(glm::vec3{0.0f, player_velocity_.y, 0.0f});

  // if we hit the ground, we can jump again
  if (on_ground_) {
    jumping_for_n_frames_ = 0;
  }

  player().update_animation(); // Assuming 60 FPS
}

void Scene::handle_keypress(int key) noexcept {
  switch (key) {
  case GLFW_KEY_LEFT:
    player_velocity_ += LEFT;
    break;
  case GLFW_KEY_RIGHT:
    player_velocity_ += RIGHT;
    break;
  case GLFW_KEY_DOWN:
    player_velocity_ += DOWN;
    break;
  case GLFW_KEY_UP: {
    // max cap of 30 frames worth of jump height
    if (jumping_for_n_frames_ == 17) {
      jumping_for_n_frames_ = -1;
    } else if (jumping_for_n_frames_ != -1) {
      player_velocity_ += UP;
      jumping_for_n_frames_ += 1;
      jumped_this_frame_ = true;
    }
    break;
  }
  default:
    return;
  }
}

void Scene::translate_player_by(glm::vec3 translation) noexcept {
  player().translate(translation);

  for (auto i = obstacles_begin_; i < objects_.size() - 1; ++i) {
    // we know this is safe, every obstacle is a rectangle
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    auto& rect = static_cast<Rectangle&>(*objects_[i]);
    auto [location, amount_less_to_move] = rect.collides_with(player());

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
    auto [location, _] = rect.collides_with(player());

    if (location == CollisionLocation::top) {
      on_ground_ = true;
    }
  }
}
