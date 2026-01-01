
cmake_minimum_required(VERSION 3.20)

project(imageGen VERSION 1.0)

# Set name of the final compiled executable
set ( TARGET_NAME ${PROJECT_NAME} )

# Set paths for the mnktRenderer library
set ( MNKT_RENDERER_LIB_PATH ${CMAKE_SOURCE_DIR}/../mnktRenderer/bin/libMnktRenderer.a)
set ( MNKT_RENDERER_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/../mnktRenderer/src)

# Set source files to be compiled
set (SRCS
        src/main.c
)

add_executable(${TARGET_NAME} ${SRCS})

target_include_directories(${TARGET_NAME} PRIVATE ${MNKT_RENDERER_INCLUDE_PATH})
target_link_libraries(${TARGET_NAME} ${MNKT_RENDERER_LIB_PATH})


# Add flags for errors during compilation
if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()


