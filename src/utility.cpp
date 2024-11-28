//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./utility.h"

std::vector<glm::vec3> rectangle_vertices(float full_width, float full_height,
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
  return {p1, p2, p3, p1, p3, p4};
}
