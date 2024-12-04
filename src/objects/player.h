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
#include <unordered_map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

/// Represents a single animation in the sprite sheet
  struct Animation {
      int frame_count;               // Total number of frames in the animation
      int current_frame = 0;         // Index of the current frame (starts at 0)
      float frame_time;              // Duration of each frame in seconds
      float elapsed_time = 0.0f;     // Time elapsed since the last frame change
      glm::vec2 frame_size;          // Size of each frame in UV coordinates (normalized)
      std::vector<glm::vec2> uv_coords; // UV coordinates for all frames
      std::string texture_name;      // Name of the associated texture

      /// Constructor to initialize the animation
      Animation(int frames, float time_per_frame, glm::vec2 size, const std::string& texture)
        : frame_count(frames), frame_time(time_per_frame), frame_size(size), texture_name(texture) {}
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

  void set_color(glm::vec4 color) noexcept { color_ = color; }


  /// Updates the current animation frame based on elapsed time
  void update_animation(float delta_time);
  /// Sets the current animation by name
  void set_animation(const std::string& anim_name);

private:
  
  glm::vec4 color_ = glm::vec4{0.0f};
  GLuint uv_vbo;                                         // OpenGL buffer for UV coordinates
  std::unordered_map<std::string, Animation> animations; // Map of animations by name
  Animation* current_animation = nullptr;               // Pointer to the current animation

  /// Loads animations into the player object
  void load_animations();

  /// Updates the UV buffer with the current animation frame
  void update_uv_buffer();
};

#endif // PROJECT_PLAYER_H
