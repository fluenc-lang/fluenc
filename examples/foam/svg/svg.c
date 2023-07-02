#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <float.h>

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

img_t *render(const char *filename)
{
	printf("parsing %s\n", filename);

	NSVGimage *image = nsvgParseFromFile(filename, "px", 96.0f);

	if (image == NULL)
	{
		printf("Could not open SVG image.\n");
		return NULL;
	}

	int w = (int)image->width;
	int h = (int)image->height;

	NSVGrasterizer *rast = nsvgCreateRasterizer();

	if (rast == NULL)
	{
		printf("Could not init rasterizer.\n");
		return NULL;
	}

	img_t *img = malloc(sizeof(img_t) + sizeof(buffer_t) + (w * h * 4));
	img->width = w;
	img->height = h;

	buffer_t* buffer = (buffer_t *)&(img[1]);
	buffer->size = (w * h * 4);

	img->data = buffer;

	if (img == NULL)
	{
		printf("Could not alloc image buffer.\n");
		return NULL;
	}

	printf("rasterizing image %d x %d\n", w, h);

	nsvgRasterize(rast, image, 0, 0, 1, (uint8_t*)&(buffer[1]), w, h, w * 4);

	printf("rasterized\n");

    return img;
}
