//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#include "./gl_context.h"
#include <array>
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace {

void glfw_error_callback(int error, const char* description) {
  std::cerr << "glfw3 error (code = " << error << "): " << description << '\n';
}

GLuint create_and_compile_shader(const std::string& name, const std::string& content,
                                 char* info_log, GLenum type) noexcept {
  auto shader = glCreateShader(type);
  const auto* p = content.c_str();

  glShaderSource(shader, 1, &p, nullptr);
  glCompileShader(shader);

  auto params = -1;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &params);

  if (params != GL_TRUE) {
    auto suffix = (type == GL_VERTEX_SHADER) ? "vert" : "frag";

    std::cerr << "error: shader '" << name << '.' << suffix << "' did not compile\n";

    glGetShaderInfoLog(shader, 1024, nullptr, info_log);
    std::cerr << "  info log: " << info_log << "\n";

    std::abort();
  }

  return shader;
}

std::string read_entire_file(std::string name) noexcept {
  auto path = fs::relative(std::move(name));

  if (!fs::exists(path)) {
    std::cerr << "error: shader '" << path << "' was not found";
    glfwTerminate();
  }

  // open the file, and find the size in chars
  auto in = std::ifstream{path};
  in.seekg(0, std::ios::end);

  // make a string of that size
  auto size = in.tellg();
  auto buffer = std::string(size, ' ');

  // read the entire file into that string
  in.seekg(0);
  in.read(&buffer[0], size);

  return buffer;
}

} // namespace

GLContext::GLContext() noexcept {
  glfwSetErrorCallback(glfw_error_callback);

  if (glfwInit() != GL_TRUE) {
    std::cerr << "fatal error! unable to start GLFW3\n";
    std::abort();
  }
}

void GLContext::setup(int framebuffer_width, int framebuffer_height) noexcept {
  // enable depth testing, smaller Z-distance => "closer"
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // enable culling the back of objects, we should only be looking at the front
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // set counter-clock-wise vertex order to mean the front
  glFrontFace(GL_CCW);

  // set black clear color
  glClearColor(0.1f, 0.1f, 0.1f, 1.0);

  // set our viewport to be the framebuffer size we got
  //  glViewport(0, 0, framebuffer_width, framebuffer_height);

  std::clog << "OpenGL version supported: " << glGetString(GL_VERSION) << "\n";
  std::clog << "Renderer: " << glGetString(GL_RENDERER) << "\n";
}

void GLContext::clear_enabled_buffers() noexcept {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLContext::create_vao(const std::string& name) noexcept {
  auto vao = GLuint{};

  glGenVertexArrays(1, &vao);

  vaos_[name] = vao;
}

void GLContext::bind_vao(const std::string& name) noexcept {
  auto vao = vaos_[name];

  current_vao_ = vao;
  glBindVertexArray(vao);
}

std::vector<GLuint> GLContext::create_vbos(int count) noexcept {
  auto buf = std::vector<GLuint>(static_cast<std::size_t>(count), GLuint{});

  glGenBuffers(count, buf.data());

  for (auto vbo : buf) {
    vbos_[current_vao_].push_back(vbo);
  }

  return buf;
}

void GLContext::fill_enable_vbo_raw(GLuint vbo, int index, GLenum type,
                                    int elements_per_vec, const void* data,
                                    std::ptrdiff_t size_bytes) noexcept {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_STATIC_DRAW);
  glVertexAttribPointer(index, elements_per_vec, type, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(index);

  enabled_vbos_[current_vao_].emplace_back(vbo, index);
}

void GLContext::create_shader_program(std::string name, std::string vert_file,
                                      std::string frag_file) noexcept {
  auto vert = read_entire_file(std::move(vert_file));
  auto frag = read_entire_file(std::move(frag_file));
  auto info_log = std::array<char, 1024>{};
  auto vertex_shader =
      create_and_compile_shader(name, vert, info_log.data(), GL_VERTEX_SHADER);
  auto fragment_shader =
      create_and_compile_shader(name, frag, info_log.data(), GL_FRAGMENT_SHADER);

  auto program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  auto params = -1;
  glGetProgramiv(program, GL_LINK_STATUS, &params);

  if (params != GL_TRUE) {
    std::cerr << "error: program '" << name << "' did not link\n";

    glGetProgramInfoLog(program, 1024, nullptr, info_log.data());
    std::cerr << "  info log: " << info_log.data() << "\n";

    std::abort();
  }

  shaders_.emplace(std::move(name), program);
}

GLuint GLContext::use_program(const std::string& name) noexcept {
  auto program = shaders_[name];

  glUseProgram(program);

  return program;
}
