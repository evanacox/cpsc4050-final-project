//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./gl_context.h"
#include "./window.h"
#include <glm/glm.hpp>

constexpr auto CAMERA_STARTING_POS = glm::vec3{0.0f, 0.0f, 5.0f};
constexpr auto CAMERA_UP = glm::vec3{0.0f, 1.0f, 0.0f};

int main() {
  auto gl = GLContext{};
  auto scene = Scene{CAMERA_STARTING_POS, CAMERA_UP};
  auto window = Window{"CPSC4050 Final Project", scene};

  scene.setup(gl);
  window.setup();

  // once both are configured, we have a correct width/height and can set our
  // viewport and whatnot via. OpenGL
  gl.setup(window.width(), window.height());

  return window.loop_until_done(gl);
}