
/**
 * @file rasterizer.c
 *
 * Contains implementation of functions to rasterize primitive graphic shapes
*/

#include "rasterizer.h"

#include <stdio.h>


// Prototypes for internal functions and structs

/**
 * @struct BBox_t
 * Models a bounding box (the minimal rectangular area that contains a set of specific points)
*/
typedef struct {
        float   x;              ///< X coordinate of the top left corner of the box
        float   y;              ///< Y coordinate of the top left corner of the box
        float   width;          ///< Width of the box expressed
        float   height;         ///< Height of the box expressed
} BBox_t;


static void     mnkt_rasterizeHorLine(Vec3_t* pointA, Vec3_t* pointB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb);
static void     mnkt_rasterizeVertLine(Vec3_t* pointA, Vec3_t* pointB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb);

static BBox_t   mnkt_getScreenBBox(Vec3_t* points, size_t pointsNum);

static void     mnkt_drawFragment(const Vec2_t* fragCoords, float fragDepth, size_t fragIndex, const ShaderProgram_t* shader, const ShaderParameter_t* varyings, Framebuffer_t* fb);


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
void mnkt_rasterizePoint(Vec3_t screenCoords, const size_t pointSize, const ShaderProgram_t* shader, const ShaderParameter_t varyings[MAX_VARYING_PARAMS], Framebuffer_t* fb)
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
 * @function mnkt_rasterizeLine
 * Rasterizes a 2D line and invokes the fragment shader for each fragment produced.
 * @param screenCoords Array of vectors which defines the screen coordinates of the two extreme points of the line to be rasterized
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader for the two extreme points of the line
 *      (those will be interpolated across the line and passed as input to the fragment shader)
 * @param fb Framebuffer on which the line will be rasterized
*/
void mnkt_rasterizeLine(Vec3_t screenCoords[2], const ShaderProgram_t* shader, const ShaderParameter_t varyings[2][MAX_VARYING_PARAMS], Framebuffer_t* fb)
{
        if(shader == NULL || varyings == NULL || fb == NULL)
                return;

        // Ensure that all points are in the valid range
        for(uint32_t i = 0; i < 2; ++i)
        {
                if( !mnkt_math_pointIntersectRect(screenCoords[i].x, screenCoords[i].y, 0.0f, 0.0f, fb->width - 1, fb->height - 1) )
                        return;
        }

        Vec3_t* pointA = &screenCoords[0];
        Vec3_t* pointB = &screenCoords[1];

        // Invoke a specific internal line function according to the line type
        if( abs((int) (pointA->x - pointB->x)) > abs((int) (pointA->y - pointB->y)) )
        {
                // If line is more "horizontal-ish"
                
                // Invoke function ensuring that the first point parameter is the leftmost point
                if(pointA->x > pointB->x)
                {
                        mnkt_rasterizeHorLine(pointB, pointA, shader, varyings[1], varyings[0], fb);
                } else {
                        mnkt_rasterizeHorLine(pointA, pointB, shader, varyings[0], varyings[1], fb);
                }

        } else {
                // Otherwise if line is more "vertical-ish"

                // Invoke function ensuring that the first point parameter is the bottom-most point
                if(pointA->y > pointB->y)
                {
                        mnkt_rasterizeVertLine(pointB, pointA, shader, varyings[1], varyings[0], fb);
                } else {
                        mnkt_rasterizeVertLine(pointA, pointB, shader, varyings[0], varyings[1], fb);
                }
        }
}


/**
 * @function mnkt_rasterizeHorLine
 * Rasterizes an horizontal-ish 2D line and invokes the fragment shader for each fragment produced.
 * @param pointA Screen coordinates of the leftmost point of the line
 * @param pointB Screen coordinates of the rightmost point of the line
 * @note: For internal usage only!!!
*/
static void mnkt_rasterizeHorLine(Vec3_t* pointA, Vec3_t* pointB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb)
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
static void mnkt_rasterizeVertLine(Vec3_t* pointA, Vec3_t* pointB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb)
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


