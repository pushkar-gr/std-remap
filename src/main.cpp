#include "mapper/swd_sorter/swd_sorter.hpp"
#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "mapper/luminance_sorter/luminance_sorter.hpp"
#include "mapper/swd_sorter/swd_sorter.hpp"

class StdRemap {
public:
  StdRemap();
  StdRemap(StdRemap &&) = default;
  StdRemap(const StdRemap &) = default;
  StdRemap &operator=(StdRemap &&) = default;
  StdRemap &operator=(const StdRemap &) = default;
  // read input flags
  void read_flags(int argc, char *argv[]);
  // load source and target images
  void load_images();
  // resize source image to match target image dimensions
  void resize_source_image();
  // build output image
  void build_output_image();
  ~StdRemap();

private:
  // default values
  const char *source_path;
  const char *target_path;
  const char *output_path;
  const char *mapper;
  int L;
  const char *placement;

  // images
  unsigned char *src_img;
  unsigned char *target_img;
  unsigned char *resized_src_img;
  unsigned char *result_img;
  int src_w, src_h;
  int w, h;

  // show help message
  void show_usage(const char *name);
  void print_config();
};

int main(int argc, char *argv[]) {
  StdRemap *stdRemap = new StdRemap;
  stdRemap->read_flags(argc, argv);
  stdRemap->load_images();
  stdRemap->resize_source_image();
  stdRemap->build_output_image();
  printf("Done\n");
  delete stdRemap;
  return EXIT_SUCCESS;
}

StdRemap::StdRemap() {
  this->source_path = "source.jpg";
  this->target_path = "target.jpg";
  this->output_path = "output.jpg";
  this->mapper = "ls";
  this->mapper = "auction";
  this->L = 50;
}

void StdRemap::read_flags(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      this->show_usage(argv[0]);
      exit(EXIT_SUCCESS);
    } else if ((strcmp(argv[i], "-s") == 0 ||
                strcmp(argv[i], "--source") == 0) &&
               i + 1 < argc) {
      this->source_path = argv[++i];
    } else if ((strcmp(argv[i], "-t") == 0 ||
                strcmp(argv[i], "--target") == 0) &&
               i + 1 < argc) {
      this->target_path = argv[++i];
    } else if ((strcmp(argv[i], "-o") == 0 ||
                strcmp(argv[i], "--output") == 0) &&
               i + 1 < argc) {
      this->output_path = argv[++i];
    } else if ((strcmp(argv[i], "-m") == 0 ||
                strcmp(argv[i], "--mapper") == 0) &&
               i + 1 < argc) {
      this->mapper = argv[++i];
    } else if ((strcmp(argv[i], "-L") == 0 || (strcmp(argv[i], "-l") == 0)) &&
               i + 1 < argc) {
      this->L = atoi(argv[++i]);
    } else if ((strcmp(argv[i], "-p") == 0 ||
                (strcmp(argv[i], "--placement") == 0)) &&
               i + 1 < argc) {
      this->placement = argv[++i];
    }
  }
  this->print_config();
}

void StdRemap::load_images() {
  int src_channels, target_channels;
  this->src_img = stbi_load(this->source_path, &this->src_w, &this->src_h,
                            &src_channels, 3);
  if (this->src_img == NULL) {
    std::cout << "Error: Could not load source image at " << this->source_path
              << std::endl;
    exit(EXIT_FAILURE);
  }

  this->target_img =
      stbi_load(this->target_path, &this->w, &this->h, &target_channels, 3);
  if (this->target_img == NULL) {
    std::cout << "Error: Could not load target image at " << this->target_path
              << std::endl;
    stbi_image_free(this->src_img);
    exit(EXIT_FAILURE);
  }
}

void StdRemap::resize_source_image() {
  std::cout << "Resizing source image to match target image dimensions ("
            << this->w << "x" << this->h << ")" << std::endl;
  this->resized_src_img = (unsigned char *)malloc(this->w * this->h * 3);
  if (!this->resized_src_img) {
    std::cout << "Error: Failed to allocate memory" << std::endl;
    exit(EXIT_FAILURE);
  }

  stbir_resize_uint8_linear(this->src_img, this->src_w, this->src_h, 0,
                            this->resized_src_img, this->w, this->h, 0,
                            STBIR_RGB);
  stbi_image_free(this->src_img);
}

void StdRemap::build_output_image() {
  long num_pixles = (long)this->w * this->h;
  this->result_img = (unsigned char *)malloc(num_pixles * 3);
  if (!this->result_img) {
    std::cout << "Error: Failed to allocate memory for final image"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  if (strcmp(this->mapper, "ls") == 0) {
    LuminanceSorter::luminance_remap(&this->result_img, this->resized_src_img,
                                     this->target_img, this->w, this->h);
  } else if (strcmp(this->mapper, "swds") == 0) {
    SwdSorter::swd_remap(&this->result_img, this->resized_src_img,
                         this->target_img, this->w, this->h, this->L,
                         this->placement);
  } else {
    std::cout << "Invalide mapper" << std::endl;
    exit(EXIT_FAILURE);
  }
  free(this->resized_src_img);
  stbi_image_free(this->target_img);

  std::cout << "Saving result to " << this->output_path << "..." << std::endl;
  stbi_write_png(this->output_path, this->w, this->h, 3, this->result_img,
                 this->w * 3);
}

void StdRemap::show_usage(const char *name) {
  std::cout << "Usage: " << name << " [options]" << std::endl
            << "Options:" << std::endl
            << "  -h, --help\t\t\tShow this help message" << std::endl
            << "  -s, --source FILE\t\tSource image path (default: source.jpg)"
            << std::endl
            << "  -t, --target FILE\t\tTarget image path (default: target.jpg)"
            << std::endl
            << "  -o, --output FILE\t\tOutput image path (default: output.jpg)"
            << std::endl
            << "  -m, --mapper [MAPPER]\tChoose mapper (default: ls "
               "(luminance sorter))"
            << std::endl
            << "  -l, -L\t\t\tNumber of slices for SWD sorter (default: 50)"
            << std::endl
            << "  -p, --placement [ALGO]\tChoose placement algorithm for SWD "
               "sorter (default: auction)"
            << std::endl
            << std::endl
            << "  Mappers:" << std::endl
            << "  \t\tls: luminance sorter" << std::endl
            << "  \t\tswds: SWD sorter" << std::endl
            << std::endl
            << "  Placement Algorithms (for swds)" << std::endl
            << "  \t\tgreedy: Assigns each pixel to the closest available grid "
               "location."
            << std::endl
            << "  \t\tauction: Uses ε-scaling auction algorithm to resolve "
               "high-density conflicts."
            << std::endl;
}

void StdRemap::print_config() {
  std::cout << "Configuration:" << std::endl
            << "  Source: " << this->source_path << std::endl
            << "  Target: " << this->target_path << std::endl
            << "  Output: " << this->output_path << std::endl
            << "-------------------------" << std::endl;
}

StdRemap::~StdRemap() { free(this->result_img); }
