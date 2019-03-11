// Author: 		Kevin Barry
// Module: 		Theory Of Algorithms
// Description:	SHA-256 as defined at https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
// Padding as described in section 5.1 and 5.2 of the sha 256 standard
#include <stdio.h>
#include <stdint.h>

/**
 * x to access as 2 8 bit integers.
 * y to access as a 16 bit integer.
 */
union point {
    uint8_t x[64];  	// 8*64 = 512 bits
    uint32_t y[16];     // 8*16 = 128 bits
    uint64_t s[8]       // 64*8 = 512 bits
};

int main(int argc, char *argv[]){
    FILE* file ;
    char c;

    // Open the file.
    file = fopen(argv[1],"r");
    // TODO add check file success

    printf("%c\n",fread(&c,1,1,file));


    // Close the file.
    fclose(file);


    return 0;
}