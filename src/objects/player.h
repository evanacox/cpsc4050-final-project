//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H

#include "../game_object.h"
#include <array>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/// Represents a single animation in the sprite sheet
struct Animation {
  int frame_count = 0;              // Total number of frames in the animation
  int current_frame = 0;            // Index of the current frame (starts at 0)
  int elapsed_frames = 0;           // Time elapsed since the last frame change
  glm::vec2 frame_size{};           // Size of each frame in UV coordinates (normalized)
  std::vector<glm::vec2> uv_coords; // UV coordinates for all frames
  std::string texture_name;         // Name of the associated texture

  Animation() = default;

  /// Constructor to initialize the animation
  Animation(int frames, glm::vec2 size, std::string texture) noexcept;
};

/// Models the player character
class Player final : public UniqueGameObject {
public:
  /// Initializes the object
  ///
  /// \param position The starting position of the player
  explicit Player(glm::vec3 position) noexcept
      : UniqueGameObject(position, "player", "") {}

  void setup(GLContext& gl) noexcept override;

  GLuint load_uniforms(GLContext& gl, const glm::mat4& proj,
                       const glm::mat4& view) noexcept override;

  [[nodiscard]] glm::vec2 dimensions() const noexcept { return glm::vec2{5.0f, 5.0f}; }

  /// Updates the current animation frame based on elapsed time
  void update_animation() noexcept;

  /// Sets the current animation by name
  void set_animation(const std::string& anim_name);

  /// When the player changes direction, the texture may need to be mirrored. This updates
  /// the player's facing direction (either left or right)
  ///
  /// \param is_facing_left Whether the player is facing left
  void set_facing_left(bool is_facing_left) noexcept { should_mirror_ = !is_facing_left; }

private:
  std::unordered_map<std::string, Animation> animations_; // Map of animations by name
  Animation* current_animation_ = nullptr; // Pointer to the current animation
  GLuint uv_vbo_{};                        // OpenGL buffer for UV coordinates
  int current_frame_ = 0;                  // the current animation frame
  bool update_uv_on_next_draw_ = false;
  bool should_mirror_ = false;

  /// Loads animations into the player object
  void load_animations(GLContext& gl);

  /// Updates the UV buffer with the current animation frame
  void update_uv_buffer();
};

#endif // PROJECT_PLAYER_H
