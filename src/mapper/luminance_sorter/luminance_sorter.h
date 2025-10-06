#ifndef LUMINANCE_SORTER_H
#define LUMINANCE_SORTER_H

#ifdef __cplusplus
extern "C" {
#endif

void luminance_remap(unsigned char **result_img, const unsigned char *src_img,
                     const unsigned char *trgt_img, int w, int h);

#ifdef __cplusplus
}
#endif // __cplusplus
//
#endif // !LUMINANCE_SORTER_H
