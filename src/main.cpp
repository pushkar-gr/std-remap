#include <iostream>

#include "std_reamp.hpp"

int main(int argc, char *argv[]) {
  StdRemap::StdRemap *stdRemap = new StdRemap::StdRemap;
  stdRemap->read_flags(argc, argv);
  stdRemap->load_images();
  stdRemap->resize_source_image();
  stdRemap->build_output_image();
  delete stdRemap;
  std::cout << "Done" << std::endl;
  return EXIT_SUCCESS;
}
