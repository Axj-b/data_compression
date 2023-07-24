#include "PlatformAPI.h"

#define CompressorVersion 1
#define StackUsage_byte 1 * 1024 
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

uint32 readCounter = 0;
char ReadChar(const uint8* ptr)
{
    readCounter++;
    char* cPtr = (char*)ptr;
    return *cPtr;
}


// return compressed size
uint32 rle_compress(const uint8* dataSource, uint32 dataSourceSize, uint8* compressedDestination)
{
    uint32 compressedSize = 0U;
    uint8 _stackbuffer[StackUsage_byte];
    compressedHeader* _header = reinterpret_cast<compressedHeader*>(&compressedDestination[compressedSize]);
    _header->version = 1;
    _header->originalSize = dataSourceSize;
    _header->reserved = 0xffffffff;

    // reserve space for header
    compressedSize += sizeof(compressedHeader);
    // devide into splits to work on stack instead of extern memory
    const uint32 _splits = dataSourceSize / StackUsage_byte;
    for (uint32 _split = 0U; _split < _splits; _split++) 
    {
        memcpy(_stackbuffer, &dataSource[_split * StackUsage_byte], sizeof(StackUsage_byte));
        for (int index = 0; index < StackUsage_byte; index++)
        {
            char newByte = ReadChar(&_stackbuffer[index]);
            unsigned char numBytesRepeated = 0;
            for (int nextIndex = index + 1; nextIndex <= StackUsage_byte; nextIndex++)
            {
                char nextByte = ReadChar(&_stackbuffer[nextIndex]);
                if (newByte == nextByte)
                {
                    if (numBytesRepeated != 0xff)
                    {
                        numBytesRepeated++;
                    }
                    else
                    {
                        compressedEntry* newEntry = (compressedEntry*)&compressedDestination[compressedSize];
                        newEntry->character = newByte;
                        newEntry->num = numBytesRepeated;
                        compressedSize += sizeof(compressedEntry);
                        numBytesRepeated = 0;
                    }
                }
                else
                {
                    index = nextIndex - 1;
                    compressedEntry* newEntry = (compressedEntry*)&compressedDestination[compressedSize];
                    newEntry->character = newByte;
                    newEntry->num = numBytesRepeated;
                    compressedSize += sizeof(compressedEntry);

                    break;
                }
            }
        }
    }
    _header->compressedSize = compressedSize;
    cout << "ReadBytes: " << readCounter << endl;
    return compressedSize;
}


uint32 rle_decompress(uint8* dataSource, uint32 dataSourceSize, uint8* decompressedDestination)
{
    int readBytes = dataSourceSize;
    int writeBytes = 0;
    int NumEntries = (readBytes - sizeof(compressedHeader)) / sizeof(compressedEntry);
    compressedHeader* _header = reinterpret_cast<compressedHeader*>(&dataSource[0]);
    if (_header->version != CompressorVersion)
    {
        printf("the data was compressed with version: %d \n", _header->version);
    }
    const unsigned int _originalSize = _header->originalSize;

    compressedEntry* list = reinterpret_cast<compressedEntry*>(&dataSource[sizeof(compressedHeader)]);
    for (int entry = 0; entry < NumEntries; entry++)
    {
        const char character = list[entry].character;
        decompressedDestination[writeBytes++] = character;
        for (int numBytes = 0; numBytes < list[entry].num; numBytes++)
        {
            decompressedDestination[writeBytes++] = character;
        }
    }
    if (_originalSize == writeBytes)
    {
        printf("decompression sucess. orignal size == decompressed size\n");
    }
    return writeBytes;
}