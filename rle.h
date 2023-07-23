#pragma once
#include "PlatformAPI.h"

uint32 compressRLE(uint8 *input, uint32 inputSize, uint8 *compressedData)
{
    int count = 1;
    int compressedIndex = 0;

    for (int i = 1; i < inputSize; i++)
    {
        if (input[i] == input[i - 1])
        {
            count++;
        }
        else
        {
            // Write the count and character to the compressed data
            compressedData[compressedIndex++] = count;
            compressedData[compressedIndex++] = input[i - 1];
            count = 1;
        }
    }

    // Write the last count and character to the compressed data
    compressedData[compressedIndex++] = count;
    compressedData[compressedIndex++] = input[inputSize - 1];

    // Set the compressed size
    return compressedIndex;
}


