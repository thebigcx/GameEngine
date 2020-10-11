cmake_minimum_required(VERSION 3.13) # Minimum CMake version
project(Fast-Game-Library)                   # Create project '2D-Game-Library'
set(CMAKE_CXX_STANDARD 20)           # Set the C++ standard VERSION

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin) # Set the binrary output directory

# Compile all files
file(GLOB SOURCES CONFIGURE_DEPENDS
src/vendor/stb_image/stb_image.cpp

src/Window.cpp
src/Quad.cpp
src/Batch.cpp
src/Shader.cpp
src/VertexBuffer.cpp
src/IndexBuffer.cpp
src/VertexArray.cpp
src/Image.cpp

tests/Main.cpp
)

# Set the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Build the executable
add_executable(Fast-Game-Library ${SOURCES})

# Link the OpenGL libraries
target_link_libraries(Fast-Game-Library glfw GL X11 pthread Xrandr Xi dl GLEW openal)
