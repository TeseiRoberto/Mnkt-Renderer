
/**
 * @file rasterizer.h
 *
 * Contains definitions of functions to rasterize primitive graphic shapes.
*/


#ifndef MNKT_RASTERIZER_H
#define MNKT_RASTERIZER_H

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include "math/vec.h"
#include "math/mathUtils.h"
#include "utility/colorUtils.h"
#include "shader.h"
#include "framebuffer.h"


/**
 * @function mnkt_rasterizePoint
 * Rasterizes a 2D point and invokes the fragment shader for each fragment produced.
 * @param screenCoords Vector which defines the screen coordinates of the center of the point to be rasterized
 * @param pointSize Number of pixels that each side of the point takes up.
 *      Use 0 to rasterize a single pixel.
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader (will be interpolated and passed as input to the fragment shader)
 * @param fb Framebuffer on which the point will be rasterized
*/
void mnkt_rasterizePoint(Vec3_t screenCoords, const size_t pointSize, const ShaderProgram_t* shader, const ShaderParameter_t varyings[MAX_VARYING_PARAMS], Framebuffer_t* fb);


/**
 * @function mnkt_rasterizeLine
 * Rasterizes a 2D line and invokes the fragment shader for each fragment produced.
 * @param screenCoords Array of vectors which defines the screen coordinates of the two extreme points of the line to be rasterized
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader for the two extreme points of the line
 *      (those will be interpolated and passed as input to the fragment shader)
 * @param fb Framebuffer on which the line will be rasterized
*/
void mnkt_rasterizeLine(Vec3_t screenCoords[2], const ShaderProgram_t* shader, const ShaderParameter_t varyings[2][MAX_VARYING_PARAMS], Framebuffer_t* fb);


/*
 * @function mnkt_rasterizeTriangle
 * Rasterizes a 2D triangle and invokes the fragment shader for each fragment produced.
 * @param screenCoords Array of vectors which defines the screen coordinates of the vertices of the triangle to be rasterized
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader for the vertices of the triangle
 *      (those will be interpolated across the triangle surface and passed as input to the fragment shader)
 * @param fb Framebuffer on which the line will be rasterized
*/
void mnkt_rasterizeTriangle(Vec3_t screenCoords[3], const ShaderProgram_t* shader, const ShaderParameter_t varyings[3][MAX_VARYING_PARAMS], Framebuffer_t* fb);


#endif // MNKT_RASTERIZER_H
