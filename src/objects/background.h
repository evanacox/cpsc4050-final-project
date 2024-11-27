//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_BACKGROUND_H
#define PROJECT_BACKGROUND_H

#include "../scene.h"

class Background final : public NonUniqueGameObject {
public:
  explicit Background(glm::vec3 center, glm::vec2 dimension) noexcept
      : NonUniqueGameObject(center, "background"), dimension_{dimension} {}

  void setup(GLContext& gl) noexcept override;

  GLuint load_uniforms(GLContext& gl, const glm::mat4& proj,
                       const glm::mat4& view) noexcept override;

private:
  glm::vec2 dimension_;
};

#endif // PROJECT_BACKGROUND_H
