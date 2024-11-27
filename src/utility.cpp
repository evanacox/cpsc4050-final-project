//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./utility.h"

std::vector<glm::vec3> rectangle_vertices(float width, float height, float z,
                                          glm::vec3 center) {

  auto p1 = center + glm::vec3{-width, height, z};
  auto p2 = center + glm::vec3{-width, -height, z};
  auto p3 = center + glm::vec3{width, -height, z};
  auto p4 = center + glm::vec3{width, height, z};

  //
  //  p1---p4
  //  | \  |
  //  |  \ |
  //  |   \|
  //  p2--p3
  //
  return {p1, p2, p3, p1, p3, p4};
}
