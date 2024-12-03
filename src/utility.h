//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_UTILITY_H
#define PROJECT_UTILITY_H

#include <array>
#include <glm/glm.hpp>
#include <vector>

struct RectangleBuffers {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uv_coords;
};

/// Generates the triangular vertices to render a rectangle
///
/// \param width The full width of the rectangle
/// \param height The full height of the rectangle
/// \param z The z-coordinate to give to all vertices
/// \param center The center point of the rectangle
/// \return The corners and vertices
RectangleBuffers rectangle_vertices(float width, float height, glm::vec3 center);

#endif // PROJECT_UTILITY_H
