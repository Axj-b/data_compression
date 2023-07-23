#pragma once
#include "PlatformAPI.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_DICT_SIZE 4096
#ifndef Entry_TYPE
#define Entry_TYPE
typedef struct {
    int prefix;
    uint8 suffix;
} Entry;
#endif

uint32 compressLZ78(uint8* input, uint32 inputSize, uint8* compressedData_new) {
    int*compressedData  = (int*)compressedData_new;
    Entry dictionary[MAX_DICT_SIZE];
    for (int i = 0; i < MAX_DICT_SIZE; i++) {
        dictionary[i].prefix = -1;
        dictionary[i].suffix = 0;
    }

    int dictSize = 1;
    int currentIndex = 0;
    int compressedIndex = 0;

    while (currentIndex < inputSize) {
        int prefixIndex = 0;
        uint8 nextChar = input[currentIndex++];

        while (currentIndex < inputSize) {
            uint8 currentChar = input[currentIndex];
            int found = 0;

            for (int i = 0; i < dictSize; i++) {
                if (dictionary[i].prefix == prefixIndex && dictionary[i].suffix == currentChar) {
                    prefixIndex = i;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                break;
            }

            currentIndex++;
        }

        compressedData[compressedIndex++] = prefixIndex;
        compressedData[compressedIndex++] = nextChar;

        if (dictSize < MAX_DICT_SIZE) {
            dictionary[dictSize].prefix = prefixIndex;
            dictionary[dictSize].suffix = nextChar;
            dictSize++;
        }
    }

    return compressedIndex;
}


uint32 decompressLZ78(uint8 * compressedData_, uint32 compressedSize, uint8* decompressedData) {
    int* compressedData = (int *)compressedData_;
    Entry dictionary[MAX_DICT_SIZE];
    for (int i = 0; i < MAX_DICT_SIZE; i++) {
        dictionary[i].prefix = -1;
        dictionary[i].suffix = 0;
    }

    int dictSize = 1;
    uint32 decompressedIndex = 0;
    int compressedIndex = 0;

    while (compressedIndex < compressedSize) {
        int prefixIndex = compressedData[compressedIndex++];
        uint8 suffix = compressedData[compressedIndex++];

        // Output the suffix
        decompressedData[decompressedIndex++] = suffix;

        // Add the new entry to the dictionary
        if (dictSize < MAX_DICT_SIZE) {
            dictionary[dictSize].prefix = prefixIndex;
            dictionary[dictSize].suffix = suffix;
            dictSize++;
        }
    }

    return decompressedIndex;
}