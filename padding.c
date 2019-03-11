// Author: 		Kevin Barry
// Module: 		Theory Of Algorithms
// Description:	SHA-256 as defined at https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

#include <stdio.h>
#include <stdint.h>

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