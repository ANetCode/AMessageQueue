# 最低要求版本
cmake_minimum_required (VERSION 2.8)

project(Test_Sample)

set(IS_LIBRARY OFF)
set(CUSTOM_SOURCE_DIR src)
set(CUSTOM_MODULE_DEP AMessageQueue)

set(CUSTOM_3RD_MODULE_DEP
    libev
)

include_directories(${CMAKE_SOURCE_DIR}/AMessageQueue/include)

include("${CMAKE_SOURCE_DIR}/CMakeListsTemplate.txt")

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} AMessageQueue ev)