
cmake_minimum_required(VERSION 3.2)
project(golf_game C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(i IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER "${i}" i)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${i} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()

# include_directories(
# 	${CMAKE_SOURCE_DIR}/lib/glm/
	
# 	${CMAKE_SOURCE_DIR}/include/
# 	${CMAKE_SOURCE_DIR}/lib/
# )
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
add_executable(golf ${SRC_FILES} ${CMAKE_SOURCE_DIR}/src/glad.c)

target_include_directories(golf PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/glm/
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/lib/
)
target_compile_options(golf PRIVATE -Wall -Werror -Wno-unused-but-set-variable -DDEBUG_MODE=false)

if(WIN)
    find_package(OpenGL REQUIRED)
    target_link_libraries(golf PRIVATE glfw ${OPENGL_gl_LIBRARY} gdi32 winmm user32)
else()
    target_link_libraries(golf PRIVATE glfw glut)
    target_link_options(golf PRIVATE -Wl,--copy-dt-needed-entries)
endif()

if (MINGW)
    set_target_properties(golf PROPERTIES BUILD_RPATH_USE_ORIGIN OFF INSTALL_PATH "")
endif()

# target_link_libraries(golf PRIVATE glfw glut)
# set_target_properties(golf PROPERTIES LINK_FLAGS "-Wl,--copy-dt-needed-entries")


