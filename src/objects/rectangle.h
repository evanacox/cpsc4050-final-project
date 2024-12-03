//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_RECTANGLE_H
#define PROJECT_RECTANGLE_H

#include "../game_object.h"
#include "./player.h"

enum class CollisionLocation { none = 0, bottom, top, left, right };

struct Collision {
  CollisionLocation location;
  float amount_less_to_move;
};

/// A rectangular obstacle that supports collision detection in 2d.
class Rectangle final : public NonUniqueGameObject {
public:
  explicit Rectangle(glm::vec3 center, glm::vec2 dimension) noexcept
      : NonUniqueGameObject(center, "rectangle", ""), dimension_{dimension} {}

  void setup(GLContext& gl) noexcept override;

  /// Returns which part of `this` is colliding with `player`, if any
  ///
  /// \param player The player being compared to
  /// \param translation The translation that just got applied, used for corners
  /// \return The side of `this` colliding with `player`
  [[nodiscard]] Collision collides_with(const Player& player,
                                        glm::vec3 translation) const noexcept;

private:
  glm::vec2 dimension_;
};

#endif // PROJECT_RECTANGLE_H
