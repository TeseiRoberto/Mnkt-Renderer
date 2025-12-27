
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
 * @param vertices Array of data that defines the properties of each vertex that must be drawn.
 *      Vertices in this array are taken one by one to form points.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param pointSize Size of the points to be drawn expressed in pixels
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered points should be outputted
*/
void mnkt_draw2DPoint(void* vertices, const size_t verticesCount, const size_t pointSize, ShaderProgram_t* shader, Framebuffer_t* fb);


/**
 * @function mnkt_draw2DLine
 * Draws a 2D segmented line
 * @param vertices Array of data that defines the properties of each vertex that must be drawn.
 *      Vertices in this array are grouped two by two to form lines.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered line should be outputted
*/
void mnkt_draw2DLine(void* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb);


/**
 * @function mnkt_draw
 * Draws a sequence of triangles
 * @param vertices Array of data that defines the properties of each vertex that must be drawn.
 *      Vertices in this array are grouped three by three to form triangles.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered triangles should be outputted
*/
void mnkt_draw(void* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb);


#endif // MNKT_RENDERER_H


