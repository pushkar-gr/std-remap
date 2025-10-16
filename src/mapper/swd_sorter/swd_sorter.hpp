#pragma once

namespace Mapper {
void swd_remap(unsigned char **result_img, const unsigned char *src_img,
               const unsigned char *target_img, int w, int h, int L,
               const char *placement);
}
