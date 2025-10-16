#pragma once

#include <vector>

struct Pixel {
  unsigned char r, g, b;
  int x, y;
  std::vector<int> pos_x;
  std::vector<int> pos_y;
  float avg_x, avg_y;
  float dot_product;

  Pixel(unsigned char r, unsigned char g, unsigned char b, int x, int y,
        int l_count)
      : r(r), g(g), b(b), x(x), y(y), pos_x(), pos_y(), avg_x(0), avg_y(0),
        dot_product(0) {
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
