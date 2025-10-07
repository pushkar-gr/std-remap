#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "swd_sorter.h"

typedef struct {
  unsigned char r, g, b;
  int x, y;
  int *pos_x, *pos_y;
  float dot_product;
} Pixel;

// get dot product of [r, g, b] and random vector
static inline float get_dot_product(unsigned char r, unsigned char g, unsigned char b,
                             float *vector);
// compare 2 pixels
static inline int compare_pixel(const void *a, const void *b);
// generate random float between -1.0 to +1.0
static inline double random_double();
// generate random unit vector
static void random_unit_vector(float *vector);

void swd_remap(unsigned char **result_img, const unsigned char *src_img,
               const unsigned char *trgt_img, int w, int h, int L) {
  // flatten source and target images
  long num_pixles = (long)w * h;
  Pixel *source_pixels = (Pixel *)malloc(sizeof(Pixel) * num_pixles);
  Pixel *target_pixels = (Pixel *)malloc(sizeof(Pixel) * num_pixles);
  if (!source_pixels || !target_pixels) {
    fprintf(stderr, "Error: Failed to allocate memory for pixel array\n");
    exit(EXIT_FAILURE);
  }

  printf("Extracting and analyzing pixels...\n");
  // set seed
  srand(time(NULL));

  float *unit_vector = (float *)malloc(sizeof(float) * 3);
  if (!unit_vector) {
    fprintf(stderr, "Error: Failed to allocate memory for random unit array\n");
    exit(EXIT_FAILURE);
  }
  random_unit_vector(unit_vector);

  // first run with data filling
  for (long i = 0; i < num_pixles; i++) {
    source_pixels[i].pos_x = (int *)malloc(sizeof(int) * L);
    source_pixels[i].pos_y = (int *)malloc(sizeof(int) * L);

    source_pixels[i].r = src_img[i * 3 + 0];
    source_pixels[i].g = src_img[i * 3 + 1];
    source_pixels[i].b = src_img[i * 3 + 2];
    source_pixels[i].dot_product =
        get_dot_product(source_pixels[i].r, source_pixels[i].g,
                        source_pixels[i].b, unit_vector);

    target_pixels[i].x = i % w;
    target_pixels[i].y = i / w;

    target_pixels[i].r = trgt_img[i * 3 + 0];
    target_pixels[i].g = trgt_img[i * 3 + 1];
    target_pixels[i].b = trgt_img[i * 3 + 2];
    target_pixels[i].dot_product =
        get_dot_product(target_pixels[i].r, target_pixels[i].g,
                        target_pixels[i].b, unit_vector);
  }
  // sort pixels after calculating dot_product
  printf("Sorting pixel list by dot product for L = 1...\n");
  qsort(source_pixels, num_pixles, sizeof(Pixel), compare_pixel);
  qsort(target_pixels, num_pixles, sizeof(Pixel), compare_pixel);

  int initial_l = L;

  // --L > 0 because 1 run already done
  while (--L > 0) {
    // generate random unit vector
    random_unit_vector(unit_vector);
    for (long i = 0; i < num_pixles; i++) {
      // fill x and y for source from previous run
      *(source_pixels[i].pos_x + initial_l - L - 1) = target_pixels[i].x;
      *(source_pixels[i].pos_y + initial_l - L - 1) = target_pixels[i].y;

      // find new dot products
      source_pixels[i].dot_product =
          get_dot_product(source_pixels[i].r, source_pixels[i].g,
                          source_pixels[i].b, unit_vector);

      target_pixels[i].dot_product =
          get_dot_product(target_pixels[i].r, target_pixels[i].g,
                          target_pixels[i].b, unit_vector);
    }
    if (L % 10 == 0) {
      printf("Sorting pixel list by dot product for L = %d...\n", initial_l - L);
    }
    qsort(source_pixels, num_pixles, sizeof(Pixel), compare_pixel);
    qsort(target_pixels, num_pixles, sizeof(Pixel), compare_pixel);
  }
  free(unit_vector);

  // after L iterations, calculate average position for x and y
  for (long i = 0; i < num_pixles; i++) {
    // fill x and y for source from previous run
    *(source_pixels[i].pos_x + initial_l - L - 1) = target_pixels[i].x;
    *(source_pixels[i].pos_y + initial_l - L - 1) = target_pixels[i].y;

    // calculate average
    long x = 0, y = 0;
    for (int j = 0; j < initial_l; j++) {
      x += *(source_pixels[i].pos_x + j);
      y += *(source_pixels[i].pos_y + j);
    }
    source_pixels[i].x = x / initial_l;
    source_pixels[i].y = y / initial_l;

    free(source_pixels[i].pos_x);
    free(source_pixels[i].pos_y);
  }

  // build final image
  printf("Building the final image...\n");

  char *occupied = (char *)calloc(num_pixles, sizeof(char));
  for (int i = 0; i < num_pixles; i++) {
    occupied[i] = 0;
  }
  if (!occupied) {
    fprintf(stderr, "Error: Failed to allocate memory for occupied array\n");
    exit(EXIT_FAILURE);
  }
  for (long i = 0; i < num_pixles; i++) {
    long index = (long)(source_pixels[i].y * w + source_pixels[i].x);
    while (occupied[index]) {
      index = (index + 1) % num_pixles;
    }
    occupied[index] = 1;
    long final_pos = index * 3;

    *((*result_img) + final_pos + 0) = source_pixels[i].r;
    *((*result_img) + final_pos + 1) = source_pixels[i].g;
    *((*result_img) + final_pos + 2) = source_pixels[i].b;
  }
  free(occupied);
  free(source_pixels);
  free(target_pixels);
}

static inline float get_dot_product(unsigned char r, unsigned char g, unsigned char b,
                             float *vector) {
  return vector[0] * r + vector[1] * g + vector[2] * b;
}

static inline int compare_pixel(const void *a, const void *b) {
  Pixel *p1 = (Pixel *)a;
  Pixel *p2 = (Pixel *)b;
  if (p1->dot_product < p2->dot_product)
    return -1;
  if (p1->dot_product > p2->dot_product)
    return 1;
  return 0;
}

static inline double random_double() {
  return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

static void random_unit_vector(float *vector) {
  double x = random_double();
  double y = random_double();
  double z = random_double();
  double magnitude = sqrt(x * x + y * y + z * z);

  if (magnitude == 0) {
    random_unit_vector(vector);
    return;
  }
  *(vector + 0) = x / magnitude;
  *(vector + 1) = y / magnitude;
  *(vector + 2) = z / magnitude;
}
