
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "mnktRenderer.h"


int     createResources(Framebuffer_t* fb, const size_t fbWidth, const size_t fbHeight, ShaderProgram_t* shader);
void    destroyResources(Framebuffer_t* fb, ShaderProgram_t* shader);

void    renderImage(Framebuffer_t* fb, ShaderProgram_t* shader);

Vec4_t  vertexShader(const void* vertex, ShaderParameter_t* varyings, const ShaderParameter_t* uniforms);
Vec4_t  fragmentShader(const ShaderParameter_t* varyings, const ShaderParameter_t* uniforms, const Vec2_t* fragCoords, int* discard);

void    getRandomVertices(float vertices[12]);

int     savePPMImage(const char* filename, const Framebuffer_t* fb);


int main()
{
        #define FRAMEBUFFER_WIDTH 128
        #define FRAMEBUFFER_HEIGHT 128

        Framebuffer_t fb;
        ShaderProgram_t shader;

        // Setup all resources
        if(createResources(&fb, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, &shader) != 0)
                return 0;

        // Render the scene and save image to file
        renderImage(&fb, &shader);
        savePPMImage("output.ppm", &fb);

        // Deallocate all resources
        destroyResources(&fb, &shader);
        
        return 0;
}


/**
 * @function createResources
 * Tries to set up the given frame buffer and shader program structs
 * @param fb Pointer to the frame buffer struct to be set up
 * @param fbWidth frame buffer width
 * @param fbHeight frame buffer height
 * @param shader Shader program to be set up
 * @return Zero on success, non zero on failure
*/
int createResources(Framebuffer_t* fb, const size_t fbWidth, const size_t fbHeight, ShaderProgram_t* shader)
{
        // Setup frame buffer struct, if one is given 
        if(fb != NULL)
        {
                // Allocate color buffer
                fb->colorBuffer = malloc( fbWidth * fbHeight * sizeof(unsigned char) * 3 );
                if(fb->colorBuffer == NULL)
                {
                        fprintf(stderr, "[ ERROR ] createResources() failed, failed to allocate memory for the frame buffer's color buffer!\n");
                        return 1;
                }

                // Allocate depth buffer
                fb->depthBuffer = malloc( sizeof(float) * fbWidth * fbHeight );
                if(fb->depthBuffer == NULL)
                {
                        destroyResources(fb, NULL);

                        fprintf(stderr, "[ ERROR ] createResources() failed, failed to allocate memory for the frame buffer's depth buffer!\n");
                        return 1;
                }

                fb->width = fbWidth;
                fb->height = fbWidth;
        }

        // Setup shader program struct, if one is given
        if(shader != NULL)
        {
                // Setup shader functions
                shader->vertexShader = vertexShader;
                shader->fragmentShader = fragmentShader;

                // Setup the size of a single vertex (we have 6 float values, 3 for position and 3 for color)
                shader->vertexSize = sizeof(float) * 6;
        }

        return 0;
}


/**
 * @function destroyResources
 * Deallocates memory previously allocated for the given structs
*/
void destroyResources(Framebuffer_t* fb, ShaderProgram_t* shader)
{
        // Deallocate memory for the frame buffer, if one is given 
        if(fb != NULL)
        {
                if(fb->colorBuffer != NULL)
                {
                        free(fb->colorBuffer);
                        fb->colorBuffer = NULL;
                }

                if(fb->depthBuffer != NULL)
                {
                        free(fb->depthBuffer);
                        fb->depthBuffer = NULL;
                }

                fb->width = 0;
                fb->height = 0;
        }

        // Deallocate memory for the shader program, if one is given 
        if(shader != NULL)
        {
                shader->vertexShader = NULL;
                shader->fragmentShader = NULL;

                shader->vertexSize = 0;
        }
}


/**
 * @function renderImage
 * Renders a scene into the given frame buffer
 * @param fb Frame buffer in which the scene will be rendered
 * @param shader Shader program to be used to render the scene
*/
void renderImage(Framebuffer_t* fb, ShaderProgram_t* shader)
{
        if(fb == NULL || shader == NULL)
        {
                fprintf(stderr, " [ ERROR ] renderImage() failed, frame buffer and/or shader is NULL!\n");
                return;
        }

        // Clear framebuffer content
        mnkt_framebuffer_clearColor(255, 116, 0, fb);
        mnkt_framebuffer_clearDepth(1.0f, fb);
        
        // For test purpouses let's render a lot of random primitives
        srandom(time(NULL));
        
        float vertices[] = {
        //        x       y      z                r      g       b
                -0.9f,  -0.5f,  0.0f,           1.0f,   0.0f,   0.0f,
                0.9f,   0.5f,   0.0f,           1.0f,   0.0f,   0.0f,
        };

        //#define POINT_TEST
        #define LINE_TEST
        //#define POLY_LINE_TEST
        //#define TRIANGLE_TEST
        
        // Draw 2048 random primitives
        for(size_t i = 0; i < 2048; ++i)
        {
                getRandomVertices(vertices);

                #ifdef POINT_TEST
                printf("Drawing point %lu at (%f, %f)\n", i + 1, vertices[0], vertices[1]);
                mnkt_draw2DPoint(vertices, 1, 1, shader, fb);
                
                #elif defined LINE_TEST
                printf("Drawing line %lu: (%f, %f) to (%f, %f)\n", i + 1, vertices[0], vertices[1], vertices[6], vertices[7]);
                mnkt_draw2DLine(vertices, 2, shader, fb);

                #elif defined POLY_LINE_TEST
                // TODO: Add implementation...
                //printf("Drawing poly-line %lu with vertices: (%f, %f) to (%f, %f) to (%f, %f)\n", i + 1, vertices[0], vertices[1], vertices[6], vertices[7], vertices[12], vertices[13]);
                //mnkt_draw2DPolyLine(vertices, 3, shader, fb);
                break;
                
                #else
                // TODO: Add implementation...
                //printf("Drawing triangle %lu with vertices: (%f, %f), (%f, %f), (%f, %f)\n", i + 1, vertices[0], vertices[1], vertices[6], vertices[7], vertices[12], vertices[13]);
                //mnkt_draw(vertices, 3, shader, fb);
                break;

                #endif
        }
}


