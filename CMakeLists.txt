cmake_minimum_required(VERSION 2.8.0)
project(monoEngine)

set(MonoEngine_VERSION_MAJOR "1")
set(MonoEngine_VERSION_MINOR "0")
set(MonoEngine_VERSION_PATCH "3")
set(MonoEngine_VERSION_EXTRA "-OpenGL4.5")
set(MonoEngine_VERSION "${MonoEngine_VERSION_MAJOR}.${MonoEngine_VERSION_MINOR}")
set(MonoEngine_VERSION_FULL "${GLFW_VERSION}.${GLFW_VERSION_PATCH}${GLFW_VERSION_EXTRA}")

set(CMAKE_BUILD_TYPE Release)

# check OS
if (NOT UNIX AND NOT MSCV)
	message(FATAL_ERROR "Only UNIX and MSCV are supported so far!")
endif ()

# warnings
set(MY_WARNING_FLAGS " -Wall -pedantic -Wextra -Werror -Wcast-align -Wconversion -Wfloat-equal -Wformat=2 -Wimplicit-atomic-properties -Wmissing-declarations -Woverlength-strings -Wshadow -Wstrict-selector-match -Wundeclared-selector -Wunreachable-code ")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -msse2 -msse3 ${MY_WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer")
add_definitions( -I${PROJECT_SOURCE_DIR}/src )

# Files
file(GLOB_RECURSE CXX_SRCS src/*.cpp)
file(GLOB_RECURSE C_SRCS src/*.c)
file(GLOB_RECURSE HPP_HDRS src/*.hpp)
file(GLOB_RECURSE H_HDRS src/*.h)
set(SRCS "${C_SRCS};${CXX_SRCS}")
set(HDRS "${H_HDRS};${HPP_HDRS}")

# Packages
find_package(OpenGL REQUIRED)
find_package(Threads)

# GLFW
include(ExternalProject)
if (UNIX)
	ExternalProject_Add(
	    glfw
	    PREFIX "contrib"
	    URL "https://github.com/glfw/glfw/archive/3.0.4.zip"
	    CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=/glfw
	    INSTALL_COMMAND make DESTDIR=${PROJECT_SOURCE_DIR}/contrib install
	)
	set(GLFW_STATIC_LIB "${PROJECT_SOURCE_DIR}/contrib/glfw/lib/libglfw3.a")
	set(GLFW_LIBRARIES "-lX11 -lXrandr -lXi -lXxf86vm")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PROJECT_SOURCE_DIR}/contrib/glfw/include")
elseif (MSCV)
	ExternalProject_Add(
	    glfw
	    PREFIX "contrib"
	    URL "https://github.com/glfw/glfw/archive/3.0.4.zip"
	    CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=/glfw
	    INSTALL_COMMAND make DESTDIR=${PROJECT_SOURCE_DIR}/contrib install
	)
	set(GLFW_STATIC_LIB "${PROJECT_SOURCE_DIR}/contrib/glfw/lib/libglfw3.a")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PROJECT_SOURCE_DIR}/contrib/glfw/include")
endif ()
add_definitions( -DGLFW_INCLUDE_NONE ) # we already include the glew headers

# Glew
add_definitions( -DGLEW_NO_GLU -DGLEW_STATIC )

# glm
if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PROJECT_SOURCE_DIR}/contrib/glm")
elseif (MSCV)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PROJECT_SOURCE_DIR}/contrib/glm")
endif ()
add_definitions( -DGLM_FORCE_RADIANS )

add_executable(monoEngine ${SRCS} ${HDRS})

target_link_libraries(monoEngine ${GLFW_STATIC_LIB} ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
