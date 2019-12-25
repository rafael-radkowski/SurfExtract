# Main cmake file 
cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0015 NEW)

file(GLOB SURFE_SRC
    "*.cpp"
)

file(GLOB SURFE_INC
    "*.h"
)

source_group(src FILES ${SURFE_SRC})
source_group(include FILES ${SURFE_INC})


# remove the export file from the main app
set(Console_src ${SURFE_SRC} ${SURFE_INC})
list(REMOVE_ITEM Console_src ${CMAKE_CURRENT_SOURCE_DIR}/SurfExtractExport.cpp)
list(REMOVE_ITEM Console_src ${CMAKE_CURRENT_SOURCE_DIR}/SurfExtractAppEx.h)
list(REMOVE_ITEM Console_src ${CMAKE_CURRENT_SOURCE_DIR}/SurfExtractAppEx.cpp)

# remove the main.cpp file from the dll
set(Dll_src ${SURFE_SRC} ${SURFE_INC})
list(REMOVE_ITEM Dll_src ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)



add_compile_definitions(_WITH_CAMERA)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
add_compile_definitions(NOMINMAX)
add_compile_definitions(_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_definitions(-DWITH_EXPORT)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OutputDir})
set(CMAKE_DEBUG_POSTFIX d)


link_directories(${CMAKE_SOURCE_DIR}/lib)



# Create an executable
add_executable(${PROJECT_NAME}  ${Console_src} )

set(GLSupport
	debug GLUtilsd.lib
	optimized GLUtils.lib
)

# add GLUtils as a dependency
add_dependencies(${PROJECT_NAME} GLUtils)

# Add libraries
target_link_libraries(${PROJECT_NAME}   ${GLEW_LIBS}  ${GLFW3_LIBS} ${OPENGL_LIBRARIES}  ${OpenCV_LIBS}  ${GLSupport} )


option(WITH_LIB "Build a lib" OFF)

if(WITH_LIB)
	# Create an executable
	add_library(${PROJECT_NAME}Lib SHARED  ${Dll_src} )


	# Add libraries
	target_link_libraries(${PROJECT_NAME}Lib   ${GLEW_LIBs}  ${GLFW3_LIB} ${OPENGL_LIBRARIES} ${OpenCV_LIBS}  ${GLSupport} )
	
endif()

option(WITH_TEST "Build test cases" OFF)

if(WITH_TEST)
# add subdirectory
add_subdirectory(tests)

endif()
