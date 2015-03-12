cmake_minimum_required(VERSION 2.8.12)
project(monoEngine)

#--------------------------------------------------------------------
# Set version
#--------------------------------------------------------------------
set(MONOENGINE_VERSION_MAJOR "1")
set(MONOENGINE_VERSION_MINOR "0")
set(MONOENGINE_VERSION_PATCH "5")
set(MONOENGINE_VERSION_EXTRA "-OpenGL4.5")
set(MONOENGINE_VERSION "${MONOENGINE_VERSION_MAJOR}.${MONOENGINE_VERSION_MINOR}")
set(MONOENGINE_VERSION_FULL "${GLFW_VERSION}.${GLFW_VERSION_PATCH}${GLFW_VERSION_EXTRA}")

#--------------------------------------------------------------------
# Check OS
#--------------------------------------------------------------------
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(WINDOWS "TRUE")
endif ()
if (NOT UNIX AND NOT WINDOWS)
	message(FATAL_ERROR "Only UNIX and WINDOWS are supported so far!")
endif ()

#--------------------------------------------------------------------
# Check if static lib
#--------------------------------------------------------------------
option(STATIC_LIB "builds static library" OFF)
if (NOT STATIC_LIB)
	set(CMAKE_BUILD_TYPE Release)
endif ()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
if (UNIX)
	set(MY_WARNING_FLAGS " -Wall -pedantic -Wextra -Werror -Wcast-align -Wconversion -Wfloat-equal -Wformat=2 -Wmissing-declarations -Woverlength-strings -Wshadow -Wunreachable-code")
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(MY_WARNING_FLAGS "${MY_WARNING_FLAGS} -Wimplicit-atomic-properties -Wstrict-selector-match -Wundeclared-selector")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
	endif ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -msse2 -msse3 ${MY_WARNING_FLAGS}")
endif ()

#--------------------------------------------------------------------
# Glob files
#--------------------------------------------------------------------
file(GLOB_RECURSE CXX_SRCS src/*.cpp)
file(GLOB_RECURSE C_SRCS src/*.c)
file(GLOB_RECURSE HPP_HDRS src/*.hpp)
file(GLOB_RECURSE H_HDRS src/*.h)
set(SRCS "${C_SRCS};${CXX_SRCS}")
set(HDRS "${H_HDRS};${HPP_HDRS}")

#--------------------------------------------------------------------
# Find packages
#--------------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(Threads)

#--------------------------------------------------------------------
# GLFW
#--------------------------------------------------------------------
set(GLFW_DOWNLOAD_DIR "${PROJECT_SOURCE_DIR}/dl" CACHE STRING "specifies the GLFW download directory")
set(GLFW_DOWNLOAD_FILE "3.0.4.zip")
if (EXISTS ${GLFW_DOWNLOAD_DIR}/${GLFW_DOWNLOAD_FILE})
	set(GLFW_URL "${GLFW_DOWNLOAD_DIR}/${GLFW_DOWNLOAD_FILE}")
else ()
	set(GLFW_URL "https://github.com/glfw/glfw/archive/${GLFW_DOWNLOAD_FILE}")
endif ()
include(ExternalProject)

if (UNIX)
	ExternalProject_Add(
		glfw
		PREFIX "contrib"
		DOWNLOAD_DIR ${GLFW_DOWNLOAD_DIR}
		URL ${GLFW_URL}
		CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=/glfw
		INSTALL_COMMAND make DESTDIR=${PROJECT_SOURCE_DIR}/contrib install
	)
	set(GLFW_STATIC_LIB "${PROJECT_SOURCE_DIR}/contrib/glfw/lib/libglfw3.a")
	set(GLFW_LIBRARIES "-lX11 -lXrandr -lXi -lXxf86vm")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PROJECT_SOURCE_DIR}/contrib/glfw/include")
elseif (WINDOWS)
	ExternalProject_Add(
		glfw
		PREFIX "contrib"
		URL "https://github.com/glfw/glfw/archive/3.0.4.zip"
		CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DCMAKE_INSTALL_PREFIX=${PROJECT_SOURCE_DIR}/contrib/glfw
	)
	set(GLFW_STATIC_LIB "${PROJECT_SOURCE_DIR}/contrib/glfw/lib/glfw3.lib")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PROJECT_SOURCE_DIR}/contrib/glfw/include")
endif ()
add_definitions( -DGLFW_INCLUDE_NONE ) # we already include the glew headers

#--------------------------------------------------------------------
# GLEW
#--------------------------------------------------------------------
add_definitions( -DGLEW_NO_GLU -DGLEW_STATIC )

#--------------------------------------------------------------------
# GLM
#--------------------------------------------------------------------
if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${PROJECT_SOURCE_DIR}/contrib/glm")
elseif (WINDOWS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PROJECT_SOURCE_DIR}/contrib/glm")
endif ()
add_definitions( -DGLM_FORCE_RADIANS )

#--------------------------------------------------------------------
# Building
#--------------------------------------------------------------------
if (STATIC_LIB)
	add_library(monoEngine STATIC ${SRCS} ${HDRS})
else ()
	add_executable(monoEngine ${SRCS} ${HDRS})
	target_link_libraries(monoEngine ${GLFW_STATIC_LIB} ${OPENGL_gl_LIBRARY} ${GLFW_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif ()
