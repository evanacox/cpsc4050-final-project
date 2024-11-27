CC_FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g
LIBS := -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi -lglfw -lGLEW

SRCS_WITH_HEADERS := src/gl_context.cpp src/window.cpp src/scene.cpp src/objects/background.cpp
SRCS_WITH_HEADERS += src/objects/player.cpp src/utility.cpp
SRCS := $(SRCS_WITH_HEADERS) src/main.cpp src/stb_image_impl.cpp
INCLUDES := -isystem lib/
OBJS := $(subst .cpp,.o,$(subst src/,build/,$(SRCS))) # converts `src/something.cpp` -> `build/something.o`
HEADERS := $(subst .cpp,.h,$(SRCS_WITH_HEADERS))

all: platformer

platformer: $(OBJS)
	$(CXX) $(CC_FLAGS) $^ $(LIBS) -o $@

build/%.o: src/%.cpp $(HEADERS)
	mkdir -p $(subst .cpp,,$(subst src/,build/,$<))
	$(CXX) $(CC_FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf ./build
	rm -f ./platformer
