#include "PlatformAPI.h"

#define CompressorVersion 1

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

// return compressed size
uint32 rle_compress(const uint8 *dataSource, uint32 dataSourceSize, uint8 *compressedDestination)
{
    uint32 compressedSize = 0U;

    compressedHeader *_header = reinterpret_cast<compressedHeader *>(&compressedDestination[compressedSize]);
    _header->version = 1;
    _header->originalSize = dataSourceSize;
    _header->reserved = 0xffffffff;
    compressedSize += sizeof(compressedHeader);
    for (int index = 0; index < dataSourceSize; index++)
    {
        char newByte = dataSource[index];
        unsigned char numBytesRepeated = 0;
        for (int nextIndex = index + 1; nextIndex <= dataSourceSize; nextIndex++)
        {
            char nextByte = dataSource[nextIndex];
            if (newByte == nextByte)
            {
                if (numBytesRepeated != 0xff)
                {
                    numBytesRepeated++;
                }
                else
                {
                    compressedEntry *newEntry = (compressedEntry *)&compressedDestination[compressedSize];
                    newEntry->character = newByte;
                    newEntry->num = numBytesRepeated;
                    compressedSize += sizeof(compressedEntry);
                    numBytesRepeated = 0;
                }
            }
            else
            {
                index = nextIndex - 1;
                compressedEntry *newEntry = (compressedEntry *)&compressedDestination[compressedSize];
                newEntry->character = newByte;
                newEntry->num = numBytesRepeated;
                compressedSize += sizeof(compressedEntry);

                break;
            }
        }
    }
    _header->compressedSize = compressedSize;
    return compressedSize;
}