//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include "./gl_context.h"
#include <glm/glm.hpp>
#include <memory>

/// A single object in the game world.
///
/// This is an abstraction allowing the render loop to draw all objects
/// in the scene the same way, and delegate to the object itself on
/// how to draw it.
class GameObject {
public:
  /// Virtual destructor, required for base classes
  virtual ~GameObject() = default;

  /// Sets up the GameObject's OpenGL state. This is the place
  /// to compile an object's shaders, create VAOs and VBOs, etc.
  ///
  /// \param gl The OpenGL state
  virtual void setup(GLContext& gl) noexcept = 0;

  /// Loads uniforms for the object. By default this doesn't need to
  /// be overridden, the model matrix will be loaded as a translation
  /// from `position_` to the world origin and the projection/view matrices
  /// will simply be copied in.
  ///
  /// \param gl The OpenGL state
  /// \return The shader being used by the object, can be used by derived instances
  virtual GLuint load_uniforms(GLContext& gl, const glm::mat4& proj,
                               const glm::mat4& view) noexcept;

  /// Tells the GameObject to draw itself as if it was inside the render loop.
  ///
  /// \param gl The OpenGL state
  virtual void draw(GLContext& gl) noexcept;

  /// Translates the object by a given vector.
  ///
  /// \param by The amount to translate by
  void translate(glm::vec3 by) noexcept { position_ += by; }

  /// Gets the position of the GameObject.
  ///
  /// \return The object's position
  [[nodiscard]] glm::vec3 position() noexcept { return position_; }

  /// Gets the number of vertices in the object's vertex buffer.
  ///
  /// \return The number of vertices
  [[nodiscard]] int vertex_count() const noexcept { return vertex_len_; }

  [[nodiscard]] const std::string& shader_name() const noexcept { return shader_name_; }

  [[nodiscard]] const std::string& vao_name() const noexcept { return vao_name_; }

protected:
  /// Initializes the object's position. This is used to compute the model
  /// matrix.
  ///
  /// \param position The position of the object
  /// \param shader_name The name of the shader program used by this object
  /// \param vao_name The name of the VAO created by this object
  explicit GameObject(glm::vec3 position, std::string shader_name,
                      std::string vao_name) noexcept
      : shader_name_{std::move(shader_name)}, vao_name_{std::move(vao_name)},
        position_{position} {}

  /// Sets the number of vertices the vertex array has. This is
  /// not the number of individual float values, rather the number
  /// of vertices (i.e. 3-float triplets).
  ///
  /// \param count The number of vertices
  void set_vertex_count(int count) noexcept { vertex_len_ = count; }

private:
  int vertex_len_ = 0;
  std::string shader_name_;
  std::string vao_name_;
  glm::vec3 position_;
};

/// A game object where there can only be exactly one of them
/// at any given time.
class UniqueGameObject : public GameObject {
protected:
  /// Creates a unique object, i.e. it's VAO name is the same as the shader
  /// because there's only ever one of these.
  ///
  /// \param position
  /// \param object_name
  UniqueGameObject(glm::vec3 position, const char* object_name) noexcept
      : GameObject{position, object_name, object_name} {}
};

/// A game object where there can only be exactly one of them
/// at any given time.
class NonUniqueGameObject : public GameObject {
protected:
  /// Creates a unique object, i.e. it's VAO name is the same as the shader
  /// because there's only ever one of these.
  ///
  /// \param position
  /// \param object_name
  NonUniqueGameObject(glm::vec3 position, const char* object_name) noexcept;

private:
  static int counter;
};

/// Models a very trivial scene graph that's simply a list of objects
///
/// This contains a camera looking at the scene, and the scene itself.
class Scene {
public:
  Scene(glm::vec3 camera_pos, glm::vec3 camera_up) noexcept
      : camera_position_{camera_pos}, camera_up_{camera_up} {}

  /// Adds an object to the scene
  ///
  /// \param object The object being added
  GameObject* add_object(std::unique_ptr<GameObject> object) noexcept;

  /// Sets the GameObject that's the "player", i.e. the GameObject that
  /// the camera is looking at.
  ///
  /// \param player
  void set_player(GameObject* player) noexcept { player_ = player; }

  /// Sets up the OpenGL state for all the objects
  ///
  /// \param gl The OpenGL state
  void setup(GLContext& gl) noexcept;

  /// Draws all the objects
  ///
  /// \param gl The OpenGL state
  /// \param proj The projection matrix to use, given by the window
  void draw_everything(GLContext& gl, const glm::mat4& proj) noexcept;

  /// Translates the camera's position
  ///
  /// \param by The amount to translate by
  void translate_camera(glm::vec3 by) noexcept { camera_position_ += by; }

  /// Handles a keypress from the window
  ///
  /// \param key The key to handle
  void handle_keypress(int key) noexcept;

private:
  std::vector<std::unique_ptr<GameObject>> objects_;
  GameObject* player_ = nullptr;
  glm::vec3 camera_position_;
  glm::vec3 camera_up_;
};

#endif // PROJECT_SCENE_H
