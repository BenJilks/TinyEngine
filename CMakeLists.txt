
cmake_minimum_required(VERSION 3.14)
project(TinyEngine)
set(CMAKE_CXX_STANDARD 11)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/source/*.cpp"
        "${PROJECT_SOURCE_DIR}/source/*.c"
)

add_executable(AutoBlock ${all_SRCS})
target_link_libraries(AutoBlock ${GLEW_LIBRARIES})
target_link_libraries(AutoBlock glfw dl)
target_link_libraries(AutoBlock ${OPENGL_LIBRARIES})
