
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


// TODO: MOVE THIS TO mnkt_renderer
typedef struct {
        uint32_t        width;                  ///< Width of the image
        uint32_t        height;                 ///< Height of the image
        int*            pixels;                 ///< The pixels of the image packed as 32 bit integers in RGBA encoding
} Image_t;


int savePPMImage(const char* filename, const Image_t* image);


int main()
{
        Image_t img;
        img.width = 128;
        img.height = 128;

        // Allocate memory to store image pixels
        img.pixels = malloc(sizeof(int) * img.width * img.height);
        if(img.pixels == NULL)
        {
                fprintf(stderr, "ERROR: failed to allocate memory to store image pixels!\n");
                return 0;
        }

        // Fill the image with the red color
        for(uint32_t y = 0; y < img.height; ++y)
        {
                for(uint32_t x = 0; x < img.width; ++x)
                        img.pixels[ (y * img.width) + x ] = 255 << 24;
        }

        // Save image to file
        savePPMImage("image.ppm", &img);

        free(img.pixels);
        return 0;
}


/**
 * Saves the given image on file system as a PPM file
 * @param filename Name to be used for the PPM image file
 * @param image Image of which content must be saved
 * @return Zero on success, non zero on failure
*/
int savePPMImage(const char* filename, const Image_t* image)
{
        const uint32_t maxColorValue = 255;

        if(filename == NULL)
        {
                fprintf(stderr, "ERROR: savePPMImage() failed, no file name is given!\n");
                return -1;
        }

        if(image == NULL || image->pixels == NULL)
        {
                fprintf(stderr, "ERROR: savePPMImage() failed, no image is given!\n");
                return -1;
        }

        FILE* imgFile = fopen(filename, "w");
        if(imgFile == NULL)
        {
                fprintf(stderr, "ERROR: savePPMImage() failed, file \"%s\" could not be opened!\n", filename);
                return -1;
        }

        fprintf(imgFile, "P3\n");
        fprintf(imgFile, "%u %u %u\n", image->width, image->height, maxColorValue);

        size_t currIndex = 0;
        for(uint32_t y = 0; y < image->height; ++y)
        {
                for(uint32_t x = 0; x < image->width; ++x)
                {
                        unsigned char r = image->pixels[currIndex] >> 24;
                        unsigned char g = image->pixels[currIndex] >> 16;
                        unsigned char b = image->pixels[currIndex] >> 8;

                        fprintf(imgFile, "%d %d %d ", r, g, b);

                        ++currIndex;
                }

                fputc('\n', imgFile);
        }

        fclose(imgFile);
        return 0;
}



