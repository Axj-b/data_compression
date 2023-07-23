#pragma once
#include "PlatformAPI.h"
#if 0
uint32 lz77_compress(uint8 *uncompressed_text, uint32 uncompressed_size, uint8 *compressed_text)
{
    uint8 pointer_length, temp_pointer_length;
    uint16 pointer_pos, temp_pointer_pos, output_pointer;
    uint32 compressed_pointer, output_size, coding_pos, output_lookahead_ref, look_behind, look_ahead;

    *((uint32 *)compressed_text) = uncompressed_size;
    compressed_pointer = output_size = 4;

    for (coding_pos = 0; coding_pos < uncompressed_size; ++coding_pos)
    {
        pointer_pos = 0;
        pointer_length = 0;
        for (temp_pointer_pos = 1; (temp_pointer_pos < 4096) && (temp_pointer_pos <= coding_pos); ++temp_pointer_pos)
        {
            look_behind = coding_pos - temp_pointer_pos;
            look_ahead = coding_pos;
            for (temp_pointer_length = 0; uncompressed_text[look_ahead++] == uncompressed_text[look_behind++]; ++temp_pointer_length)
                if (temp_pointer_length == 15)
                    break;
            if (temp_pointer_length > pointer_length)
            {
                pointer_pos = temp_pointer_pos;
                pointer_length = temp_pointer_length;
                if (pointer_length == 15)
                    break;
            }
        }
        coding_pos += pointer_length;
        if (pointer_length && (coding_pos == uncompressed_size))
        {
            output_pointer = (pointer_pos << 4) | (pointer_length - 1);
            output_lookahead_ref = coding_pos - 1;
        }
        else
        {
            output_pointer = (pointer_pos << 4) | pointer_length;
            output_lookahead_ref = coding_pos;
        }
        *((uint32 *)(compressed_text + compressed_pointer)) = output_pointer;
        compressed_pointer += 2;
        *(compressed_text + compressed_pointer++) = *(uncompressed_text + output_lookahead_ref);
        output_size += 3;
    }

    return output_size;
}
#endif