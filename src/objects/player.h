//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

#include "../scene.h"
#include <array>

/// Models the player character
class Player final : public UniqueGameObject {
public:
  /// Initializes the object
  ///
  /// \param position The starting position of the player
  explicit Player(glm::vec3 position) noexcept : UniqueGameObject(position, "player") {}

  void setup(GLContext& gl) noexcept override;

  /// Returns the (2d) corners of the player's hitbox. This allows for collision
  /// detection by obstacles.
  ///
  /// \return
  [[nodiscard]] std::array<glm::vec2, 4> hitbox_corners() const noexcept {
    return corners_;
  }

private:
  std::array<glm::vec2, 4> corners_;
};

#endif // PROJECT_PLAYER_H
