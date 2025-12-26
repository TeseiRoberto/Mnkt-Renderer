
/**
 * @file mnktRenderer.h
 *
 * Defines the graphics API implemented by the mnktRenderer.
*/

#ifndef MNKT_RENDERER_H
#define MNKT_RENDERER_H

#include <stdint.h>
#include <stddef.h>

#include "image.h"
#include "shader.h"
#include "framebuffer.h"


/**
 * @function mnkt_draw2DPoint
 * Draws a sequence of 2D points of the given size
 * @param vertices Array that defines the x and y coordinates of each point to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param pointSize Size of the points to be drawn expressed in pixels
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered points should be outputted
*/
void mnkt_draw2DPoint(float* vertices, const size_t verticesCount, const size_t pointSize, ShaderProgram_t* shader, Framebuffer_t* fb);


/**
 * @function mnkt_draw2DLine
 * Draws a 2D segmented line
 * @param vertices Array that defines the x and y coordinates of each point making up the line to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered line should be outputted
*/
void mnkt_draw2DLine(float* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb);


/**
 * @function mnkt_draw
 * Draws a sequence of triangles
 * @param vertices Vertices that defines the triangles to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered triangles should be outputted
*/
void mnkt_draw(float* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb);


#endif // MNKT_RENDERER_H


