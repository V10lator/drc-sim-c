//
// Created by rolando on 5/22/17.
//

#include <cstdint>
#include <cstring>
#include "ImageUtil.h"
#include "../data/Constants.h"
#include "Args.h"

extern "C" {
    #include <jpeglib.h>
}

size_t ImageUtil::rgb_to_jpeg(uint8_t *rgb, uint8_t *out, size_t outSize) {
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jerr.trace_level = 10;
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, &out, &outSize);

    cinfo.image_width = (JDIMENSION) WII_VIDEO_WIDTH;
    cinfo.image_height = (JDIMENSION) WII_VIDEO_HEIGHT;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, Args::video_quality, true);
    jpeg_start_compress(&cinfo, true);
    int row_stride = WII_VIDEO_WIDTH * 3;
    JSAMPROW row_pointer[1];
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &rgb[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return outSize;
}
