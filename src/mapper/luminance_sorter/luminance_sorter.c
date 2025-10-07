#include <stdio.h>
#include <stdlib.h>

#include "luminance_sorter.h"

typedef struct {
  unsigned char r, g, b;
  int x, y;
  float luminance;
} Pixel;

// calculate luminance for pixel
static inline float get_luminance(unsigned char r, unsigned char g, unsigned char b);
// compare 2 pixels
static inline int compare_pixel(const void *a, const void *b);

void luminance_remap(unsigned char **result_img, const unsigned char *src_img,
                     const unsigned char *trgt_img, int w, int h) {
  // flatten source and target images
  long num_pixles = (long)w * h;
  Pixel *source_pixels = (Pixel *)malloc(sizeof(Pixel) * num_pixles);
  Pixel *target_pixels = (Pixel *)malloc(sizeof(Pixel) * num_pixles);
  if (!source_pixels || !target_pixels) {
    fprintf(stderr, "Error: Failed to allocate memory for pixel array\n");
    exit(EXIT_FAILURE);
  }

  printf("Extracting and analyzing pixels...\n");
  for (long i = 0; i < num_pixles; i++) {
    source_pixels[i].r = src_img[i * 3 + 0];
    source_pixels[i].g = src_img[i * 3 + 1];
    source_pixels[i].b = src_img[i * 3 + 2];
    source_pixels[i].luminance = get_luminance(
        source_pixels[i].r, source_pixels[i].g, source_pixels[i].b);

    target_pixels[i].x = i % w;
    target_pixels[i].y = i / w;
    target_pixels[i].luminance = get_luminance(
        trgt_img[i * 3 + 0], trgt_img[i * 3 + 1], trgt_img[i * 3 + 2]);
  }

  printf("Sorting pixel list by luminance...\n");
  qsort(source_pixels, num_pixles, sizeof(Pixel), compare_pixel);
  qsort(target_pixels, num_pixles, sizeof(Pixel), compare_pixel);

  // build final image
  printf("Building the final image...\n");
  for (long i = 0; i < num_pixles; i++) {
    long index = (long)(target_pixels[i].y * w + target_pixels[i].x) * 3;
    *((*result_img) + index + 0) = source_pixels[i].r;
    *((*result_img) + index + 1) = source_pixels[i].g;
    *((*result_img) + index + 2) = source_pixels[i].b;
  }
  free(source_pixels);
  free(target_pixels);
}

static inline float get_luminance(unsigned char r, unsigned char g, unsigned char b) {
  return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

static inline int compare_pixel(const void *a, const void *b) {
  Pixel *p1 = (Pixel *)a;
  Pixel *p2 = (Pixel *)b;
  if (p1->luminance < p2->luminance)
    return -1;
  if (p1->luminance > p2->luminance)
    return 1;
  return 0;
}
