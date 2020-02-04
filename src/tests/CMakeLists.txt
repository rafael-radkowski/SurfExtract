# Main cmake file 
cmake_minimum_required(VERSION 2.6)




file(GLOB testfiles_SRC
    "*.h"
    "*.cpp"
)


set(files_to_test
	../ReaderWriterPLY.h
	../ReaderWriterPLY.cpp
	../ReaderWriterOBJ.h
	../ReaderWriterOBJ.cpp
	../ReaderWriter.h
	../FileUtilsExt.h
	../FileUtilsExt.cpp
)

source_group(src FILES ${testfiles_SRC})
source_group(testfiles FILES ${files_to_test})


add_compile_definitions(_WITH_CAMERA)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OutputDir})
set(CMAKE_DEBUG_POSTFIX d)


link_directories(${CMAKE_SOURCE_DIR}/dep/lib)

# Create an executable
add_executable(tests  ${testfiles_SRC} ${files_to_test})

set(tests
	debug GLUtilsd.lib
	optimized GLUtils.lib
)

# add GLUtils as a dependency
add_dependencies(${PROJECT_NAME} GLUtils)


# Add libraries
target_link_libraries(tests  ${GLEW_LIBRARIES} ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES}  ${OPENCV_LIBRARIES}  ${GLSupport} )



# Add libraries
target_link_libraries(tests   ${GLEW_LIBRARIES} ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES} ${OPENCV_LIBRARIES}  ${GLSupport} )


