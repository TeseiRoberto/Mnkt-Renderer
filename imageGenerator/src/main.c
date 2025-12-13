
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "mnktRenderer.h"


int savePPMImage(const char* filename, const Framebuffer_t* fb);


int main()
{
        // Setup a framebuffer
        Framebuffer_t fb;

        fb.width = 128;
        fb.height = 128;

        // Allocate memory for the framebuffer's color buffer
        fb.colorBuffer = malloc( fb.width * fb.height * 3 * sizeof(unsigned char) );
        if(fb.colorBuffer == NULL)
        {
                fprintf(stderr, "ERROR: failed to allocate memory for the framebuffer's color buffer!\n");
                return 0;
        }

        // Allocate memory for the framebuffer's depth buffer
        fb.depthBuffer = malloc( fb.width * fb.height * sizeof(float) );
        if(fb.depthBuffer == NULL)
        {
                free(fb.colorBuffer);
                fprintf(stderr, "ERROR: failed to allocate memory for the framebuffer's color buffer!\n");
                return 0;
        }

        // Now let's render a rectangle!!!!

        // Define the vertices of the triangle (clockwise order)
        float vertices[] = {
                -0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f,
                0.5f, -0.5f, 0.0f
        };

        // Clear framebuffer content
        mnkt_framebufferClearColor(255, 116, 0, &fb);
        mnkt_framebufferClearDepth(0.0f, &fb);

        mnkt_draw(vertices, sizeof(vertices) / sizeof(vertices[0]), &fb);

        // Save framebuffer's content to file
        savePPMImage("image.ppm", &fb);

        // Deallocate framebuffer's memory
        free(fb.colorBuffer);
        free(fb.depthBuffer);

        return 0;
}


/**
 * Saves the given image on file system as a PPM file
 * @param filename Name to be used for the PPM image file
 * @param fb Framebuffer of which content must be saved
 * @return Zero on success, non zero on failure
*/
int savePPMImage(const char* filename, const Framebuffer_t* fb)
{
        const uint32_t maxColorValue = 255;

        if(filename == NULL)
        {
                fprintf(stderr, "ERROR: savePPMImage() failed, no file name is given!\n");
                return -1;
        }

        if(fb == NULL || fb->colorBuffer == NULL)
        {
                fprintf(stderr, "ERROR: savePPMImage() failed, no framebuffer is given!\n");
                return -1;
        }

        FILE* imgFile = fopen(filename, "w");
        if(imgFile == NULL)
        {
                fprintf(stderr, "ERROR: savePPMImage() failed, file \"%s\" could not be opened!\n", filename);
                return -1;
        }

        fprintf(imgFile, "P3\n");
        fprintf(imgFile, "%u %u %u\n", fb->width, fb->height, maxColorValue);

        size_t currIndex = 0;
        for(uint32_t y = 0; y < fb->height; ++y)
        {
                for(uint32_t x = 0; x < fb->width; ++x)
                {
                        unsigned char r = fb->colorBuffer[currIndex];
                        unsigned char g = fb->colorBuffer[currIndex + 1];
                        unsigned char b = fb->colorBuffer[currIndex + 2];

                        fprintf(imgFile, "%d %d %d ", r, g, b);
                        currIndex += 3;
                }

                fputc('\n', imgFile);
        }

        fclose(imgFile);
        return 0;
}



