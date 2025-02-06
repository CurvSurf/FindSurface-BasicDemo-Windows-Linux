cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CSS_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

set(TARGET_NAME basic_demo_cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable(${TARGET_NAME} main.cpp pointcloud.cpp)
target_include_directories(${TARGET_NAME} PRIVATE ../include)
target_link_directories(${TARGET_NAME} PRIVATE ../lib)
target_link_libraries(${TARGET_NAME} FindSurface)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/lib $<TARGET_FILE_DIR:${TARGET_NAME}>
    COMMENT "Copying shared libraries to $<TARGET_FILE_DIR:${TARGET_NAME}> directory"
)