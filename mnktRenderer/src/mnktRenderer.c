
/**
 * @file mnktRenderer.c
 *
 * Contains implementation of the mnkt renderer graphics API
*/

#include "mnktRenderer.h"


/**
 * @function mnkt_draw2DPoint
 * Draws a sequence of 2D points of the given size
 * @param vertices Array that defines the x and y coordinates of each point to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param pointSize Size of the points to be drawn expressed in pixels
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered points should be outputted
*/
void mnkt_draw2DPoint(float* vertices, const size_t verticesCount, const size_t pointSize, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;
/*
        // Until 2D points can be extracted from the given vertices
        for(size_t i = 0; i + 2 <= verticesCount; i += 2)
        {
                Vec3_t in = { vertices[i], vertices[i + 1], 1 };

                // TODO: Understand how to pass additional parameters to the vertex shader...

                // Invoke the vertex shader on the current point
                Vec4_t out = vertexShader(in, shader->uniforms);
                
                // Perform perspective division and clipping
                out.x /= out.w;
                if(out.x < -1.0f || out.x > 1.0f)
                        continue;

                out.y /= out.w;
                if(out.y < -1.0f || out.y > 1.0f)
                        continue;

                out.z /= out.w;
                if(out.z < -1.0f || out.z > 1.0f)
                        continue;

                // TODO: Need to understand how to implement this stuff...
                // Determine all pixels covered by the point
                Vec2_t center;

                // Invoke fragment shader
                
        }*/
}


/**
 * @function mnkt_draw2DLine
 * Draws a 2D segmented line
 * @param vertices Array that defines the x and y coordinates of each point making up the line to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered line should be outputted
*/
void mnkt_draw2DLine(float* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;

        // TODO: Add implementation...
}


/**
 * @function mnkt_draw
 * Draws a sequence of triangles
 * @param vertices Array that defines the x, y and z coordinates of each point making up the triangle to be drawn
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered triangles should be outputted
*/
void mnkt_draw(float* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;

        // TODO: Add implementation...
}


