#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "mapper/luminance_sorter/luminance_sorter.h"
#include "mapper/swd_sorter/swd_sorter.h"

typedef struct {
  unsigned char r, g, b;
  int x, y;
  float luminance;
} Pixel;

// show help message
void show_usage(const char *name);
// read input flags
void read_flags(int argc, char *argv[], const char **source_path,
                const char **target_path, const char **output_path,
                const char **mapper);

int main(int argc, char *argv[]) {
  // default values
  const char *source_path = "source.jpg";
  const char *target_path = "target.jpg";
  const char *output_path = "output.jpg";
  const char *mapper = "ls";

  // read user flags
  read_flags(argc, argv, &source_path, &target_path, &output_path, &mapper);

  // print configuration
  printf("Configuration:\n");
  printf("  Source: %s\n", source_path);
  printf("  Target: %s\n", target_path);
  printf("  Output: %s\n", output_path);
  printf("-------------------------\n");

  // load source and target images
  int src_w, src_h, src_channels, target_w, target_h, target_channels;
  unsigned char *src_img =
      stbi_load(source_path, &src_w, &src_h, &src_channels, 3);
  if (src_img == NULL) {
    fprintf(stderr, "Error: Could not load source image at %s\n", source_path);
    return EXIT_FAILURE;
  }

  unsigned char *target_img =
      stbi_load(target_path, &target_w, &target_h, &target_channels, 3);
  if (target_img == NULL) {
    fprintf(stderr, "Error: Could not load target image at %s\n", source_path);
    stbi_image_free(src_img);
    return EXIT_FAILURE;
  }

  // resize source image
  printf("Resizing source image to match target image dimensions (%dx%d)...\n",
         target_w, target_h);
  unsigned char *resized_src_img =
      (unsigned char *)malloc(target_w * target_h * 3);
  if (!resized_src_img) {
    fprintf(stderr, "Error: Failed to allocate memory\n");
    return EXIT_FAILURE;
  }

  stbir_resize_uint8_linear(src_img, src_w, src_h, 0, resized_src_img, target_w,
                            target_h, 0, STBIR_RGB);
  stbi_image_free(src_img);

  long num_pixles = (long)target_w * target_h;
  unsigned char *result_img = (unsigned char *)malloc(num_pixles * 3);
  if (!result_img) {
    fprintf(stderr, "Error: Failed to allocate memory for final image\n");
    return EXIT_FAILURE;
  }

  if (strcmp(mapper, "ls") == 0) {
    luminance_remap(&result_img, resized_src_img, target_img, target_w, target_h);
  } else if (strcmp(mapper, "swds") == 0) {
    swd_remap(&result_img, resized_src_img, target_img, target_w, target_h);
  }

  free(resized_src_img);
  stbi_image_free(target_img);

  // write final image to file
  printf("Saving result to %s...\n", output_path);
  stbi_write_png(output_path, target_w, target_h, 3, result_img, target_w * 3);
  free(result_img);

  printf("Done\n");
  return EXIT_SUCCESS;
}

void show_usage(const char *name) {
  fprintf(stderr,
          "Usage: %s [options]\n"
          "Options:\n"
          "  -h, --help\t\tShow this help message\n"
          "  -s, --source FILE\tSource image path (default: source.jpg)\n"
          "  -t, --target FILE\tTarget image path (default: target.jpg)\n"
          "  -o, --output FILE\tOutput image path (default: output.jpg)\n"
          "  -m, --mapper [mappers]\tChoose mapper (default: ls (luminance "
          "sorter))\n"
          "  Mappers:\n"
          "  \t\tls: luminance sorter\n"
          "  \t\tswds: SWD sorter\n",
          name);
}

void read_flags(int argc, char *argv[], const char **source_path,
                const char **target_path, const char **output_path,
                const char **mapper) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      show_usage(argv[0]);
      exit(EXIT_SUCCESS);
    } else if ((strcmp(argv[i], "-s") == 0 ||
                strcmp(argv[i], "--source") == 0) &&
               i + 1 < argc) {
      *source_path = argv[++i];
    } else if ((strcmp(argv[i], "-t") == 0 ||
                strcmp(argv[i], "--target") == 0) &&
               i + 1 < argc) {
      *target_path = argv[++i];
    } else if ((strcmp(argv[i], "-o") == 0 ||
                strcmp(argv[i], "--output") == 0) &&
               i + 1 < argc) {
      *output_path = argv[++i];
    } else if ((strcmp(argv[i], "-m") == 0 ||
                strcmp(argv[i], "--mapper") == 0) &&
               i + 1 < argc) {
      *mapper = argv[++i];
    }
  }
}
