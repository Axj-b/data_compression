#pragma once
#include <stdio.h>

typedef unsigned char		uint8;
typedef int sint32;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef short sint16;



long fsize(FILE *in)
{
    long pos, length;
    pos = ftell(in);
    fseek(in, 0L, SEEK_END);
    length = ftell(in);
    fseek(in, pos, SEEK_SET);
    return length;
}