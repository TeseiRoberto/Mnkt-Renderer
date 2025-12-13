
cmake_minimum_required(VERSION 3.20)

project(imageGen VERSION 1.0)

set ( MNKT_RENDERER_LIB_PATH ${CMAKE_SOURCE_DIR}/../mnktRenderer/bin/libMnktRenderer.a)
set ( MNKT_RENDERER_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/../mnktRenderer/src)

set (SRCS
        src/main.c
)

add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${MNKT_RENDERER_INCLUDE_PATH})
target_link_libraries(${PROJECT_NAME} ${MNKT_RENDERER_LIB_PATH})
