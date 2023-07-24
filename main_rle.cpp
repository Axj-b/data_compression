#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
//#include "rle_compression.h"




using namespace std;
long fsize(FILE* in)
{
    long pos, length;
    pos = ftell(in);
    fseek(in, 0L, SEEK_END);
    length = ftell(in);
    fseek(in, pos, SEEK_SET);
    return length;
}

int LoadFile(std::string input_file_name, void* data)
{
    std::fstream input_file;
    
    input_file.open(input_file_name.c_str(), ios::in | ios::binary | ios::ate);
    int fileSize = input_file.tellg();
    input_file.close();
    input_file.open(input_file_name.c_str(), ios::in | ios::binary);
    if (input_file.is_open())
    {

        input_file.read((char*)data, fileSize);
        input_file.close();
    }
    else {
        cout << "Error: could not open file to read: " << input_file_name << endl;
    }
    return fileSize;
}

int WriteFile(std::string output_file_name, void* data, unsigned int data_size)
{
    std::ofstream output_file;
    output_file.open(output_file_name.c_str(), ios::binary);
    if (output_file.is_open()) {

        output_file.write((char*)data, data_size);
        output_file.close();
    }
    else {
        cout << "Error: could not open file to write: " << output_file_name << endl;
    }
    return data_size;
}


char compressionBuffer[512 * 1024];
char originalData[512 * 1024];
char decompressionBuffer[512 * 1024];
#include "rle_compression.h"
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
    string input_file_name = "E:\\development\\\data_compression\\test_big.bin";
    string output_file_name = "E:\\development\\\data_compression\\test.lzw";
    string input_file_name_decompress = "e:\\development\\\data_compression\\test.lzw";
    string output_file_name_docmpressed = "e:\\development\\\data_compression\\test2.bin";

    int orignalDataSize = LoadFile(input_file_name, originalData);

    int compressedDataSize = rle_compress((uint8*)originalData, orignalDataSize, (uint8*)compressionBuffer);

    WriteFile(output_file_name, compressionBuffer, compressedDataSize);

    int decompressedDataSize = rle_decompress((uint8*)compressionBuffer, compressedDataSize, (uint8*)decompressionBuffer);

    WriteFile(output_file_name_docmpressed, decompressionBuffer, decompressedDataSize);


    return 0;
}
