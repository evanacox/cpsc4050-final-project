//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_WINDOW_H
#define PROJECT_WINDOW_H

#include "./gl_context.h"
#include "./scene.h"

/// Wraps up the logic for managing a GLFW window together into one class that
/// can be called from `main`.
class Window {
public:
  /// Initializes and creates the GLFW window
  explicit Window(std::string window_title, Scene& scene) noexcept;

  /// Callback used by GLFW3
  void glfw_key_callback(int key, int scancode, int action, int mods) noexcept;

  /// This shouldn't exist, but it does because GLFW is stupid and doesn't call the
  /// callbacks it promises to.
  void setup() noexcept;

  /// Returns the window width that should be used in OpenGL stuff
  [[nodiscard]] int width() const noexcept { return framebuffer_width_; }

  /// Returns the window height that should be used in OpenGL stuff
  [[nodiscard]] int height() const noexcept { return framebuffer_height_; }

  /// Enters a draw loop until the window is closed. This is called from `main`
  ///
  /// \return A return value for the program
  [[nodiscard]] int loop_until_done(GLContext& context) noexcept;

private:
  void update_fps_counter() noexcept;

  GLFWwindow* window_;
  Scene& scene_;
  std::string window_title_;
  int framebuffer_width_ = 0;
  int framebuffer_height_ = 0;
};

#endif // PROJECT_WINDOW_H
