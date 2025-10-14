#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "swd_sorter.hpp"

struct Pixel {
  unsigned char r, g, b;
  int x, y;
  std::vector<int> pos_x;
  std::vector<int> pos_y;
  float dot_product;

  Pixel(unsigned char r, unsigned char g, unsigned char b, int x, int y,
        int l_count)
      : r(r), g(g), b(b), x(x), y(y), pos_x(), pos_y(), dot_product(0) {
    pos_x.reserve(l_count);
    pos_y.reserve(l_count);
  }
  // get dot product of [r, g, b] and random vector
  inline float get_dot_product(std::vector<float> &unit_vector) {
    return unit_vector[0] * this->r + unit_vector[1] * this->g +
           unit_vector[2] * this->b;
  }
  // compare 2 pixels
  bool operator<(const Pixel &other) const {
    return this->dot_product < other.dot_product;
  }
};

// generate random float between -1.0 to +1.0
static inline double random_double();
// generate random unit vector
static void random_unit_vector(std::vector<float> &unit_vector);

namespace SwdSorter {

void swd_remap(unsigned char **result_img, const unsigned char *src_img,
               const unsigned char *target_img, int w, int h, int L,
               const char *placement) {
  // flatten source and target images
  long num_pixles = (long)w * h;
  std::vector<Pixel> source_pixels;
  source_pixels.reserve(num_pixles);
  std::vector<Pixel> target_pixels;
  target_pixels.reserve(num_pixles);

  std::cout << "Extracting and analyzing pixels..." << std::endl;
  // set seed
  srand(time(NULL));

  std::vector<float> unit_vector(3);
  random_unit_vector(unit_vector);

  // first run with data filling
  for (long i = 0; i < num_pixles; i++) {
    source_pixels.emplace_back(src_img[i * 3 + 0], src_img[i * 3 + 1],
                               src_img[i * 3 + 2], 0, 0, L);
    source_pixels[i].dot_product =
        source_pixels[i].get_dot_product(unit_vector);

    target_pixels.emplace_back(target_img[i * 3 + 0], target_img[i * 3 + 1],
                               target_img[i * 3 + 2], i % w, i / w, L);
    target_pixels[i].dot_product =
        target_pixels[i].get_dot_product(unit_vector);
  }
  // sort pixels after calculating dot_product
  std::cout << "Sorting pixel list by luminance..." << std::endl;
  std::sort(source_pixels.begin(), source_pixels.end());
  std::sort(target_pixels.begin(), target_pixels.end());

  for (int l = 1; l < L; l++) {
    // generate random unit vector
    random_unit_vector(unit_vector);
    for (long i = 0; i < num_pixles; i++) {
      // fill x and y for source from previous run
      source_pixels[i].pos_x.push_back(target_pixels[i].x);
      source_pixels[i].pos_y.push_back(target_pixels[i].y);

      // find new dot products
      source_pixels[i].dot_product =
          source_pixels[i].get_dot_product(unit_vector);
      target_pixels[i].dot_product =
          target_pixels[i].get_dot_product(unit_vector);
    }
    if (l % 10 == 0) {
      std::cout << "Sorting pixel list by dot product for L = " << l
                << std::endl;
    }
    std::sort(source_pixels.begin(), source_pixels.end());
    std::sort(target_pixels.begin(), target_pixels.end());
  }

  // after L iterations, calculate average position for x and y
  for (long i = 0; i < num_pixles; i++) {
    // fill x and y for source from previous run
    source_pixels[i].pos_x.push_back(target_pixels[i].x);
    source_pixels[i].pos_y.push_back(target_pixels[i].y);

    // calculate average
    long x = 0, y = 0;
    for (int l = 0; l < L; l++) {
      x += source_pixels[i].pos_x[l];
      y += source_pixels[i].pos_y[l];
    }
    source_pixels[i].x = std::clamp(x / L, 0l, num_pixles);
    source_pixels[i].y = std::clamp(y / L, 0l, num_pixles);
  }

  // build final image
  std::cout << "Building the final image..." << std::endl;
  std::vector<bool> occupied_pixels(num_pixles, false);

  for (long i = 0; i < num_pixles; i++) {
    // calculate best position for the pixel
    long best_target_idx = -1;
    float min_distance_sq = -1;

    for (long j = 0; j < num_pixles; j++) {
      if (!occupied_pixels[j]) {
        long tx = j % w;
        long ty = j / 2;

        float distance_sq =
            pow(source_pixels[i].x - tx, 2) + pow(source_pixels[i].y - ty, 2);

        if (best_target_idx == -1 || distance_sq < min_distance_sq) {
          min_distance_sq = distance_sq;
          best_target_idx = j;
        }
      }
    }
    if (best_target_idx != -1) {
      long final_pos = best_target_idx * 3;

      *((*result_img) + final_pos + 0) = source_pixels[i].r;
      *((*result_img) + final_pos + 1) = source_pixels[i].g;
      *((*result_img) + final_pos + 2) = source_pixels[i].b;
      occupied_pixels[best_target_idx] = true;
    }
  }
}
} // namespace SwdSorter

static inline double random_double() {
  return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

static void random_unit_vector(std::vector<float> &unit_vector) {
  double x = random_double();
  double y = random_double();
  double z = random_double();
  double magnitude = sqrt(x * x + y * y + z * z);

  if (magnitude == 0) {
    random_unit_vector(unit_vector);
    return;
  }
  unit_vector[0] = x / magnitude;
  unit_vector[1] = y / magnitude;
  unit_vector[2] = z / magnitude;
}
