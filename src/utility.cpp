//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./utility.h"

RectangleBuffers rectangle_vertices(float full_width, float full_height,
                                    glm::vec3 center) {
  auto width = full_width / 2.0f;
  auto height = full_height / 2.0f;
  auto p1 = center + glm::vec3{-width, height, 0.0f};
  auto p2 = center + glm::vec3{-width, -height, 0.0f};
  auto p3 = center + glm::vec3{width, -height, 0.0f};
  auto p4 = center + glm::vec3{width, height, 0.0f};

  //
  //  p1---p4
  //  | \  |
  //  |  \ |
  //  |   \|
  //  p2--p3
  //
  auto vertices = std::vector{p1, p2, p3, p1, p3, p4};

  auto uv1 = glm::vec2{0.0f, 1.0f};
  auto uv2 = glm::vec2{0.0f, 0.0f};
  auto uv3 = glm::vec2{1.0f, 0.0f};
  auto uv4 = glm::vec2{1.0f, 1.0f};
  auto uv_coords = std::vector{uv1, uv2, uv3, uv1, uv3, uv4};

  return {std::move(vertices), std::move(uv_coords)};
}
