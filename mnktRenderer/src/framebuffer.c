
/*
 * @file framebuffer.c
 *
 * Contains implementation of the framebuffer API
*/


#include "framebuffer.h"


/**
 * @function mnkt_framebuffer_clearColor
 * Sets the color of all pixels inside the framebuffer
 * @param r Red value to be used for the clear color
 * @param g Green value to be used for the clear color
 * @param b Blue value to be used for the clear color
 * @param framebuffer Framebuffer of which the color buffer must be cleared
*/
void mnkt_framebuffer_clearColor(const unsigned char r, const unsigned char g, const unsigned char b, Framebuffer_t* fb)
{
        if(fb == NULL || fb->colorBuffer == NULL)
                return;

        size_t currIndex = 0;
        for(uint32_t i = 0; i < fb->width * fb->height; ++i)
        {
                fb->colorBuffer[currIndex] = r;
                fb->colorBuffer[currIndex + 1] = g;
                fb->colorBuffer[currIndex + 2] = b;

                currIndex += 3;
        }
}


/**
 * @function mnkt_framebuffer_clearDepth
 * Sets the depth values of all pixels inside the framebuffer
 * @param depth Depth value to be used for all pixels
 * @param framebuffer Framebuffer of which the depth buffer must be cleared
*/
void mnkt_framebuffer_clearDepth(float depth, Framebuffer_t* fb)
{
        if(fb == NULL || fb->depthBuffer == NULL)
                return;

        for(uint32_t i = 0; i < fb->width * fb->height; ++i)
                fb->depthBuffer[i] = depth;
}


