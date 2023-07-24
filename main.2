

#include <stdint.h>
#include <string>
#include "RunLenghtCompression.h"
uint32 lz77_compress(uint8 *uncompressed_text, uint32 uncompressed_size, uint8 *compressed_text)
{
    uint8 pointer_length, temp_pointer_length;
    uint16_t pointer_pos, temp_pointer_pos, output_pointer;
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

uint32 lz77_decompress(char *compressed_text, char *uncompressed_text)
{
    uint8 pointer_length;
    uint16_t input_pointer, pointer_pos;
    uint32 compressed_pointer, coding_pos, pointer_offset, uncompressed_size;

    uncompressed_size = *((uint32 *)compressed_text);
    compressed_pointer = 4;

    for (coding_pos = 0; coding_pos < uncompressed_size; ++coding_pos)
    {
        input_pointer = *((uint32 *)(compressed_text + compressed_pointer));
        compressed_pointer += 2;
        pointer_pos = input_pointer >> 4;
        pointer_length = input_pointer & 15;
        if (pointer_pos)
            for (pointer_offset = coding_pos - pointer_pos; pointer_length > 0; --pointer_length)
                uncompressed_text[coding_pos++] = uncompressed_text[pointer_offset++];
        *(uncompressed_text + coding_pos) = *(compressed_text + compressed_pointer++);
    }

    return coding_pos;
}


uint32 file_lz77_compress(const char *filename_in, const char *filename_out)
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

    compressed_size = lz77_compress((uint8*)uncompressed_text, uncompressed_size, (uint8*)compressed_text);

    out = fopen(filename_out, "wb");
    if (out == NULL)
        return 0;
    if ((compressed_size != fwrite(compressed_text, 1, compressed_size, out)))
        return 0;
    fclose(out);

    return compressed_size;
}
uint8  compressedBuffer[1*1024*1024];


uint32 file_lz77_decompress(const char *filename_in, const char *filename_out)
{
    FILE *in, *out;
    uint32 compressed_size, uncompressed_size;
    void *compressed_text, *uncompressed_text;

    in = fopen(filename_in, "rb");
    if (in == NULL)
        return 0;
    compressed_size = fsize(in);
    compressed_text = malloc(compressed_size);
    size_t readBytes  = fread(compressed_text, 1, compressed_size, in);
    if (readBytes != compressed_size)
        return 0;
    fclose(in);

    uncompressed_size = *((uint32 *)compressed_text);
    uncompressed_text = malloc(uncompressed_size);

    if (lz77_decompress((char*)compressed_text, (char*)uncompressed_text) != uncompressed_size)
        return 0;

    out = fopen(filename_out, "wb");
    if (out == NULL)
        return 0;
    if (fwrite(uncompressed_text, 1, uncompressed_size, out) != uncompressed_size)
        return 0;
    fclose(out);

    return uncompressed_size;
}

int main(int argc, char const *argv[])
{

    std::string inputFilename_raw = ".\\..\\main.cpp";
    std::string inputFilename_compressed = "..\\main.z77";
    std::string outputFilename_compressed = "../main.z77";
    std::string outputFilename_compressed_rle = "../main.rle";
    std::string outputFilename_decompressed = "../main_2.cpp";
    

    char name;
    int number;
    FILE *f;
    f = fopen("test.txt", "a");

    fprintf(f, inputFilename_raw.c_str(), "test");
    fclose(f);

    FILE *in;
    in = fopen(inputFilename_raw.c_str(), "r");
    if (in == NULL)
        return 0;
    printf("Original size: %ld\n", fsize(in));
    fclose(in);
    printf("Compressed: %u\n", file_lz77_compress(inputFilename_raw.c_str(), outputFilename_compressed.c_str()));
    printf("Compressed: %u\n", file_rle_compress(inputFilename_raw.c_str(), outputFilename_compressed_rle.c_str()));
    printf("Decompressed: %u", file_lz77_decompress(inputFilename_compressed.c_str(), outputFilename_decompressed.c_str()));



    


    return 0;
}
