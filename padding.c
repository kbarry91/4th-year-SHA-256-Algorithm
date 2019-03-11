// Author: 		Kevin Barry
// Module: 		Theory Of Algorithms
// Description:	SHA-256 as defined at https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
// Padding as described in section 5.1 and 5.2 of the sha 256 standard
#include <stdio.h>
#include <stdint.h>

/**
 * x to access as 2 8 bit integers.
 * y to access as a 16 bit integer.
 * 
 * This union allows every element to be saved in same memory location.
 */
union msgblock {
    uint8_t     e[64];  // 8*64 = 512 bits
    uint32_t    t[16]; // 8*16 = 128 bits
    uint64_t    s[8];   // 64*8 = 512 bits
};

int main(int argc, char *argv[])
{
    // Define union message block instance.
    union msgblock M;

    // Used in future 
    uint64_t nobytes;

    // File pointer
    FILE* file;
    char c;

    // Open the file.
    file = fopen(argv[1], "r");
    // TODO add check file success!!!!!

    // TODO must adaptdeal with error f.error!!!!!!
    while (!feof(file))
    {
        nobytes = fread(M.e, 1, 64, file);

        // Number of bytes that have been read.
        printf("%11u\n", nobytes);
    }

    // Close the file.
    fclose(file);

    return 0;
}