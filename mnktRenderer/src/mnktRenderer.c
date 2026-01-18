
/**
 * @file mnktRenderer.c
 *
 * Contains implementation of the mnkt renderer graphics API
*/

#include "mnktRenderer.h"

static int      mnkt_isVertexVisible(const Vec4_t* vertex);
static int      mnkt_clipLine(Vec4_t* pointA, Vec4_t* pointB);

static Vec3_t   mnkt_ndcToScreenCoords(Vec4_t clipCoords, size_t screenWidth, size_t screenHeight);


/**
 * @function mnkt_drawPoints
 * Draws a sequence of 2D points of the given size
 * @param vertices Array of data that defines the properties of each vertex that must be drawn.
 *      Vertices in this array are taken one by one to form points.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param pointSize Size of the points to be drawn, expressed in pixels
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered points should be outputted
*/
void mnkt_drawPoints(void* vertices, const size_t verticesCount, const size_t pointSize, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;

        Vec4_t clipCoords;
        Vec3_t screenCoords;
        ShaderParameter_t varyings[MAX_VARYING_PARAMS];

        char* currVertexData = vertices;

        // Until points can be extracted from the given vertices
        for(size_t i = 0; i < verticesCount; ++i, currVertexData += shader->vertexSize)
        {
                // Invoke the vertex shader on the current point
                clipCoords = shader->vertexShader(currVertexData, varyings, shader->uniforms);
                
                // Perform clipping (for simplicity, as OpenGL standard specifies, we discard the point if its center is not inside the view volume)
                if( !mnkt_isVertexVisible(&clipCoords) )
                        continue;

                // Perform perspective division
                clipCoords = mnkt_vec4_div(&clipCoords, clipCoords.w);

                // Convert ndc coordinates to screen coordinates
                screenCoords = mnkt_ndcToScreenCoords(clipCoords, fb->width, fb->height);

                // Rasterize the point
                mnkt_rasterize2DPoint(screenCoords, pointSize, shader, varyings, fb);
        }
}


/**
 * @function mnkt_drawLines
 * Draws a sequence of lines non connected between each other
 * @param vertices Array of data that defines the properties of each vertex that composes the lines to be drawn,
 *      Vertices in this array are grouped two by two to form non connected lines.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered line should be outputted
*/
void mnkt_drawLines(void* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;

        Vec4_t clipCoords[2];
        Vec3_t screenCoords[2];
        ShaderParameter_t varyings[2][MAX_VARYING_PARAMS];

        char* currVertexData = vertices;

        // Until points can be extracted from the given vertices
        for(size_t i = 0; i + 2 <= verticesCount; i += 2)
        {
                // Invoke vertex shader on each vertex
                for(size_t j = 0; j < 2; ++j, currVertexData += shader->vertexSize)
                        clipCoords[j] = shader->vertexShader(currVertexData, varyings[j], shader->uniforms);

                // Perform clipping (discard the line if clipping fails)
                if( !mnkt_clipLine(&clipCoords[0], &clipCoords[1]) )
                        continue;

                // Perform perspective division and convert from ndc space to screen space
                for(size_t j = 0; j < 2; ++j)
                {
                        clipCoords[j] = mnkt_vec4_div(&clipCoords[j], clipCoords[j].w);
                        screenCoords[j] = mnkt_ndcToScreenCoords(clipCoords[j], fb->width, fb->height);
                }

                // Rasterize the line
                mnkt_rasterize2DLine(screenCoords[0], screenCoords[1], shader, varyings[0], varyings[1], fb);
        }
}


/**
 * @function mnkt_drawPolyLine
 * Draws a continuous segmented line
 * @param vertices Array of data that defines the properties of each vertex that composes the line to be drawn.
 *      Vertices in this array define the points that are to be connected by the line.
 * @param verticesCount Number of elements stored in the given vertices array
 * @param shader Shader program to be used for drawing
 * @param fb Framebuffer on which the rendered line should be outputted
*/
void mnkt_drawPolyLine(void* vertices, const size_t verticesCount, ShaderProgram_t* shader, Framebuffer_t* fb)
{
        if(vertices == NULL || shader == NULL || fb == NULL)
                return;

        char* currVertexData = vertices;

        for(size_t i = 0; i + 2 <= verticesCount; ++i, currVertexData += shader->vertexSize)
                mnkt_drawLines(currVertexData, 2, shader, fb);
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
        char* currVertexData = (char*) vertices;

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

                // TODO: Implement clipping and perspective division
        
                // TODO: Rasterize the triangle and invoke the fragment shader
        }
}


/**
 * @function mnkt_isVertexVisible
 * Checks if the given vertex is visible according to its w parameter
 * @param vertex The vertex to be checked, expressed in clip coordinates
 * @return One if the vertex is visible, zero otherwise
*/
static int mnkt_isVertexVisible(const Vec4_t* vertex)
{
        return ( fabs(vertex->x) <= vertex->w && fabs(vertex->y) <= vertex->w && fabs(vertex->z) <= vertex->w );
}


/**
 * @function mnkt_clipLine
 * Performs clipping on the line defined by the given vertices
 * @param pointA, pointB Points, expressed in clip coordinates, which define the line to be clipped
 * @return Zero if the given line does not intersect the clipping volume (must be discared), one otherwise
*/
static int mnkt_clipLine(Vec4_t* pointA, Vec4_t* pointB)
{
        // TODO: FIX THIS!!! (need to implement clipping properly...)
        pointA->x = mnkt_math_clamp(pointA->x, -1.0f, 1.0f);
        pointA->y = mnkt_math_clamp(pointA->y, -1.0f, 1.0f);

        pointB->x = mnkt_math_clamp(pointB->x, -1.0f, 1.0f);
        pointB->y = mnkt_math_clamp(pointB->y, -1.0f, 1.0f);

        return 1;
}


/**
 * @function mnkt_ndcToScreenCoords
 * Converts NDC coordinates to screen coordinates (applies the viewport transform)
 * @param ndcCoords The coordinates to be converted from NDC to screen space
 * @param screenWidth The width of the screen
 * @param screenHeight The height of the screen
 * @return A Vec3 which defines the coordinates, in screen space, of the given point
*/
static Vec3_t mnkt_ndcToScreenCoords(Vec4_t ndcCoords, size_t screenWidth, size_t screenHeight)
{
        return (Vec3_t)
        {
                .x = ( (ndcCoords.x + 1) / 2 ) * screenWidth,
                .y = ( ( (-1 * ndcCoords.y) + 1) / 2) * screenHeight,
                .z = ( (ndcCoords.z + 1) / 2 )
        };
}


