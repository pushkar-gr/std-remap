#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// show help message
void show_usage(const char *name);
// read input flags
void read_flags(int argc, char *argv[], const char **source_path,
                const char **target_path, const char **output_path);

int main(int argc, char *argv[]) {
  // default values
  const char *source_path = "source.jpg";
  const char *target_path = "target.jpg";
  const char *output_path = "output.jpg";

  // read user flags
  read_flags(argc, argv, &source_path, &target_path, &output_path);

  // print configuration
  printf("Configuration:\n");
  printf("  Source: %s\n", source_path);
  printf("  Target: %s\n", target_path);
  printf("  Output: %s\n", output_path);
  printf("-------------------------\n");

  return EXIT_SUCCESS;
}

void show_usage(const char *name) {
  fprintf(stderr,
          "Usage: %s [options]\n"
          "Options:\n"
          "  -h, --help\t\tShow this help message\n"
          "  -h, --source FILE\tSource image path (default: source.jpg)\n"
          "  -t, --target FILE\tTarget image path (default: target.jpg)\n"
          "  -o, --output FILE\tOutput image path (default: output.jpg)\n",
          name);
}

void read_flags(int argc, char *argv[], const char **source_path,
                const char **target_path, const char **output_path) {
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
    }
  }
}
