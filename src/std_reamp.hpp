#pragma once

namespace StdRemap {
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
} // namespace StdRemap
