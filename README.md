# Final Project

## How to build

```bash
$ cd project
$ make
$ ./platformer
```

## Code Structure

The `src/` and `shaders/` folders are our code. The `lib` directory contains our non-system dependencies.

- `gl_context.h/cpp`: Class that manages keeps all the OpenGL stuff nice and tidy
- `scene.h/cpp`: Class that manages a scene graph, and handles drawing objects in a scene (along with the camera for
  that scene)
- `window.h/cpp`: Class that manages a GLFW window and the associated OpenGL canvas

## Dependencies

- `glm` (OpenGL Mathematics): Math library we used for vector/matrix types
- `stb_image`: Image library we used for texture loading
