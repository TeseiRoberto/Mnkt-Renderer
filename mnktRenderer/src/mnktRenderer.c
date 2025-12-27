
/**
 * @file mnktRenderer.c
 *
 * Contains implementation of the mnkt renderer graphics API
*/

#include "mnktRenderer.h"


/**
 * @function mnkt_draw2DPoint
 * Draws a sequence of 2D points of the given size
 * @param vertices Array of data that defines the properties of each vertex that must be drawn.
 *      Vertices in this array are taken one by one to form points.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param pointSize Size of the points to be drawn, expressed in pixels
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered points should be outputted
*/
void mnkt_draw2DPoint(void* vertices, const size_t verticesCount, const size_t pointSize, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;

        /*Vec4_t clipCoords;
        void* currVertexData = vertices;

        ShaderParameter_t varyings[MAX_VARYING_PARAMS];

        // Until points can be extracted from the given vertices
        for(size_t i = 0; i < verticesCount; ++i)
        {
                // Invoke the vertex shader on the current point
                clipCoords = vertexShader(currVertexData, varyings, shader->uniforms);
                
                // TODO: Implement perspective division and clipping

                // TODO: Rasterize the point and invoke the fragment shader
                
                // Advance into the vertices data
                currVertexData += shader->vertexSize;
        }*/
}


/**
 * @function mnkt_draw2DLine
 * Draws a 2D segmented line
 * @param vertices Array of data that defines the properties of each vertex that must be drawn.
 *      Vertices in this array are grouped two by two to form lines.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered line should be outputted
*/
void mnkt_draw2DLine(void* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;

        /*Vec4_t clipCoords[2];
        void* currVertexData = vertices;

        ShaderParameter_t varyings[MAX_VARYING_PARAMS];

        // Until points can be extracted from the given vertices
        for(size_t i = 0; i + 2 <= verticesCount; i += 2)
        {
                // Invoke vertex shader on each vertex
                for(size_t j = 0: j < 2; ++j)
                {
                        clipCoords[j] = vertexShader(currVertexData, varyings, shader->uniforms);
                
                        // Advance into the vertices data
                        currVertexData += shader->vertexSize;
                }

                // TODO: Implement perspective division and clipping

                // TODO: Rasterize the point and invoke the fragment shader
                
        }*/
}


/**
 * @function mnkt_draw
 * Draws a sequence of triangles
 * @param vertices Array of data that defines the properties of each vertex that must be drawn.
 *      Vertices in this array are grouped three by three to form triangles.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered triangles should be outputted
*/
void mnkt_draw(void* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;

        Vec4_t clipCoords[3];
        void* currVertexData = vertices;

        ShaderParameter_t varyings[3][MAX_VARYING_PARAMS];

        // Until triangles can be built from the given vertices
        for(size_t i = 0; i + 3 <= verticesCount; i += 3)
        {
                // Invoke vertex shader on each vertex
                for(int j = 0; j < 3; ++j)
                {
                        clipCoords[j] = shader->vertexShader(currVertexData, varyings[j], shader->uniforms);

                        // Advance into the vertices data
                        currVertexData += shader->vertexSize;
                }

                printf("\n");

                // TODO: Implement perspective division and clipping
        
                // TODO: Rasterize the triangle and invoke the fragment shader
        }
}


