//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_GL_CONTEXT_H
#define PROJECT_GL_CONTEXT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <limits>
#include <span>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

/// A type that contains and manages all the OpenGL state.
///
/// It provides a consistent abstraction for messing with OpenGL
class GLContext {
public:
  /// Initializes OpenGL
  GLContext() noexcept;

  /// Enables all the rest of the OpenGL state once the window has been created.
  ///
  /// \param framebuffer_width The true width of the framebuffer
  /// \param framebuffer_height The true height of the framebuffer
  void setup(int framebuffer_width, int framebuffer_height) noexcept;

  /// Clears the buffers that were enabled in `setup`
  void clear_enabled_buffers() noexcept;

  /// Creates a VAO with a given name
  ///
  /// \param name The name used to refer to this VAO
  void create_vao(const std::string& name) noexcept;

  /// Enables the VAO associated with a given name
  ///
  /// \param name The name previously associated with the VAO
  void bind_vao(const std::string& name) noexcept;

  /// Combines the create/bind operations into one call
  ///
  /// \param name The name for the VAO
  void create_and_bind_vao(const std::string& name) noexcept {
    create_vao(name);
    bind_vao(name);
  }

  /// Creates a VBO and returns it
  ///
  /// \param count The number of VBOs to create
  /// \return The list of VBOs
  [[nodiscard]] std::vector<GLuint> create_vbos(int count) noexcept;

  /// Fills a VBO with a given array of vector data.
  ///
  /// \tparam Vec The GLM vector type for the data
  /// \param vbo The VBO to fill
  /// \param index The index to enable the VBO at
  /// \param data The data to be loaded
  template <typename Vec>
  void fill_enable_vbo(GLuint vbo, int index, const std::vector<Vec>& data) noexcept {
    static_assert(std::is_same_v<typename Vec::value_type, float>,
                  "this code needs to be adapted for different vector types");

    auto elements_per_vec = data[0].length();
    auto type = static_cast<GLenum>(GL_FLOAT);

    fill_enable_vbo_raw(vbo, index, type, elements_per_vec, data.data(),
                        data.size() * sizeof(Vec));
  }

  /// Compiles two shaders, combines them into a program, and associates that
  /// program with a name that can be used to enable/disable it.
  ///
  /// \param name The name to give the shader program
  /// \param vert_file The name of the file containing the vertex shader GLSL code
  /// \param frag_file The name of the file containing the fragment shader GLSL code
  void create_shader_program(std::string name, std::string vert_file,
                             std::string frag_file) noexcept;

  /// Enables a shader program that was previously created
  ///
  /// \param name The name of the shader
  GLuint use_program(const std::string& name) noexcept;

private:
  /// Fills an existing VBO with data, and enables it at a given index.
  ///
  /// \param vbo The VBO to fill
  /// \param index The index to enable it at for consumption in GLSL
  /// \param type The type of data contained
  /// \param elements_per_vec The number of components per vector (e.g. if
  /// `data` contains `vec3`, this will be `3`)
  /// \param data Pointer to the raw data
  /// \param size_bytes The size, in bytes, of the data array
  void fill_enable_vbo_raw(GLuint vbo, int index, GLenum type, int elements_per_vec,
                           const void* data, std::ptrdiff_t size_bytes) noexcept;

  GLuint current_vao_ = std::numeric_limits<GLuint>::max();
  std::unordered_map<std::string, GLuint> vaos_;
  std::unordered_map<GLuint, std::vector<GLuint>> vbos_;
  std::unordered_map<GLuint, std::vector<std::pair<GLuint, int>>> enabled_vbos_;
  std::unordered_map<std::string, GLuint> shaders_;
  std::unordered_map<std::string, std::pair<std::string, std::string>> shader_files_;
};

#endif // PROJECT_GL_CONTEXT_H
