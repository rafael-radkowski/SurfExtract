# Main cmake file 
cmake_minimum_required(VERSION 2.6)

# policy for relative subfolders
cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0074 NEW)


# Main project name
project(SurfExtract)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(OUTPUT_BINDIR ${CMAKE_SOURCE_DIR}/bin)
make_directory(${OUTPUT_BINDIR})


# Set the project architecture for this example
set( PROJECT_ARCH "x64" )

# All output files are copied to bin
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)



# Set the runtime output path
set( RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin")

# All library output files are copied to dep/lib
#set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Find packages
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(glew REQUIRED)
FIND_PACKAGE(glm REQUIRED)
FIND_PACKAGE(GLFW3 REQUIRED)
FIND_PACKAGE(Eigen3 )
FIND_PACKAGE(OpenCV REQUIRED)


if (GLEW_FOUND)
message(STATUS "Found GLEW ${GLEW_INCLUDE_DIR} and ${GLEW_LIBRARIES}")
else(GLEW_FOUND)
message(FATAL_ERROR  "Did not find GLEW")
endif(GLEW_FOUND)


if (GLFW_FOUND)
message(STATUS "Found GLEW ${GLFW_INCLUDE_DIRS} and ${GLFW3_LIBRARY}")
else()
IF(WIN32)
ELSE(WIN32)
include_directories("/usr/local/Cellar/glfw/3.2.1/include")
include_directories("/usr/local/Cellar/glfw/1.11.0/include")
set( GLFW3_LIBRARY glfw)
ENDIF(WIN32)
endif()




# Include dirs
include_directories("C:/SDK/opencv-3.4.5/build/include")
include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${GLFW3_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(./src)
include_directories(./include)
include_directories(./dep/GLSupport/include)


add_compile_definitions(_WITH_CAMERA)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


# create the directory for all dependencies and download the dependencies.
find_path( GLSUPPORT_DIR NAMES dep/GLSupport/README.md)

if( (NOT GLSUPPORT_DIR) OR (NOT EXISTS ${GLSUPPORT_DIR}))
	file(MAKE_DIRECTORY "./dep")

	execute_process(COMMAND git submodule update --init -- dep
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(GLSUPPORT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dep/GLSupport 
					CACHE PATH "glSupport dir" FORCE)


	add_library(GLUtils INTERFACE)
	target_include_directories(GLUtils INTERFACE
								$<BUILD_INTERFACE:${GLSUPPORT_DIR}/include>
								$<INSTALL_INTERFACE:./dep/lib>)
		
else()
  # update the submodule to the latest version.
  #execute_process(COMMAND git submodule update --remote --merge
  #	  				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


endif()
	

add_subdirectory(src)
