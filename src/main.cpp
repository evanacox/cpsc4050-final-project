//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./gl_context.h"
#include "./scene.h"
#include "./window.h"
#include <glm/glm.hpp>

constexpr auto CAMERA_STARTING_POS = glm::vec3{0.0f, 0.0f, 5.0f};
constexpr auto CAMERA_UP = glm::vec3{0.0f, 1.0f, 0.0f};

int main() {
  auto gl = GLContext{};
  auto window = Window{"CPSC4050 Final Project"};
  auto scene = Scene{CAMERA_STARTING_POS, CAMERA_UP};

  gl.setup(window.width(), window.height());
  scene.setup(gl);

  return window.loop_until_done(gl, scene);
}