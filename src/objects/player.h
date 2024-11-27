//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

#include "../scene.h"

/// Models the player character
class Player final : public GameObject {
public:
  /// Initializes the object
  ///
  /// \param position The starting position of the player
  explicit Player(glm::vec3 position) noexcept : GameObject(position, "player") {
    (void)0;
  }

  void setup(GLContext& gl) noexcept override;

  void draw(GLContext& gl) noexcept override;
};

#endif // PROJECT_PLAYER_H
