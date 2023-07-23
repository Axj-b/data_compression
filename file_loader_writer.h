#pragma once
#include "PlatformAPI.h"
#include <stdint.h>
#include <string>
 

uint32 file_compress(const char *filename_in, const char *filename_out, uint32 (*fun_ptr)( uint8*, const uint32, uint8*))
{
    FILE *in, *out;
    void *uncompressed_text, *compressed_text;
    uint32 uncompressed_size, compressed_size;

    in = fopen(filename_in, "rb");
    if (in == NULL)
        return 0;
    uncompressed_size = fsize(in);
    uncompressed_text = malloc(uncompressed_size);
    if ((uncompressed_size != fread(uncompressed_text, 1, uncompressed_size, in)))
        return 0;
    fclose(in);

    compressed_text = (uint8 *)malloc(655360);

    compressed_size = fun_ptr((uint8 *)uncompressed_text, uncompressed_size, (uint8 *)compressed_text);

    out = fopen(filename_out, "wb");
    if (out == NULL)
        return 0;
    if ((compressed_size != fwrite(compressed_text, 1, compressed_size, out)))
        return 0;
    fclose(out);

    return compressed_size;
}


