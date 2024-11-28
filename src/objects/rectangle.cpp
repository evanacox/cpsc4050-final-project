//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./rectangle.h"
#include "../utility.h"

void Rectangle::setup(GLContext& gl) noexcept {
  gl.create_shader_program("rectangle", "shaders/rectangle.vert",
                           "shaders/rectangle.frag");
  gl.create_and_bind_vao(vao_name());

  auto props = rectangle_vertices(dimension_.x, dimension_.y, glm::vec3{0.0f});
  auto buf = gl.create_vbos(2);
  auto vertex = buf[0];
  auto uv = buf[1];

  set_vertex_count(static_cast<int>(props.triangle_vertices.size()));

  gl.fill_enable_vbo(vertex, 0, props.triangle_vertices);
}

bool Rectangle::collides_with(const Player& player) const noexcept {
  //  const dx = (rect1.x + rect1.w / 2) - (rect2.x + rect2.w / 2);
  //  const dy = (rect1.y +rect1.h / 2) - (rect2.y + rect2.h / 2);
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
  return false;
}
