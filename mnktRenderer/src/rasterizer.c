
/**
 * @file rasterizer.c
 *
 * Contains implementation of functions to rasterize primitive graphic shapes
*/

#include "rasterizer.h"

#include <stdio.h>


// Prototypes for internal functions

static void     mnkt_rasterize2DHorLine(Vec3_t* pointA, Vec3_t* pointB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb);
static void     mnkt_rasterize2DVertLine(Vec3_t* pointA, Vec3_t* pointB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb);

static void     mnkt_drawFragment(const Vec2_t* fragCoords, float fragDepth, size_t fragIndex, const ShaderProgram_t* shader, const ShaderParameter_t* varyings, Framebuffer_t* fb);


/**
 * Rasterizes a 2D point and invokes the fragment shader for each fragment produced.
 * @param screenCoords Vector which defines the screen coordinates of the center of the point to be rasterized
 * @param pointSize Number of pixels that each side of the point takes up.
 *      Use 0 to rasterize a single pixel.
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader (will be interpolated and passed as input to the fragment shader)
 * @param fb Framebuffer on which the point will be rasterized
*/
void mnkt_rasterize2DPoint(Vec3_t screenCoords, const size_t pointSize, const ShaderProgram_t* shader, const ShaderParameter_t* varyings, Framebuffer_t* fb)
{
        if(shader == NULL || varyings == NULL || fb == NULL)
                return;

        // Ensure that the given coordinate for the point is in the valid range
        if( !mnkt_math_pointIntersectRect(screenCoords.x, screenCoords.y, 0.0f, 0.0f, fb->width - 1, fb->height - 1) )
                return;

        // Determine area of the framebuffer on which point will be rasterized
        Vec2_t startCoords;
        startCoords.x = mnkt_math_clamp(screenCoords.x - pointSize, 0, fb->width - 1);
        startCoords.y = mnkt_math_clamp(screenCoords.y - pointSize, 0, fb->height - 1);

        Vec2_t endCoords;
        endCoords.x = mnkt_math_clamp(screenCoords.x + pointSize, 0, fb->width - 1);
        endCoords.y = mnkt_math_clamp(screenCoords.y + pointSize, 0, fb->height - 1);

        Vec2_t fragCoords;

        for(size_t y = startCoords.y; y <= endCoords.y; ++y)
        {
                fragCoords.y = y;
                size_t fragIndex = (y * fb->width) + (size_t) startCoords.x;

                for(size_t x = startCoords.x; x <= endCoords.x; ++x, ++fragIndex)
                {
                        fragCoords.x = x;

                        // TODO: Should we interpolate varyings??? (A point is made of a single vertex but multiple fragments may be produced...)
                        mnkt_drawFragment(&fragCoords, screenCoords.z, fragIndex, shader, varyings, fb);
                }
        }

}


/**
 * Rasterizes a 2D line and invokes the fragment shader for each fragment produced.
 * @param screenCoordsA Vector which defines the screen coordinates of the first point of the line to be rasterized
 * @param screenCoordsB Vector which defines the screen coordinates of the second point of the line to be rasterized
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyingsA Additional output parameters produced by the vertex shader for the first point of the line (will be interpolated and passed as input to the fragment shader)
 * @param varyingsB Additional output parameters produced by the vertex shader for the second point of the line (will be interpolated and passed as input to the fragment shader)
 * @param fb Framebuffer on which the line will be rasterized
*/
void mnkt_rasterize2DLine(Vec3_t screenCoordsA, Vec3_t screenCoordsB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb)
{
        if(shader == NULL || varyingsA == NULL || varyingsB == NULL || fb == NULL)
                return;

        // Ensure that screenCoordsA is in the valid range
        if( !mnkt_math_pointIntersectRect(screenCoordsA.x, screenCoordsA.y, 0.0f, 0.0f, fb->width - 1, fb->height - 1) )
                return;

        // Ensure that screenCoordsB is in the valid range
        if( !mnkt_math_pointIntersectRect(screenCoordsB.x, screenCoordsB.y, 0.0f, 0.0f, fb->width - 1, fb->height - 1) )
                return;

        Vec3_t* pointA = &screenCoordsA;
        Vec3_t* pointB = &screenCoordsB;

        // Invoke a specific internal line function according to the line type
        if( abs((int) (pointA->x - pointB->x)) > abs((int) (pointA->y - pointB->y)) )
        {
                // If line is more "horizontal-ish"
                
                // Invoke function ensuring that the first point parameter is the leftmost point
                if(pointA->x > pointB->x)
                {
                        mnkt_rasterize2DHorLine(pointB, pointA, shader, varyingsB, varyingsA, fb);
                } else {
                        mnkt_rasterize2DHorLine(pointA, pointB, shader, varyingsA, varyingsB, fb);
                }

        } else {
                // Otherwise if line is more "vertical-ish"

                // Invoke function ensuring that the first point parameter is the bottom-most point
                if(pointA->y > pointB->y)
                {
                        mnkt_rasterize2DVertLine(pointB, pointA, shader, varyingsB, varyingsA, fb);
                } else {
                        mnkt_rasterize2DVertLine(pointA, pointB, shader, varyingsA, varyingsB, fb);
                }
        }
}


