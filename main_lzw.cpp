#include <stdio.h>
#include <string.h>
#define MAX_CODES 4096
#define CompressorVersion 1
typedef struct dictionary
{
    short code;
    short prefix_code;
    short character;
} dictionary;

dictionary dict[MAX_CODES];

int next_code;
char compressionBuffer[512 * 1024];
char decompressionBuffer[512 * 1024];
long fsize(FILE* in)
{
    long pos, length;
    pos = ftell(in);
    fseek(in, 0L, SEEK_END);
    length = ftell(in);
    fseek(in, pos, SEEK_SET);
    return length;
}
void init_dict()
{
    for (int i = 0; i < 256; i++)
    {
        dict[i].code = i;
        dict[i].prefix_code = -1;
        dict[i].character = i;
    }
    next_code = 256;
}

short find_dict(short prefix_code, short character)
{
    for (short i = 0; i < next_code; i++)
    {
        if (dict[i].prefix_code == prefix_code && dict[i].character == character)
        {
            return i;
        }
    }
    return -1;
}

void add_dict(short prefix_code, short character)
{
    if (next_code < MAX_CODES)
    {
        dict[next_code].code = next_code;
        dict[next_code].prefix_code = prefix_code;
        dict[next_code].character = character;
        next_code++;
    }
}
#pragma pack(push, 1)
struct compressedHeader
{
    unsigned int version; 
    unsigned int compressedSize;
    unsigned int originalSize;
    unsigned int reserved;
};
struct compressedEntry
{
    char character;
    unsigned char num;
};
#pragma pack(pop)

void compress(FILE* input, FILE* output)
{
    int orginalSize = fsize(input);
    printf("compression \noriginal size: %d\n", orginalSize);
    memset(&decompressionBuffer[0], 0x00, sizeof(decompressionBuffer));
    size_t readBytes = fread(&decompressionBuffer[0], 1, orginalSize, input);
    int newSize = 0;

#if 1

    // write header. fill when finished
    compressedHeader* _header = reinterpret_cast<compressedHeader*>(&compressionBuffer[newSize]);
    _header->version = 1;
    _header->originalSize = orginalSize;
    _header->reserved = 0xffffffff;
    newSize += sizeof(compressedHeader);
    for (int index = 0; index < readBytes; index++)
    {
        char newByte = decompressionBuffer[index];
        unsigned char numBytesRepeated = 0;
        for (int nextIndex = index + 1; nextIndex <= readBytes; nextIndex++)
        {
            char nextByte = decompressionBuffer[nextIndex];     
            if (newByte == nextByte)
            {
                if (numBytesRepeated == 255)
                {
                    compressedEntry* newEntry = (compressedEntry*)&compressionBuffer[newSize];
                    newEntry->character = newByte;
                    newEntry->num = numBytesRepeated;
                    newSize += sizeof(compressedEntry);
                    numBytesRepeated = 0;
                }
                else
                {
                    numBytesRepeated++;
                }
            }
            else
            {
                index = nextIndex - 1;
                compressedEntry* newEntry = (compressedEntry*)&compressionBuffer[newSize];
                newEntry->character = newByte;
                newEntry->num = numBytesRepeated;
                newSize += sizeof(compressedEntry);

                break;
            }
        }
    }
    _header->compressedSize = newSize;


#else
    init_dict();
    //short prefix_code = fgetc(input);
    //short character = fgetc(input);    
    short prefix_code = decompressionBuffer[0];
    short character = decompressionBuffer[1];

    int charIdx = 2;
    for (; charIdx < orginalSize; )
        //while (character != EOF)
    {
        short index = find_dict(prefix_code, character);
        if (index != -1)
        {
            prefix_code = index;
        }
        else
        {


            // fwrite(&dict[prefix_code].code, sizeof(short), 1, output);

            compressionBuffer[newSize++] = prefix_code;

            short index = find_dict(prefix_code, character);
            if (index == -1)
            {

                add_dict(prefix_code, character);
            }

            prefix_code = find_dict(-1, character);

            //character = fgetc(input);
        }
        character = decompressionBuffer[charIdx++];


    }
    compressionBuffer[newSize++] = dict[prefix_code].code;
    // fwrite(&dict[prefix_code].code, sizeof(short), 1, output);
#endif
    printf("new Size: %d\n", newSize);

    fwrite(&compressionBuffer[0], 1, newSize, output);
}

