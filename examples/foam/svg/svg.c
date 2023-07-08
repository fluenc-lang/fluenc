#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#define NANOSVG_IMPLEMENTATION
#define NANOSVGRAST_IMPLEMENTATION

#include "nanosvg.h"
#include "nanosvgrast.h"

typedef struct buffer
{
	size_t size;
} buffer_t;

typedef struct img
{
    uint32_t width;
	uint32_t height;
    buffer_t* data;
} img_t;

img_t *render(const char *filename, int width, int height)
{
	printf("parsing %s\n", filename);

	NSVGimage *image = nsvgParseFromFile(filename, "px", 96.0f);

	if (image == NULL)
	{
		printf("Could not open SVG image.\n");
		return NULL;
	}

	NSVGrasterizer *rast = nsvgCreateRasterizer();

	if (rast == NULL)
	{
		printf("Could not init rasterizer.\n");
		return NULL;
	}

	img_t *img = malloc(sizeof(img_t) + sizeof(buffer_t) + (width * height * 4));
	img->width = width;
	img->height = height;

	buffer_t* buffer = (buffer_t *)&(img[1]);
	buffer->size = (width * height * 4);

	img->data = buffer;

	if (img == NULL)
	{
		printf("Could not alloc image buffer.\n");
		return NULL;
	}

	printf("rasterizing image %d x %d\n", width, height);

	float scale = fmin(width / image->width, height / image->height);

	nsvgRasterize(rast, image, 0, 0, scale, (uint8_t*)&(buffer[1]), width, height, width * 4);

	printf("rasterized\n");

    return img;
}
