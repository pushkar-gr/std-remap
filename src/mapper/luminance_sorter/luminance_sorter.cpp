#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "luminance_sorter.hpp"

struct Pixel {
  unsigned char r, g, b;
  int x, y;
  float luminance;

  Pixel(unsigned char r, unsigned char g, unsigned char b, int x, int y)
      : r(r), g(g), b(b), x(x), y(y) {
    this->luminance = this->get_luminance();
  }

  // calculate luminance for pixel
  inline float get_luminance() {
    return 0.2126f * this->r + 0.7152f * this->g + 0.0722f * this->b;
  }
  // compare 2 pixels
  bool operator<(const Pixel &other) const {
    return this->luminance < other.luminance;
  }
};

namespace Mapper {
void luminance_remap(unsigned char **result_img, const unsigned char *src_img,
                     const unsigned char *target_img, int w, int h) {
  // flatten source and target images
  long num_pixles = (long)w * h;
  std::vector<Pixel> source_pixels;
  source_pixels.reserve(num_pixles);
  std::vector<Pixel> target_pixels;
  target_pixels.reserve(num_pixles);

  std::cout << "Extracting and analyzing pixels..." << std::endl;
  for (long i = 0; i < num_pixles; i++) {
    source_pixels.emplace_back(src_img[i * 3 + 0], src_img[i * 3 + 1],
                               src_img[i * 3 + 2], 0, 0);

    target_pixels.emplace_back(target_img[i * 3 + 0], target_img[i * 3 + 1],
                               target_img[i * 3 + 2], i % w, i / w);
  }

  std::cout << "Sorting pixel list by luminance..." << std::endl;
  std::sort(source_pixels.begin(), source_pixels.end());
  std::sort(target_pixels.begin(), target_pixels.end());

  // build final image
  std::cout << "Building the final image..." << std::endl;
  for (long i = 0; i < num_pixles; i++) {
    long index = (long)(target_pixels[i].y * w + target_pixels[i].x) * 3;
    *((*result_img) + index + 0) = source_pixels[i].r;
    *((*result_img) + index + 1) = source_pixels[i].g;
    *((*result_img) + index + 2) = source_pixels[i].b;
  }
}
} // namespace Mapper
