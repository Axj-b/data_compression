#pragma once
#include "PlatformAPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 4096

#ifndef Entry_TYPE
#define Entry_TYPE
typedef struct {
    int prefix;
    uint8 suffix;
} Entry;
#endif

uint32 compressLZW_v1(const uint8* input, int uncompressedSize, int* compressedData) {
    // Initialize the dictionary with single character entries
    Entry dictionary[MAX_DICT_SIZE];
    for (int i = 0; i < 256; i++) {
        dictionary[i].prefix = -1;
        dictionary[i].suffix = i;
    }

    int dictSize = 256;  // Current dictionary size
    int currentPrefix = -1;
    char currentSuffix;

    int index = 0;

    // Read the first character from the input
    currentSuffix = input[index++];

    int compressedIndex = 0;

    while (index < uncompressedSize) {
        // Read the next character from the input
        char nextChar = input[index++];

        // Search for the longest prefix + suffix match in the dictionary
        int prefixIndex = currentPrefix;
        while (prefixIndex != -1) {
            if (dictionary[prefixIndex].suffix == nextChar)
                break;
            prefixIndex = dictionary[prefixIndex].prefix;
        }

        if (prefixIndex == -1) {
            // Output the code for the current prefix
            compressedData[compressedIndex++] = currentPrefix;

            // Add the new entry to the dictionary
            dictionary[dictSize].prefix = currentPrefix;
            dictionary[dictSize].suffix = nextChar;
            dictSize++;

            if (dictSize >= MAX_DICT_SIZE) {
                // Dictionary is full, reset it to the initial state
                dictSize = 256;
                memset(dictionary + dictSize, 0, (MAX_DICT_SIZE - dictSize) * sizeof(Entry));
            }

            // Set the current prefix to the code for the current suffix
            currentPrefix = nextChar;
        } else {
            // Found a match, update the current prefix
            currentPrefix = prefixIndex;
        }
    }

    // Output the code for the last prefix
    compressedData[compressedIndex++] = currentPrefix;

    // Return the compressed size
    return compressedIndex;
}



int compressLZW_v2(uint8* input, uint32 uncompressedSize, uint8* compressedData) {
    // Initialize the dictionary with single character entries
    Entry dictionary[MAX_DICT_SIZE];
    for (int i = 0; i < 256; i++) {
        dictionary[i].prefix = -1;
        dictionary[i].suffix = i;
    }

    int dictSize = 256;  // Current dictionary size
    int currentPrefix = -1;
    char currentSuffix;

    int index = 0;

    // Read the first character from the input
    currentSuffix = input[index++];

    int compressedIndex = 0;

    while (index < uncompressedSize) {
        // Read the next character from the input
        char nextChar = input[index++];

        // Search for the longest prefix + suffix match in the dictionary
        int prefixIndex = currentPrefix;
        while (prefixIndex != -1) {
            if (dictionary[prefixIndex].suffix == nextChar)
                break;
            prefixIndex = dictionary[prefixIndex].prefix;
        }

        if (prefixIndex == -1) {
            // Output the code for the current prefix
            compressedData[compressedIndex++] = currentPrefix;

            // Add the new entry to the dictionary
            dictionary[dictSize].prefix = currentPrefix;
            dictionary[dictSize].suffix = nextChar;
            dictSize++;

            // Reset the dictionary if it reaches its maximum size
            if (dictSize >= MAX_DICT_SIZE) {
                dictSize = 256;
                memset(dictionary + dictSize, 0, (MAX_DICT_SIZE - dictSize) * sizeof(Entry));
            }

            // Set the current prefix to the code for the current suffix
            currentPrefix = nextChar;
        } else {
            // Found a match, update the current prefix
            currentPrefix = prefixIndex;
        }
    }

    // Output the code for the last prefix
    compressedData[compressedIndex++] = currentPrefix;

    // Return the compressed size
    return compressedIndex;
}




// int main() {
//     const char* input = "abababcabababa";
//     int uncompressedSize = strlen(input);
//     int compressedData[MAX_DICT_SIZE];

//     compressLZW(input, uncompressedSize, compressedData);

//     // Print the compressed data
//     printf("Compressed data: ");
//     for (int i = 0; i < compressedSize; i++) {
//         printf("%d ", compressedData[i]);
//     }
//     printf("\n");

//     return 0;
// }



uint32 file_lzw_compress(const char *filename_in, const char *filename_out)
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

    compressed_size = compressLZW_v2((uint8 *)uncompressed_text, uncompressed_size, (uint8 *)compressed_text);

    out = fopen(filename_out, "wb");
    if (out == NULL)
        return 0;
    if ((compressed_size != fwrite(compressed_text, 1, compressed_size, out)))
        return 0;
    fclose(out);

    return compressed_size;
}