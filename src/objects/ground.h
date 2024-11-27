//======---------------------------------------------------------------======//
//                                                                           //
// CPSC4050 Final Project                                                    //
// Alaina Carson, Evan Cox                                                   //
//                                                                           //
//======---------------------------------------------------------------======//

#ifndef PROJECT_GROUND_H
#define PROJECT_GROUND_H

#include "../scene.h"

class Ground final : public GameObject {
public:
  void setup(GLContext& gl) noexcept override;

  void draw(GLContext& gl) noexcept override;
};

#endif // PROJECT_GROUND_H
