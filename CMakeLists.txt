cmake_minimum_required(VERSION 3.10)
project(vulkan_test)

file(GLOB SOURCE_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall)

find_package(SDL3 REQUIRED)
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3 Vulkan::Vulkan)
