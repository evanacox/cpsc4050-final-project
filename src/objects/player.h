//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

#include "../game_object.h"
#include <array>

/// Models the player character
class Player final : public UniqueGameObject {
public:
  /// Initializes the object
  ///
  /// \param position The starting position of the player
  explicit Player(glm::vec3 position) noexcept : UniqueGameObject(position, "player") {}

  void setup(GLContext& gl) noexcept override;

  [[nodiscard]] glm::vec2 dimensions() const noexcept { return glm::vec2{5.0f, 5.0f}; }
};

#endif // PROJECT_PLAYER_H
