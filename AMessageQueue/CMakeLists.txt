# 最低要求版本
cmake_minimum_required (VERSION 2.8)

project(AMessageQueue)

set(IS_LIBRARY ON)
set(CUSTOM_SOURCE_DIR
    src
    src/socket
    src/utils
)

set(CUSTOM_3RD_MODULE_DEP
    libev
)

include("${CMAKE_SOURCE_DIR}/CMakeListsTemplate.txt")

add_library(${PROJECT_NAME} STATIC ${ALL_SOURCE})

# set_target_properties(${PROJECT_NAME}
#     PROPERTIES
#     OUTPUT_NAME   "${PROJECT_NAME}"
#     PREFIX        "lib"
#     PUBLIC_HEADER "include/AMessageQueue.h"
#     )
# install(TARGETS ${PROJECT_NAME}
#         RUNTIME DESTINATION       ${PROJECT_BINARY_DIR}/bin
#         LIBRARY DESTINATION       ${PROJECT_BINARY_DIR}/lib
#         ARCHIVE DESTINATION       ${PROJECT_BINARY_DIR}/lib/static
#         PUBLIC_HEADER DESTINATION ${PROJECT_BINARY_DIR}/include
#         )
