
cmake_minimum_required(VERSION 3.20)

project(MnktRenderer VERSION 1.0)

# Set name of the final compiled library
set ( TARGET_NAME ${PROJECT_NAME} )

# Set source files to be compiled
set (SRCS
        src/math/vec.c
        src/math/mathUtils.c

        src/utility/colorUtils.c

        src/framebuffer.c
        src/rasterizer.c
        src/mnktRenderer.c
)

add_library(${TARGET_NAME} ${SRCS})


# Add flags for errors during compilation
if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif() 


