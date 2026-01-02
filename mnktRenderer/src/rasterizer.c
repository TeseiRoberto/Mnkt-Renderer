
/**
 * @file rasterizer.c
 *
 * Contains implementation of functions to rasterize primitive graphic shapes
*/

#include "rasterizer.h"

#include <stdio.h>


/**
 * Rasterizes a 2D point and invokes the fragment shader for each fragment produced.
 * @param screenCoords Vector which defines the screen coordinates of the center of the point to be rasterized
 * @param pointSize Number of pixels that each side of the point takes up.
 *      Use 0 to rasterize a single pixel.
 * @param shader Shader to be used to determine the color of each fragment produced
 * @param varyings Additional output parameters produced by the vertex shader (will be interpolated and passed as input to the fragment shader)
 * @param fb Framebuffer on which the point will be rasterized
*/
void mnkt_rasterize2DPoint(const Vec3_t* screenCoords, const size_t pointSize, const ShaderProgram_t* shader, const ShaderParameter_t* varyings, Framebuffer_t* fb)
{
        if(screenCoords == NULL || shader == NULL || varyings == NULL || fb == NULL)
                return;

        // Determine area of the framebuffer on which point will be rasterized
        Vec2_t startCoords;
        startCoords.x = mnkt_clamp(screenCoords->x - pointSize, 0, fb->width - 1);
        startCoords.y = mnkt_clamp(screenCoords->y - pointSize, 0, fb->height - 1);

        Vec2_t endCoords;
        endCoords.x = mnkt_clamp(screenCoords->x + pointSize, 0, fb->width - 1);
        endCoords.y = mnkt_clamp(screenCoords->y + pointSize, 0, fb->height - 1);

        Vec2_t fragCoords;

        for(size_t y = startCoords.y; y <= endCoords.y; ++y)
        {
                fragCoords.y = y;
                size_t fragIndex = (y * fb->width) + (size_t) startCoords.x;

                for(size_t x = startCoords.x; x <= endCoords.x; ++x, ++fragIndex)
                {
                        fragCoords.x = x;
                        int discard = 0;

                        // Perform depth test
                        if(fb->depthBuffer[fragIndex] <= screenCoords->z)
                        {
                                // If current point's depth is greater than what is already stored in the depth buffer, then skip the fragment
                                continue;
                        }

                        // TODO: Interpolate varyings (Is it necessary? we have a single vertex for a point...)

                        // Invoke fragment shader
                        Vec4_t fragColor = shader->fragmentShader(varyings, shader->uniforms, &fragCoords, &discard);
                        
                        // Discard fragment, if necessary
                        if(discard != 0)
                                continue;

                        // TODO: Perform color blending

                        // Update framebuffer content
                        fb->colorBuffer[ fragIndex * 3 ] =              mnkt_colorAsUChar(fragColor.r);
                        fb->colorBuffer[ (fragIndex * 3) + 1 ] =        mnkt_colorAsUChar(fragColor.g);
                        fb->colorBuffer[ (fragIndex * 3) + 2 ] =        mnkt_colorAsUChar(fragColor.b);

                        fb->depthBuffer[fragIndex] = screenCoords->z;
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
void mnkt_rasterize2DLine(const Vec3_t* screenCoordsA, const Vec3_t* screenCoordsB, const ShaderProgram_t* shader, const ShaderParameter_t* varyingsA, const ShaderParameter_t* varyingsB, Framebuffer_t* fb)
{
        if(shader == NULL || fb == NULL)
                return;

        if(screenCoordsA == NULL || screenCoordsB == NULL || varyingsA == NULL || varyingsB == NULL)
                return;

        // TODO: Add implementation...
}