/*
 * @function mnkt_rasterizeTriangle
 * Rasterizes a 2D triangle and invokes the fragment shader for each fragment produced.
 * @param screenCoords Array of vectors which defines the screen coordinates of the vertices of the triangle to be rasterized
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader for the vertices of the triangle
 *      (those will be interpolated across the triangle surface and passed as input to the fragment shader)
 * @param fb Framebuffer on which the line will be rasterized
*/
void mnkt_rasterizeTriangle(Vec3_t screenCoords[3], const ShaderProgram_t* shader, const ShaderParameter_t varyings[3][MAX_VARYING_PARAMS], Framebuffer_t* fb)
{
        if(shader == NULL || varyings == NULL || fb == NULL)
                return;

        // Ensure that all vertices are in the valid range
        for(uint32_t i = 0; i < 3; ++i)
        {
                if( !mnkt_math_pointIntersectRect(screenCoords[i].x, screenCoords[i].y, 0.0f, 0.0f, fb->width - 1, fb->height - 1) )
                        return;
        }

        // Dummy implementation that draws the triangle perimeter =============
        /*mnkt_rasterizeLine( screenCoords, shader, varyings, fb );
        mnkt_rasterizeLine( &(screenCoords[1]), shader, varyings, fb );

        Vec3_t coords[2] = { screenCoords[0], screenCoords[2] };
        mnkt_rasterizeLine(coords, shader, varyings, fb);
        // ====================================================================
        */

        // Compute triangle's bounding box
        BBox_t bBox = mnkt_getScreenBBox(screenCoords, 3);

        // Clamp the bounding box onto the framebuffer
        bBox.x = mnkt_math_clamp(bBox.x, 0, fb->width - 1);
        bBox.y = mnkt_math_clamp(bBox.y, 0, fb->height - 1);

        bBox.width = mnkt_math_clamp(bBox.width, 0, fb->width - 1 - bBox.x);
        bBox.height = mnkt_math_clamp(bBox.height, 0, fb->height - 1 - bBox.y);

        // Dummy implementation that draws the triangle bounding box =============
        size_t fragIndex = ( (size_t) bBox.y * fb->width) + (size_t) bBox.x;

        for(size_t y = 0; y < (size_t) bBox.height; ++y)
        {
                for(size_t x = 0; x < (size_t) bBox.width; ++x)
                {
                        fb->colorBuffer[ fragIndex * 3 ] = 255;
                        fb->colorBuffer[ (fragIndex * 3) + 1 ] = 0;
                        fb->colorBuffer[ (fragIndex * 3) + 2 ] = 0;

                        ++fragIndex;
                }

                fragIndex += fb->width;                 // Go to line below
                fragIndex -= (size_t) bBox.width;       // Reset to start pf BBox
        }
        // =======================================================================
}


/**
 * @function mnkt_getScreenBBox
 * @param points Array of points, expressed in screen coordinates, for which the bounding box must be computed
 * @param pointsNum The number of points given in the array
 * @return The bounding box that contains all the points given as parameters
*/
static BBox_t mnkt_getScreenBBox(Vec3_t* points, size_t pointsNum)
{
        if(points == NULL || pointsNum == 0)
        {
                return (BBox_t) {
                        .x = 0,
                        .y = 0,
                        .width = 0,
                        .height = 0,
                };
        }
        
        BBox_t box = {
                .x      = points[0].x,
                .y      = points[0].y,
                .width  = points[0].x,
                .height = points[0].y,
        };
        
        // Search min and max values of x and y
        for(size_t i = 1; i < pointsNum; ++i)
        {
                if(points[i].x < box.x)
                {
                        box.x = points[i].x;

                } else if(points[i].x > box.x)
                {
                        box.width = points[i].x;
                }

                if(points[i].y < box.y)
                {
                        box.y = points[i].y;

                } else if(points[i].y > box.y)
                {
                        box.height = points[i].y;
                }
        }

        box.width -= box.x;     // Compute box width (currently in such variable is stored the max x value found)
        box.height -= box.y;    // Compute box height (currently in such variable is stored the max y value found)
        
        return box;
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
static void mnkt_drawFragment(const Vec2_t* fragCoords, float fragDepth, size_t fragIndex, const ShaderProgram_t* shader, const ShaderParameter_t* varyings, Framebuffer_t* fb)
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


