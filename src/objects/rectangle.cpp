//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./rectangle.h"
#include "../utility.h"
#include <cmath>
#include <iostream>

void Rectangle::setup(GLContext& gl) noexcept {
  gl.create_shader_program("rectangle", "shaders/rectangle.vert",
                           "shaders/rectangle.frag");
  gl.create_and_bind_vao(vao_name());

  auto vertices = rectangle_vertices(dimension_.x, dimension_.y, glm::vec3{0.0f});
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  set_vertex_count(static_cast<int>(vertices.size()));

  gl.fill_enable_vbo(vertex, 0, vertices);
}

namespace {

struct RectangleProps {
  float x, y, w, h;
};

} // namespace

namespace {

RectangleProps rect_from_pos_dimensions(glm::vec3 position,
                                        glm::vec2 dimensions) noexcept {
  return RectangleProps{position.x - dimensions.x / 2.0f,
                        position.y - dimensions.y / 2.0f, dimensions.x, dimensions.y};
}

} // namespace

Collision Rectangle::collides_with(const Player& player,
                                   glm::vec3 translation) const noexcept {
  auto rect1 = rect_from_pos_dimensions(position(), dimension_);
  auto rect2 = rect_from_pos_dimensions(player.position(), player.dimensions());
  auto dx = (rect1.x + rect1.w / 2.0f) - (rect2.x + rect2.w / 2.0f);
  auto dy = (rect1.y + rect1.h / 2.0f) - (rect2.y + rect2.h / 2.0f);
  auto width = (rect1.w + rect2.w) / 2.0f;
  auto height = (rect1.h + rect2.h) / 2.0f;
  auto cross_width = width * dy;
  auto cross_height = height * dx;
  auto x_difference = width - std::fabs(dx);
  auto y_difference = height - std::fabs(dy);

  if (x_difference >= 0 && y_difference >= 0) {
    auto location = CollisionLocation{};

    // hack: check if the bottom of the player is at/above the rectangle.
    // if it is, and we're already colliding, we immediately go to "top"
    // for the purpose of our gravity simulation
    if (std::fabs(rect2.y) <= std::fabs(rect1.y + rect1.h + translation.y)) {
      location = CollisionLocation::top;
    } else if (cross_width > cross_height) {
      location = (cross_width > -cross_height) ? CollisionLocation::bottom
                                               : CollisionLocation::right;
    } else {
      location = (cross_width > -cross_height) ? CollisionLocation::left
                                               : CollisionLocation::top;
    }

    return {location, std::min(x_difference, y_difference)};
  }

  return {CollisionLocation::none, 0.0f};
}
