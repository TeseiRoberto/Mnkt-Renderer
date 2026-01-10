
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
 * Rasterizes a 2D point and invokes the fragment shader for each fragment produced.
 * @param screenCoords Vector which defines the screen coordinates of the center of the point to be rasterized
 * @param pointSize Number of pixels that each side of the point takes up.
 *      Use 0 to rasterize a single pixel.
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader (will be interpolated and passed as input to the fragment shader)
 * @param fb Framebuffer on which the point will be rasterized
*/
void mnkt_rasterize2DPoint(Vec3_t screenCoords, const size_t pointSize, const ShaderProgram_t* shader, const ShaderParameter_t* varyings, Framebuffer_t* fb);


/**
 * Rasterizes a 2D line and invokes the fragment shader for each fragment produced.
 * @param screenCoordsA Vector which defines the screen coordinates of the first point of the line to be rasterized
 * @param screenCoordsB Vector which defines the screen coordinates of the second point of the line to be rasterized
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyingsA Additional output parameters produced by the vertex shader for the first point of the line (will be interpolated and passed as input to the fragment shader)
 * @param varyingsB Additional output parameters produced by the vertex shader for the second point of the line (will be interpolated and passed as input to the fragment shader)
 * @param fb Framebuffer on which the line will be rasterized
*/
void mnkt_rasterize2DLine(Vec3_t screenCoordsA, Vec3_t screenCoordsB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb);


#endif // MNKT_RASTERIZER_H
