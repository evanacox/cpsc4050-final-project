//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_UTILITY_H
#define PROJECT_UTILITY_H

#include <glm/glm.hpp>
#include <vector>

std::vector<glm::vec3> rectangle_vertices(float width, float height, float z,
                                          glm::vec3 center);

#endif // PROJECT_UTILITY_H