void decompress(FILE* input, FILE* output)
{
    int compressed_size = fsize(input);

#if 1

    memset(&compressionBuffer[0], 0x00, sizeof(compressionBuffer));
    size_t readBytes = fread(&compressionBuffer[0], 1, compressed_size, input);
    int writeBytes = 0;
    int NumEntries = (readBytes-sizeof(compressedHeader)) / sizeof(compressedEntry);
    compressedHeader* _header = reinterpret_cast<compressedHeader*>(&compressionBuffer[0]);
    if (_header->version != CompressorVersion)
    {
        printf("the data was compressed with version: %d \n", _header->version);
    }
    const unsigned int _originalSize = _header->originalSize;
    
    compressedEntry* list = reinterpret_cast<compressedEntry*>(&compressionBuffer[sizeof(compressedHeader)]);
    for (int entry = 0; entry < NumEntries; entry++)
    {
        const char character = list[entry].character;
        decompressionBuffer[writeBytes++] = character;
        for (int numBytes = 0; numBytes < list[entry].num; numBytes++)
        {
            decompressionBuffer[writeBytes++] = character;
        }
    }
    if (_originalSize == writeBytes)
    {
        printf("decompression sucess. orignal size == decompressed size\n");
    }


#else
    int byteWrite = 0;
    short old_code = 0;
    init_dict();
    if (readBytes != compressed_size)
    {
        printf("error missmatch in size\n");
        return;
    }
    fclose(input);

    // fread(&old_code, 1, 2, input);
    old_code = (compressionBuffer[0]);

    // fputc(dict[old_code].character, output);
    decompressionBuffer[byteWrite++] = dict[old_code].character;
    // decompressionBuffer[byteWrite++] = old_code>>8;
    // decompressionBuffer[byteWrite++] = old_code;
    short new_code = 0;

    for (int i = 1; i < (compressed_size); i++)
        // while (fread(&new_code,2, 1, input) == 1)
    {
        new_code = (compressionBuffer[i]);
        if (new_code >= next_code)
        {
            add_dict(old_code, dict[new_code].character);
            short _prevCode = 0;
            short _char = 0;
            short index = find_dict(_prevCode, dict[new_code].character);
            printf("prev: %x, char: %x, index: %x\n", _prevCode, _char, index);

            // fputc(dict[old_code].character, output);
            decompressionBuffer[byteWrite++] = dict[new_code].character;

        }
        else
        {
            add_dict(old_code, dict[new_code].character);

            short code = new_code;
            while (code >= 0)
            {
                // fputc(dict[code].character, output);
                decompressionBuffer[byteWrite++] = dict[code].character;
                code = dict[code].prefix_code;
            }
        }
        old_code = new_code;
    }
#endif
    fwrite(decompressionBuffer, 1, writeBytes, output);
}

int main(int argc, char* argv[])
{
    // if (argc != 4)
    // {
    //     printf("Usage: lzw [c|d] input_file output_file\n");
    //     return 1;
    // }
    printf("start\n");
    // char mode = argv[1][0];
    // char *input_file_name = argv[2];
    // char *output_file_name = argv[3];

    char mode = 'c';
    char* input_file_name = "D:\\development\\Compressing\\test.bin";
    char* output_file_name = "D:\\development\\Compressing\\test.lzw";
    char* input_file_name_decompress = "D:\\development\\Compressing\\test.lzw";
    char* output_file_name_docmpressed = "D:\\development\\Compressing\\test2.bin";

    FILE* input_file = fopen(input_file_name, "rb");
    if (!input_file)
    {
        printf("Error opening input file\n");
        return 1;
    }

    FILE* output_file = fopen(output_file_name, "wb");
    if (!output_file)
    {
        printf("Error opening output file\n");
        fclose(input_file);
        return 1;
    }
    printf("compress\n");
    if (mode == 'c')
    {
        printf("orig size: %d\n", sizeof(input_file));
        compress(input_file, output_file);
        printf("new  size: %d\n", sizeof(output_file));
    }
    fclose(input_file);
    fclose(output_file);
    FILE* input_file_Dec = fopen(input_file_name_decompress, "rb");
    FILE* output_file_Dec = fopen(output_file_name_docmpressed, "wb");
    mode = 'd';
    if (mode == 'd')
    {
        printf("decompress\n");
        decompress(input_file_Dec, output_file_Dec);
    }
    else
    {
        printf("Invalid mode\n");
        fclose(input_file);
        fclose(output_file);
        fclose(input_file_Dec);
        fclose(output_file_Dec);
        return 1;
    }

    fclose(input_file_Dec);
    fclose(output_file_Dec);
    return 0;
}
