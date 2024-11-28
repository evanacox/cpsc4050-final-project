//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include "./game_object.h"
#include "./gl_context.h"
#include "./objects/player.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

/// Models a very trivial scene graph that's simply a list of objects
///
/// This contains a camera looking at the scene, and the scene itself.
class Scene {
public:
  Scene(glm::vec3 camera_pos, glm::vec3 camera_up) noexcept
      : view_matrix_{1.0f}, camera_position_{camera_pos}, camera_up_{camera_up} {}

  /// Initializes the scene, and sets up the OpenGL state for all the objects
  ///
  /// \param gl The OpenGL state
  void setup(GLContext& gl) noexcept;

  /// Called once per second
  ///
  void update_scene(const std::vector<int>& keys_pressed) noexcept;

  /// Draws all the objects
  ///
  /// \param gl The OpenGL state
  /// \param proj The projection matrix to use, given by the window
  void draw_everything(GLContext& gl, const glm::mat4& proj) noexcept;

  /// Translates the camera's position
  ///
  /// \param by The amount to translate by
  void translate_camera(glm::vec3 by) noexcept { camera_position_ += by; }

  /// Returns a reference to the player object
  ///
  /// \return The player object
  [[nodiscard]] Player& player() noexcept {
    // first object is always guaranteed to be a player object
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    return static_cast<Player&>(*objects_[0]);
  }

private:
  void create_scene() noexcept;

  void handle_keypress(int key) noexcept;

  // we keep all the objects in one array in order that we can still iterate over them
  // for drawing/setup/uniforms/whatever, but we have different "types" of objects at
  // different places
  //
  // [ player, background1, background2, ..., obstacle1, obstacle2, ... ]
  //   ^       ^                              ^
  //   |       |                              always index obstacles_begin_ to end
  //   |       always index 2 to obstacles_begin_
  //   always index 0
  std::vector<std::unique_ptr<GameObject>> objects_;
  std::size_t obstacles_begin_ = 0;
  glm::mat4 view_matrix_;
  glm::vec3 camera_position_;
  glm::vec3 camera_up_;
};

#endif // PROJECT_SCENE_H
