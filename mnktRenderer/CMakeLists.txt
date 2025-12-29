
cmake_minimum_required(VERSION 3.20)

project(MnktRenderer VERSION 1.0)

set (SRCS
        src/mnktRenderer.c
        src/framebuffer.c
        src/vec.c
)

add_library(${PROJECT_NAME} ${SRCS})
