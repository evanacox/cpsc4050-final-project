//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./rectangle.h"
#include "../utility.h"
#include <cmath>

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

Collision Rectangle::collides_with(const Player& player) const noexcept {
  //  const dx = (rect1.x + rect1.w / 2) - (rect2.x + rect2.w / 2);
  //  const dy = (rect1.y + rect1.h / 2) - (rect2.y + rect2.h / 2);
  //  const width = (rect1.w + rect2.w) / 2;
  //  const height = (rect1.h + rect2.h) / 2;
  //  const crossWidth = width * dy;
  //  const crossHeight = height * dx;
  //  let collision = 'none';
  //
  //  if (Math.abs(dx) <= width && Math.abs(dy) <= height) {
  //    if (crossWidth > crossHeight) {
  //      collision = (crossWidth > -crossHeight) ? 'bottom' : 'left';
  //    } else {
  //      collision = (crossWidth > -crossHeight) ? 'right' : 'top';
  //    }
  //  }
  auto player_dims = player.dimensions();
  auto player_pos = player.position();
  auto this_pos = this->position();
  auto rect1 =
      RectangleProps{player_pos.x - player_dims.x / 2.0f,
                     player_pos.y - player_dims.y / 2.0f, player_dims.x, player_dims.y};
  auto rect2 =
      RectangleProps{this_pos.x - dimension_.x / 2.0f, this_pos.y - dimension_.y / 2.0f,
                     dimension_.x, dimension_.y};

  auto dx = (rect1.x + rect1.w / 2.0f) - (rect2.x + rect2.w / 2.0f);
  auto dy = (rect1.y + rect1.h / 2.0f) - (rect2.y + rect2.h / 2.0f);
  auto width = (rect1.w + rect2.w) / 2.0f;
  auto height = (rect1.h + rect2.h) / 2.0f;
  auto cross_width = width * dy;
  auto cross_height = height * dx;

  //  auto x_difference = width - std::fabs(dx) ;
  //  auto y_difference = height - std::fabs(dy);
  //
  if (std::fabs(dx) <= width && std::fabs(dy) <= height) {
    auto width_bigger = cross_width > cross_height;
    auto width_bigger_than_neg = cross_width > -cross_height;
    auto location = CollisionLocation{};

    if (width_bigger && width_bigger_than_neg) {
      location = (cross_width > -cross_height) ? CollisionLocation::bottom
                                               : CollisionLocation::left;
    } else {
      location = (cross_width > -cross_height) ? CollisionLocation::right
                                               : CollisionLocation::top;
    }

    return {location, 0.0f};
  }

  return {CollisionLocation::none, 0.0f};
}
