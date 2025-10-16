#include <iostream>
#include <limits>
#include <numeric>
#include <utility>
#include <vector>

#include "auction.hpp"

namespace Placement {
void Auction::run_auction() {
  double epsilon = 1;
  const double scaling_factor = 4;

  // vector to store all unassignedl pixels
  std::vector<long> unassigned_pixels_indices(this->num_pixels);
  // fill vector
  std::iota(unassigned_pixels_indices.begin(), unassigned_pixels_indices.end(),
            0);

  while (epsilon > 1.0 / this->num_pixels) {
    std::cout << "Running auction round with epsilon = " << epsilon
              << std::endl;

    while (!unassigned_pixels_indices.empty()) {
      // bids for each target pixels
      // bids_for_locations[location_index] = {{source_pixel_index, bid}}
      std::vector<std::vector<std::pair<long, double>>> bids_for_locations(
          this->num_pixels);

      // find best bid for each source pixels
      for (long source_index : unassigned_pixels_indices) {
        long best_target_index = -1;
        double best_profit = -std::numeric_limits<double>::infinity();
        double second_best_profit = -std::numeric_limits<double>::infinity();

        for (long target_index = 0; target_index < this->num_pixels;
             target_index++) {
          double profit = this->get_value(source_index, target_index) -
                          this->prices[target_index];

          if (profit > best_profit) {
            second_best_profit = best_profit;
            best_target_index = target_index;
            best_profit = profit;
          } else if (profit > second_best_profit) {
            second_best_profit = profit;
          }
        }

        // calculate best bid for maxium profit
        double bid = best_profit - second_best_profit + epsilon;
        bids_for_locations[best_target_index].push_back({source_index, bid});
      }

      // assign target pixels to best bidders
      unassigned_pixels_indices.clear();

      for (long target_index = 0; target_index < this->num_pixels;
           target_index++) {
        // continue if no bids
        if (bids_for_locations[target_index].empty()) {
          continue;
        }

        // find the winning bid from a source pixel
        long winning_bid_index = -1;
        double winning_bid = -1;
        for (const auto &bid : bids_for_locations[target_index]) {
          if (bid.second > winning_bid) {
            winning_bid = bid.second;
            winning_bid_index = bid.first;
          }
        }

        // remove owner if target pixel already assigned
        long previous_owner = this->owned[target_index];
        if (previous_owner != -1) {
          unassigned_pixels_indices.push_back(previous_owner);
          this->assigned[previous_owner] = -1;
        }

        this->assigned[winning_bid_index] = target_index;
        this->owned[target_index] = winning_bid_index;
        this->prices[target_index] += winning_bid;
      }
    }

    epsilon /= scaling_factor;

    if (epsilon > 1.0 / this->num_pixels) {
      std::iota(unassigned_pixels_indices.begin(),
                unassigned_pixels_indices.end(), 0);
      // this->assigned.assign(num_pixels, -1);
      // this->owned.assign(num_pixels, -1);
    }
  }
}
} // namespace Placement
