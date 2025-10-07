#ifndef SWD_SORTER_H
#define SWD_SORTER_H

#ifdef __cplusplus
extern "C" {
#endif

void swd_remap(unsigned char **result_img, const unsigned char *src_img,
                     const unsigned char *trgt_img, int w, int h, int L);

#ifdef __cplusplus
}
#endif // __cplusplus
//
#endif // !SWD_SORTER_H
