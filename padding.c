// Author: 		Kevin Barry
// Module: 		Theory Of Algorithms
// Description:	SHA-256 as defined at https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
// Padding the Messageas described in section 5.1 and 5.1.1 of the sha 256 standard
#include <stdio.h>
#include <stdint.h>

/**
 * e to access as 8 bit integers.
 * t to access as 32 bit integer.
 * s to access as 64 bit integer.
 * 
 * This union allows every element to be saved in same memory location.
 * Values are the same but being read differently.
 * 
 */
union msgblock {
    uint8_t e[64];  // 8*64 = 512 bits
    uint32_t t[16]; // 32*16 = 512 bits
    uint64_t s[8];  // 64*8 = 512 bits
};

// Flags to track state of padding status.stores(0,1,2,3)
enum status
{
    READ,
    PAD0,
    PAD1,
    FINISH
};

int main(int argc, char *argv[])
{
    // Define union message block instance.
    union msgblock M;

    // Current number of bytes read from fread.
    uint64_t nobytes;

    // Current number of bits read.
    uint64_t nobits = 0;

    enum status S = READ; // set status to read

    // File pointer
    FILE* file;
    char c;
    int i;
    // Open the file.
    file = fopen(argv[1], "r");
    // TODO add check file success!!!!!

    // TODO must adaptdeal with error f.error!!!!!!
    // As of section 5.2.1
    while (S == READ)
    {
        nobytes = fread(M.e, 1, 64, file);
        printf("Read %2llu bytes\n", nobytes);
        // Add 8 bits for each byte;
        nobits = nobits + (nobytes * 8);

        
        // Check if less than 56 bytes.
        if (nobytes < 56)
        {
            printf("Block found with less than 55 bytes !\n");

            // Change to one byte.
            // M.e[nobytes]= 0x01; // 00000001
            M.e[nobytes] = 0x80; // 00000001

            while (nobytes < 56)
            {
                nobytes = nobytes + 1; // Add one as index into block.((actually loop var))
                M.e[nobytes] = 0x00;   // Set all bytes to 0.
            }
            // Set last 4 bytes as 64 biit integer as number of bits read from file.

            // Set last element to nobits(number of bits in origonal msg).
            M.s[7] = nobits; // TODO  make sure its big indian integer
            // set to finish stat
            S = FINISH;
        }
        // If 56 to 64 bytes read we have to have an extra message block full of padding
        // cannot append "1" and add a 64 bit integer to the original message block
        else if (nobytes < 64)
        {
            S = PAD0;            // need another message block
            M.e[nobytes] = 0x80; // append 1

            // add some padding to the current message block
            while (nobytes < 64)
            {
                nobytes = nobytes + 1;
                M.e[nobytes] = 0x00; // fill block with "0"
            }                        // end while
        }
        // If I have finished reading everything from the file and it was exactly 512 bits in length
        // i.e I kept reading 64 bytes from the file & the last time I read from the file
        // I read 64 bytes and it brought me to the exact end of the file
        else if (feof(file))
        {
            S = PAD1;
        }
    } // read while

    // If status is PAD0 or PAD1 add block padding where first 448 bits are 0's
    // in the last block, put in the 64 bit big endian integer
    // which represents the number of bits in the original message
    if (S == PAD0 || S == PAD1)
    {
        for (i = 0; i < 56; i++)
            M.e[i] = 0x00;
        M.s[7] = nobits;
    }
    if (S == PAD1)
    {
        M.e[0] = 0x80;// add a "1"
    }
    // Close the file.
    fclose(file);

    for (int i = 0; i < 64; i++)
    {
        // Print elements of M as 64 individual bytes.
        printf("%x ", M.e[i]);
    }
    printf("\n");

    return 0;
}