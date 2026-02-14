
/**
 * @file mnktRenderer.c
 *
 * Contains implementation of the mnkt renderer graphics API
*/

#include "mnktRenderer.h"

static int      mnkt_isVertexVisible(const Vec4_t* vertex);
static int      mnkt_clipLine(Vec4_t vertices[2]);
static int      mnkt_clipTriangle(Vec4_t vertices[3], Vec4_t additionalTriangle[3]);

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
                mnkt_rasterizePoint(screenCoords, pointSize, shader, varyings, fb);
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
                if(mnkt_clipLine(clipCoords) != 2)
                        continue;

                // Perform perspective division and convert from ndc space to screen space
                for(size_t j = 0; j < 2; ++j)
                {
                        clipCoords[j] = mnkt_vec4_div(&clipCoords[j], clipCoords[j].w);
                        screenCoords[j] = mnkt_ndcToScreenCoords(clipCoords[j], fb->width, fb->height);
                }

                // Rasterize the line
                mnkt_rasterizeLine(screenCoords, shader, varyings, fb);
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

        Vec4_t clipCoords[6];
        Vec3_t screenCoords[6];
        ShaderParameter_t varyings[6][MAX_VARYING_PARAMS];

        char* currVertexData = (char*) vertices;

        // Until triangles can be built from the given vertices
        for(size_t i = 0; i + 3 <= verticesCount; i += 3)
        {
                // Invoke vertex shader on each vertex
                for(int j = 0; j < 3; ++j, currVertexData += shader->vertexSize)
                        clipCoords[j] = shader->vertexShader(currVertexData, varyings[j], shader->uniforms);

                // Perform clipping (discard the triangle if clipping fails)
                size_t clippedVerticesNum = mnkt_clipTriangle( clipCoords, &(clipCoords[3]) );

                if(clippedVerticesNum < 3)
                        continue;

                // Perform perspective division and convert from ndc to screen space
                for(size_t j = 0; j < clippedVerticesNum; ++j)
                {
                        clipCoords[j] = mnkt_vec4_div(&clipCoords[j], clipCoords[j].w);
                        screenCoords[j] = mnkt_ndcToScreenCoords(clipCoords[j], fb->width, fb->height);
                }

                // Rasterize the triangle
                mnkt_rasterizeTriangle(screenCoords, shader, varyings, fb);

                // Rasterize the additional triangle produced by the clipping process, if necessary
                if(clippedVerticesNum > 3)
                        mnkt_rasterizeTriangle( &(screenCoords[3]), shader, varyings + 3, fb );
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
 * @param vertices Vertices, expressed in clip coordinates, which define the extremes of the line to be clipped
 * @return The number of vertices correctly clipped (that must be redered).
 *      Zero if the given line does not intersect the clipping volume (must be discared).
*/
static int mnkt_clipLine(Vec4_t vertices[2])
{
        // TODO: FIX THIS!!! (need to implement clipping properly...)
        vertices[0].x = mnkt_math_clamp(vertices[0].x, -1.0f, 1.0f);
        vertices[0].y = mnkt_math_clamp(vertices[0].y, -1.0f, 1.0f);

        vertices[1].x = mnkt_math_clamp(vertices[1].x, -1.0f, 1.0f);
        vertices[1].y = mnkt_math_clamp(vertices[1].y, -1.0f, 1.0f);

        return 2;
}


/**
 * @function mnkt_clipTriangle
 * Performs clipping on the triangle defined by the given vertices
 * @param vertices Vertices, expressed in clip coordinates, which define the triangle to be clipped
 * @param additionalTriangle Array in which the vertices of an additional triangle, resulting from the clipping process, are stored if necessary
 * @return The number of vertices correctly clipped (that must be redered).
 *      Zero if the given triangle does not intersect the clipping volume (must be discared).
*/
static int mnkt_clipTriangle(Vec4_t vertices[3], Vec4_t additionalTriangle[3])
{
        // TODO: FIX THIS!!! (need to implement clipping properly...)
        vertices[0].x = mnkt_math_clamp(vertices[0].x, -1.0f, 1.0f);
        vertices[1].y = mnkt_math_clamp(vertices[1].y, -1.0f, 1.0f);
        vertices[2].z = mnkt_math_clamp(vertices[2].z, -1.0f, 1.0f);

        return 3;
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


