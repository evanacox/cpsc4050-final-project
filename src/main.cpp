//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./gl_context.h"
#include "./objects/background.h"
#include "./objects/player.h"
#include "./scene.h"
#include "./window.h"
#include <glm/glm.hpp>
#include <memory>

constexpr auto CAMERA_STARTING_POS = glm::vec3{0.0f, 0.0f, 5.0f};
constexpr auto CAMERA_UP = glm::vec3{0.0f, 1.0f, 0.0f};

namespace {

void setup_game_world(Scene& scene) {
  auto o1 = std::make_unique<Player>(glm::vec3{0.0f, 0.0f, 0.0f});
  auto* player = scene.add_object(std::move(o1));
  scene.set_player(player);

  auto o2 = std::make_unique<Background>(glm::vec3{0.0f});
  scene.add_object(std::move(o2));
}

} // namespace

int main() {
  auto gl = GLContext{};
  auto scene = Scene{CAMERA_STARTING_POS, CAMERA_UP};
  auto window = Window{"CPSC4050 Final Project", scene};

  setup_game_world(scene);

  // once we have all our objects loaded, we configure the scene and the window
  scene.setup(gl);
  window.setup();

  // once both are configured, we have a correct width/height and can set our
  // viewport and whatnot via. OpenGL
  gl.setup(window.width(), window.height());

  return window.loop_until_done(gl);
}