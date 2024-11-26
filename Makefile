CC_FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g
LIBS := -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglfw -lGLEW

SRCS := src/main.cpp src/stb_image_impl.cpp src/gl_context.cpp src/window.cpp src/scene.cpp
INCLUDES := -isystem lib/
OBJS := $(subst .cpp,.o,$(subst src/,build/,$(SRCS))) # converts `src/something.cpp` -> `build/something.o`

all: platformer

platformer: $(OBJS)
	$(CXX) $(CC_FLAGS) $^ $(LIBS) -o $@

build/%.o: src/%.cpp
	mkdir -p build/
	$(CXX) $(CC_FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf ./build
	rm -f ./platformer
