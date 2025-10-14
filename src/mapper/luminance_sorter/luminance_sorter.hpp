#pragma once

namespace LuminanceSorter {
void luminance_remap(unsigned char **result_img, const unsigned char *src_img,
                     const unsigned char *target_img, int w, int h);
}
