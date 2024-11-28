//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_RECTANGLE_H
#define PROJECT_RECTANGLE_H

#include "../scene.h"
#include "./player.h"

/// A rectangular obstacle that supports collision detection in 2d.
class Rectangle final : public NonUniqueGameObject {
public:
  explicit Rectangle(glm::vec3 center, glm::vec2 dimension) noexcept
      : NonUniqueGameObject(center, "rectangle"), dimension_{dimension} {}

  void setup(GLContext& gl) noexcept override;

  [[nodiscard]] bool collides_with(const Player& player) const noexcept;

private:
  glm::vec2 dimension_;
};

#endif // PROJECT_RECTANGLE_H
