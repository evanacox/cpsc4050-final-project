//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./gl_context.h"
#include "./objects/player.h"
#include "./scene.h"
#include "./window.h"
#include <glm/glm.hpp>
#include <memory>

constexpr auto CAMERA_STARTING_POS = glm::vec3{0.0f, 0.0f, 5.0f};
constexpr auto CAMERA_UP = glm::vec3{0.0f, 1.0f, 0.0f};

namespace {

void setup_game_world(Scene& scene) {
  auto object = std::make_unique<Player>(glm::vec3{0.0f, 0.0f, 0.0f});
  auto* player = scene.add_object(std::move(object));

  scene.set_player(player);
}

} // namespace

int main() {
  auto gl = GLContext{};
  auto window = Window{"CPSC4050 Final Project"};
  auto scene = Scene{CAMERA_STARTING_POS, CAMERA_UP};

  setup_game_world(scene);

  scene.setup(gl);
  window.setup();
  gl.setup(window.width(), window.height());

  return window.loop_until_done(gl, scene);
}