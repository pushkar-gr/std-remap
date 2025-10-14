# std::remap

A small C++ program that creates an "image anagram": it reconstructs a target image using only pixels taken from a source image.

About
-----
std::remap resizes the source to the target size, computes a luminance value for each pixel, sorts source pixels and target positions by luminance, then maps source pixels into target positions in luminance order. The output has the structure of the target but uses only colors from the source.

Requirements
------------
- Make and a C++ compiler (g++) with C++17 support
- The project includes single-file stb libraries in include/:
  - stb_image.h
  - stb_image_resize2.h
  - stb_image_write.h

How to build
------------
Build with the provided Makefile:

```bash
make
```

This produces the executable at build/remap. You can also run:

```bash
make clean
make
```

How to run
----------
Basic usage:

```bash
./build/remap -s source.jpg -t target.jpg -o output.png
```

If flags are omitted the program uses these defaults:
- source: source.jpg
- target: target.jpg
- output: output.jpg

Show help:

```bash
./build/remap -h
# or
./build/remap --help
```

Note: the program writes PNG output (uses stbi_write_png), so prefer an output filename with .png.

Simple example:

```bash
# build
make

# run with defaults (if you have source.jpg and target.jpg in repo root)
./build/remap

# explicit files
./build/remap -s images/source.jpg -t images/portrait.jpg -o results/anagram.png
```

Approach (brief)
----------------
- Resize the source image to the target's dimensions.
- Compute perceptual luminance for every pixel (0.2126 R + 0.7152 G + 0.0722 B).
- Sort source pixels by luminance and sort target pixel positions by luminance.
- Assign the nth-darkest source pixel to the nth-darkest target position.
- Save the assembled image.

Notes and limitations
---------------------
- This is a simple, deterministic luminance-matching approach. It preserves the source palette and overall tonal distribution but does not attempt advanced matching (e.g., color clustering, patch-based texture matching, or iterative optimization).
- Large images use more memory; consider resizing inputs before processing if you run into memory limits.

License
-------
MIT — see LICENSE file.