/**
 * @function vertexShader
 * The shader program to be executed for each vertex
 * @param vertex The vertex data to be processed
 * @param varyings Array of values in which additional output parameters can be stored
 * @param uniforms Uniform variables
*/
Vec4_t vertexShader(const void* vertex, ShaderParameter_t* varyings, const ShaderParameter_t* uniforms)
{
        // Extract values from the vertex
        Vec3_t* inPos = (Vec3_t*) vertex;
        Vec3_t* inColor = (Vec3_t*) ( (char*) vertex + sizeof(float) * 3);

        // For now it simply returns the input it gets
        varyings[0].vec3 = *inColor;

        return (Vec4_t) { .x = inPos->x, .y = inPos->y, .z = inPos->z, .w = 1.0f };
}


/**
 * @function vertexShader
 * The shader program to be executed for each fragment
 * @param varyings Array of values in which the additional output parameters produced by the vertex shader are stored
 * @param uniforms Uniform variables
 * @param fragCoords Coordinates of the fragment to be processed, expressed in screen space
 * @param discard Flag used to indicate that this fragment must be discared
*/
Vec4_t fragmentShader(const ShaderParameter_t* varyings, const ShaderParameter_t* uniforms, const Vec2_t* fragCoords, int* discard)
{
        // For now it simply returns the input it gets
        return (Vec4_t) {
                .r = varyings[0].vec3.r,
                .g = varyings[0].vec3.g,
                .b = varyings[0].vec3.b,
                1.0f
        };
}


/**
 * @function getRandomVertices
 * Populates the elements in the given array with random data
 * Used to test the mnkt_draw* functions with arbitrary data
 * @param vertices Array of floats that must be filled with random data
*/
void getRandomVertices(float vertices[12])
{
        // First position
        vertices[0] = ( (float) rand() / (float) RAND_MAX) * (rand() % 2 == 0 ? 1 : -1);
        vertices[1] = ( (float) rand() / (float) RAND_MAX) * (rand() % 2 == 0 ? 1 : -1);
        vertices[2] = (float) rand() / (float) RAND_MAX;

        // First color
        vertices[3] = ( (float) rand() / (float) RAND_MAX);
        vertices[4] = ( (float) rand() / (float) RAND_MAX);
        vertices[5] = ( (float) rand() / (float) RAND_MAX);

        // Second position
        vertices[6] = ( (float) rand() / (float) RAND_MAX) * (rand() % 2 == 0 ? 1 : -1);
        vertices[7] = ( (float) rand() / (float) RAND_MAX) * (rand() % 2 == 0 ? 1 : -1);
        vertices[8] = (float) rand() / (float) RAND_MAX;

        // Second color
        vertices[9] = ( (float) rand() / (float) RAND_MAX);
        vertices[10] = ( (float) rand() / (float) RAND_MAX);
        vertices[11] = ( (float) rand() / (float) RAND_MAX);
}


/**
 * Saves the content of the given frame buffer on the file system as a PPM file
 * @param filename Name to be used for the PPM image file
 * @param fb Framebuffer of which content must be saved
 * @return Zero on success, non zero on failure
*/
int savePPMImage(const char* filename, const Framebuffer_t* fb)
{
        const uint32_t maxColorValue = 255;

        if(filename == NULL)
        {
                fprintf(stderr, "[ ERROR ] savePPMImage() failed, no file name is given!\n");
                return -1;
        }

        if(fb == NULL || fb->colorBuffer == NULL)
        {
                fprintf(stderr, "[ ERROR ] savePPMImage() failed, no framebuffer is given!\n");
                return -1;
        }

        FILE* imgFile = fopen(filename, "w");
        if(imgFile == NULL)
        {
                fprintf(stderr, "[ ERROR ] savePPMImage() failed, file \"%s\" could not be opened!\n", filename);
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



