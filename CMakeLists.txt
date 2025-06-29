
cmake_minimum_required(VERSION 3.2)
project(golf_game CXX C)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

include_directories(
	${CMAKE_SOURCE_DIR}/lib/glm/
	
	${CMAKE_SOURCE_DIR}/include/
	${CMAKE_SOURCE_DIR}/lib/
)
file(GLOB_RECURSE SRC_FILES
	${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(golf ${SRC_FILES} ${CMAKE_SOURCE_DIR}/src/glad.c)
target_compile_options(golf PRIVATE -Wall -Werror -Wno-unused-but-set-variable -DDEBUG_MODE=false)

target_link_libraries(golf PRIVATE glfw glut)
set_target_properties(golf PROPERTIES LINK_FLAGS "-Wl,--copy-dt-needed-entries")


