//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./window.h"
#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
#include <cstdlib>
#include <glm/ext.hpp>
#include <iostream>

namespace {

Window* window = nullptr;

void keyboard_callback(GLFWwindow*, int key, int scancode, int action, int mods) {
  window->glfw_key_callback(key, scancode, action, mods);
}

std::string to_string_two_decimals(double value) {
  auto buf = std::array<char, 64>{};
  auto [ptr, _] =
      std::to_chars(buf.data(), buf.data() + 64, value, std::chars_format::general, 2);

  return {buf.data(), static_cast<std::size_t>(ptr - buf.data())};
}

} // namespace

Window::Window(std::string window_title, Scene& scene) noexcept
    : scene_{scene}, window_title_{std::move(window_title)} {
  window = this;
  window_ = glfwCreateWindow(800, 600, "CPSC4050 Final Project", //
                             nullptr, nullptr);

  if (window_ == nullptr) {
    std::cerr << "fatal error! unable to open GLFW3 window, exiting...\n";
    std::abort();
  }

  glfwSetKeyCallback(window_, keyboard_callback);
  glfwMakeContextCurrent(window_);

  // enable 4x MSAA, framebuffer will be 4x the size of our actual window
  glfwWindowHint(GLFW_SAMPLES, 4);

  // enable GLEW now that we have an OpenGL context running
  glewInit();
}

void Window::glfw_key_callback(int key, [[maybe_unused]] int scancode, int action,
                               [[maybe_unused]] int mods) noexcept {
  if (action == GLFW_PRESS) {
    keys_pressed_.push_back(key);
  }

  if (action == GLFW_RELEASE) {
    auto it = std::find(keys_pressed_.begin(), keys_pressed_.end(), key);

    // rather than remove from the middle, we swap to the end
    // and then pop from the back. this is far more efficient
    *it = keys_pressed_.back();
    keys_pressed_.pop_back();
  }
}

void Window::setup() noexcept {
  glfwGetFramebufferSize(window_, &framebuffer_width_, &framebuffer_height_);
}

int Window::loop_until_done(GLContext& gl) noexcept {
  auto aspect_ratio = static_cast<float>(width()) / static_cast<float>(height());
  auto proj = glm::perspective(35.0f, aspect_ratio, 0.1f, 100.0f);

  while (!glfwWindowShouldClose(window_)) {
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds_;
    previous_seconds_ = current_seconds;

    update_fps_counter();

    // this triggers at approx. 60hz
    if (elapsed_seconds >= 0.0165) {
      scene_.update_scene(keys_pressed_);
    }

    // clear the screen and redraw everything
    gl.clear_enabled_buffers();
    scene_.draw_everything(gl, proj);

    // make sure we caught all the events
    glfwPollEvents();

    // we're double-buffered, so swap
    glfwSwapBuffers(window_);
  }

  glfwTerminate();

  return 0;
}

void Window::update_fps_counter() noexcept {
  static auto previous_seconds = glfwGetTime();
  static auto frame_count = 0;

  auto current_seconds = glfwGetTime();
  auto elapsed_seconds = current_seconds - previous_seconds;

  if (elapsed_seconds > 0.25) {
    previous_seconds = current_seconds;
    auto fps = static_cast<double>(frame_count) / elapsed_seconds;
    auto rounded = std::ceil(fps * 100.0) / 100.0;
    auto title = window_title_ + " (fps: " + to_string_two_decimals(rounded) + ")";

    glfwSetWindowTitle(window_, title.c_str());
    frame_count = 0;
  }

  frame_count++;
}
