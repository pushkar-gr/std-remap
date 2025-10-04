# std::remap

A small C program that creates an "image anagram": it reconstructs a target image using only pixels taken from a source image.

About
-----
std::remap resizes the source to the target size, computes a simple luminance value for every pixel, sorts source and target pixels by luminance, and then places source pixels into the target positions in luminance order. The result looks like the target image but uses only colors from the source.

Dependencies
------------
- A C compiler (gcc or clang)
- The single-file stb libraries included in the repo:
  - stb_image.h
  - stb_image_write.h
  - stb_image_resize2.h

How to build
------------
```bash
make all
```

How to use
----------
Basic usage:

```bash
./std-remap -s source.jpg -t target.jpg -o output.png
```

Default file names used by the program when flags are not provided:
- source: source.jpg
- target: target.jpg
- output: output.jpg

Run the program with `-h` or `--help` to see the exact flags.

Approach
----------------
- Resize the source to the target dimensions.
- Compute luminance for each pixel (perceptual grayscale).
- Sort source pixels by luminance and sort target pixel positions by luminance.
- Map sorted source pixels onto target positions to form the final image.
- Write the output image to disk.
