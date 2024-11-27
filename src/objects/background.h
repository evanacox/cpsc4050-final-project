//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_BACKGROUND_H
#define PROJECT_BACKGROUND_H

#include "../scene.h"

class Background final : public GameObject {
public:
  explicit Background(glm::vec3 center) noexcept : GameObject(center, "background") {}

  void setup(GLContext& gl) noexcept override;

  GLuint load_uniforms(GLContext& gl, const glm::mat4& proj,
                       const glm::mat4& view) noexcept override;

  void draw(GLContext& gl) noexcept override;
};

#endif // PROJECT_BACKGROUND_H
