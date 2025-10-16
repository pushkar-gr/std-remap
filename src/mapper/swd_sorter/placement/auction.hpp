#pragma once

#include <vector>

#include "../pixel.hpp"

namespace Placement {
class Auction {
public:
  static std::vector<long> assign(std::vector<Pixel> &pixels, int w, int h) {
    Auction instance(pixels, w, h);
    instance.run_auction();
    return instance.assigned;
  }

private:
  std::vector<Pixel> &pixels;
  int w, h;
  long num_pixels;

  // prices for all target pixels
  std::vector<double> prices;
  // map source pixels to assigned target pixels
  std::vector<long> assigned;
  // map target pixels to owned source pixels
  std::vector<long> owned;

  inline double get_value(long source_index, long target_index) {
    int target_x = target_index % this->w;
    int target_y = target_index / this->w;

    double dx = this->pixels[source_index].avg_x - target_x;
    double dy = this->pixels[source_index].avg_y - target_y;

    return -(dx * dx + dy * dy);
  }

  Auction(std::vector<Pixel> &pixels, int w, int h)
      : pixels(pixels), w(w), h(h), num_pixels((long)w * h) {
    prices.assign(num_pixels, 0);
    assigned.assign(num_pixels, -1);
    owned.assign(num_pixels, -1);
  }
  // run auction to find optimal positions for all source pixels
  void run_auction();
};
} // namespace Placement