/**
 * @function mnkt_rasterize2DHorLine
 * Rasterizes an horizontal-ish 2D line and invokes the fragment shader for each fragment produced.
 * @param pointA Screen coordinates of the leftmost point of the line
 * @param pointB Screen coordinates of the rightmost point of the line
 * @note: For internal usage only!!!
*/
void mnkt_rasterize2DHorLine(Vec3_t* pointA, Vec3_t* pointB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb)
{
        printf("rasterize2DHorLine called (pointA: %f, %f, pointB: %f, %f)!\n", pointA->x, pointA->y, pointB->x, pointB->y);

        // Compute deltas
        int32_t dx = pointB->x - pointA->x;
        int32_t dy = pointB->y - pointA->y;

        size_t y = pointA->y;
        int32_t yStep = 1;

        // Determine step to take on y axis
        if(dy < 0)
        {
                yStep = -1;
                dy *= -1;
        }

        int32_t distance = (2 * dy) - dx;

        Vec2_t fragCoords = { pointA->x, y };
        size_t fragIndex = (y * fb->width) + (size_t) pointA->x;

        for(size_t x = pointA->x; x < pointB->x; ++x)
        {
                // TODO: Interpolate varyings and vertex depth values across the line...
                
                printf("fragIndex: %lu\n", fragIndex);
                mnkt_drawFragment(&fragCoords, pointA->z, fragIndex, shader, varyingsA, fb);

                if(distance > 0)
                {
                        y += yStep;
                        distance += 2 * (dy - dx);

                        fragCoords.y += yStep;
                        fragIndex += (int32_t) (yStep * fb->width);

                } else {
                        distance += 2 * dy;
                }

                ++fragCoords.x;
                ++fragIndex;
        }
}


/**
 * @function mnkt_rasterize2DHorLine
 * Rasterizes a vertical-ish 2D line and invokes the fragment shader for each fragment produced.
 * @param pointA Screen coordinates of the bottom-most point of the line
 * @param pointB Screen coordinates of the top-most point of the line
 * @note: For internal usage only!!!
*/
void mnkt_rasterize2DVertLine(Vec3_t* pointA, Vec3_t* pointB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb)
{
        printf("rasterize2DVertLine called (pointA: %f, %f, pointB: %f, %f)!\n", pointA->x, pointA->y, pointB->x, pointB->y);
        
        // Compute deltas
        int32_t dx = pointB->x - pointA->x;
        int32_t dy = pointB->y - pointA->y;

        size_t x = pointA->x;
        int32_t xStep = 1;

        // Determine step to take on x axis
        if(dx < 0)
        {
                xStep = -1;
                dx *= -1;
        }

        int32_t distance = (2 * dx) - dy;

        Vec2_t fragCoords = { x, pointA->y };
        size_t fragIndex = ( (size_t) pointA->y * fb->width ) + x;

        for(size_t y = pointA->y; y < pointB->y; ++y)
        {
                // TODO: Interpolate varyings and vertex depth values across the line...
                
                mnkt_drawFragment(&fragCoords, pointA->z, fragIndex, shader, varyingsA, fb);

                if(distance > 0)
                {
                        x += xStep;
                        distance += 2 * (dx - dy);

                        fragCoords.x += xStep;
                        fragIndex += xStep;

                } else {
                        distance += 2 * dx;
                }

                ++fragCoords.y;
                fragIndex += fb->width;
        }
}


/**
 * @function mnkt_drawFragment
 * Computes color and depth values of a fragment and stores them into the given framebuffer.
 * @param fragCoords Coordinates of the fragment inside the frame buffer, those will be passed to the fragment shader (must be non null)
 * @param fragDepth Depth value, computed by the vertex shader, for the fragment; this will be used for depth test against fb's depth buffer
 * @param fragIndex Index of the fragment to be drawn inside the frame buffer, this will be used to access fb's color and depth buffers
 * @param shader Shader program to be used to compute the fragment color
 * @param varyings Additional parameters, outputted by the vertex shader, to be passed as input to the fragment shader
 * @param fb Frame buffer into which the fragment should be drawn
*/
void mnkt_drawFragment(const Vec2_t* fragCoords, float fragDepth, size_t fragIndex, const ShaderProgram_t* shader, const ShaderParameter_t* varyings, Framebuffer_t* fb)
{
        printf("Drawing frag at: %d, %d\n", (int) fragCoords->x, (int) fragCoords->y); // TODO: REMOVE ME!!!
        int discard = 0;

        // Perform depth test
        if(fb->depthBuffer[fragIndex] <= fragDepth)
        {
                printf("depth test failed!!!\n");
                // If current fragment's depth is greater than what is already stored in the depth buffer, then skip the fragment
                return;
        }

        // Invoke fragment shader
        Vec4_t fragColor = shader->fragmentShader(varyings, shader->uniforms, fragCoords, &discard);
        
        // Discard fragment, if necessary
        if(discard != 0)
                return;

        // TODO: Perform color blending (if requested by the shader)

        // Update framebuffer content
        fb->colorBuffer[ fragIndex * 3 ] =              mnkt_colorAsUChar(fragColor.r);
        fb->colorBuffer[ (fragIndex * 3) + 1 ] =        mnkt_colorAsUChar(fragColor.g);
        fb->colorBuffer[ (fragIndex * 3) + 2 ] =        mnkt_colorAsUChar(fragColor.b);

        fb->depthBuffer[fragIndex] = fragDepth;
}


