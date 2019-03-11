// Author: 		Kevin Barry
// Module: 		Theory Of Algorithms
// Description:	SHA-256 as defined at https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
// Padding the Messageas described in section 5.1 and 5.1.1 of the sha 256 standard
#include <stdio.h>
#include <stdint.h>

/**
 * x to access as 2 8 bit integers.
 * y to access as a 16 bit integer.
 * 
 * This union allows every element to be saved in same memory location.
 */
union msgblock {
    uint8_t e[64];  // 8*64 = 512 bits
    uint32_t t[16]; // 8*16 = 128 bits
    uint64_t s[8];  // 64*8 = 512 bits
};

int main(int argc, char *argv[])
{
    // Define union message block instance.
    union msgblock M;

    // Current number of bytes read.
    uint64_t nobytes;
    // Current number of bits read.
    uint64_t nobits = 0;

    // File pointer
    FILE *file;
    char c;

    // Open the file.
    file = fopen(argv[1], "r");
    // TODO add check file success!!!!!

    // TODO must adaptdeal with error f.error!!!!!!
    // As of section 5.2.1
    while (!feof(file))
    {
        nobytes = fread(M.e, 1, 64, file);
        // Add 8 bits for each byte;
        nobits = nobits + (nobytes * 8);

        // Check if less than 56 bytes.
        if (nobytes < 56)
        {
            printf("Block found with less than 55 bytes!\n");

            // Change to one byte.
            // M.e[nobytes]= 0x01; // 00000001
            M.e[nobytes] = 0x80; // 00000001

            while (nobytes < 56)
            {
                nobytes = nobytes + 1; // Add one as index into block.
                M.e[nobytes] = 0x00;   // Set all bytes to 0.
            }
            // Set last 4 bytes as 64 biit integer as number of bits read from file.

            // Set last element to nobits.
            M.s[7] = nobits;
        }
        // Number of bytes that have been read from file.
        //printf("%11u\n", nobytes);
    }

    // Close the file.
    fclose(file);
    
    for(int i = 0; i < 64; i++)
    {
        // Print elements of M as 64 individual bytes.
        printf("%x ", M.e[i]);
        printf("\n");
    }
    

    return 0;
}