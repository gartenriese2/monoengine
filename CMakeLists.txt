cmake_minimum_required(VERSION 2.8.0)
include(ExternalProject)

project(monoEngine)

#
# misc
#
#set(EXTERN_DIR "${PROJECT_SOURCE_DIR}/extern")
set(CMAKE_BUILD_TYPE Release)


#
# compiler & flags
#

# warnings
set(MY_WARNING_FLAGS " -Wall -pedantic -Wextra -Werror -Wcast-align -Wconversion -Wfloat-equal -Wformat=2 -Wimplicit-atomic-properties -Wmissing-declarations -Woverlength-strings -Wshadow -Wstrict-selector-match -Wundeclared-selector -Wunreachable-code ")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -msse2 -msse3 ${MY_WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer")
add_definitions( -I${PROJECT_SOURCE_DIR}/src )

#
# Files
#
file(GLOB_RECURSE CXX_SRCS src/*.cpp)
file(GLOB_RECURSE C_SRCS src/*.c)
file(GLOB_RECURSE HDRS1 src/*.h)
file(GLOB_RECURSE HDRS1 src/*.hpp)
set(SRCS "${C_SRCS};${CXX_SRCS}")
set(HDRS "${HDRS1};${HDRS2}")

# Packages
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads)

# GLFW
ExternalProject_Add(
    glfw
    PREFIX "contrib"
    URL "${PROJECT_SOURCE_DIR}/contrib/glfw/glfw-3.0.4.zip"
    CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=/glfw
    INSTALL_COMMAND make DESTDIR=${PROJECT_SOURCE_DIR}/contrib install
)
set(GLFW_STATIC_LIB "${PROJECT_SOURCE_DIR}/contrib/glfw/lib/libglfw3.a")
set(GLFW_LIBRARIES "-lX11 -lXrandr -lXi -lXxf86vm")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PROJECT_SOURCE_DIR}/contrib/glfw/include")
add_definitions( -DGLFW_INCLUDE_NONE ) # we already include the glew headers

# Glew
add_definitions( -DGLEW_NO_GLU -DGLEW_STATIC )

# glm
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PROJECT_SOURCE_DIR}/contrib/glm")
add_definitions( -DGLM_FORCE_RADIANS )

# stb_image
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PROJECT_SOURCE_DIR}/contrib/imgui")

add_executable(monoEngine ${SRCS} ${HDRS})

target_link_libraries(monoEngine ${GLFW_STATIC_LIB} ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
