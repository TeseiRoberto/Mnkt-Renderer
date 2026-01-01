
/**
 * @file rasterizer.h
 *
 * Contains definitions of functions to rasterize primitive graphic shapes.
*/


#ifndef MNKT_RASTERIZER_H
#define MNKT_RASTERIZER_H

#include <stddef.h>

#include "math/vec.h"
#include "math/mathUtils.h"
#include "utility/colorUtils.h"
#include "shader.h"
#include "framebuffer.h"


/**
 * Rasterizes a 2D point and invokes the fragment shader for each fragment produced.
 * @param screenCoords Vector which defines the screen coordinates of the center of the point to be rasterized
 * @param pointSize Size of the point to be rasterized, expressed in pixels
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader (will be interpolated and passed as input to the fragment shader)
 * @param fb Framebuffer on which the point will be rasterized
*/
void mnkt_rasterize2DPoint(const Vec3_t* screenCoords, const size_t pointSize, const ShaderProgram_t* shader, const ShaderParameter_t* varyings, Framebuffer_t* fb);


#endif // MNKT_RASTERIZER_H
