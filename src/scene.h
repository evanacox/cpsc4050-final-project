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
  virtual void draw(GLContext& gl) noexcept = 0;

  /// Translates the object by a given vector.
  ///
  /// \param by The amount to translate by
  void translate(glm::vec3 by) noexcept { position_ += by; }

  /// Gets the position of the GameObject.
  ///
  /// \return The object's position
  [[nodiscard]] glm::vec3 position() noexcept { return position_; }

protected:
  /// Initializes the object's position. This is used to compute the model
  /// matrix.
  ///
  /// \param position The position of the object
  explicit GameObject(glm::vec3 position, const char* shader_name) noexcept
      : shader_name_{shader_name}, position_{position} {}

  int vertex_len_ = 0;
  int uv_len_ = 0;
  const char* shader_name_;
  glm::vec3 position_;
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
