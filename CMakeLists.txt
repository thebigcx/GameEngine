cmake_minimum_required(VERSION 3.13) # Minimum CMake version
project(FGL)                         # Create project 'FGL'
set(CMAKE_CXX_STANDARD 20)           # Set the C++ standard VERSION

# Compiler optimisations
set(CMAKE_CXX_FLAGS "-O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin) # Set the binary output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib) # Set the library output directory

# Compile all .c and .cpp files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.c)

# Compile the tests
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
sandbox/Sandbox.cpp
)

# Compile the static library
add_library(FGL STATIC ${SOURCES})

# Link the OpenGL libraries
target_link_libraries(FGL glfw GL X11 pthread Xrandr Xi dl GLEW openal)

# Set the include directory
target_include_directories(FGL PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(FGL PUBLIC ${CMAKE_SOURCE_DIR}/include/vendor)

# Build the test executable
add_executable(Library_Test ${TEST_SOURCES})

# Set the include directory
target_include_directories(Library_Test PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(Library_Test PUBLIC ${CMAKE_SOURCE_DIR}/include/vendor)

# Statically link the game library
target_link_libraries(Library_Test FGL)

