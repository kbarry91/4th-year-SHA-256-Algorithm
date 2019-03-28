// Author: 		Kevin Barry
// Module: 		Theory Of Algorithms
// Description:	SHA-256 as defined at https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
// Tested using : https://emn178.github.io/online-tools/sha256_checksum.html
#include <stdio.h>  // IO header file.
#include <stdint.h> // For fixed bit length integers.

/**
 * e to access as 8 bit integers.
 * t to access as 32 bit integer.
 * s to access as 64 bit integer.
 * 
 * This union represnts message block and  allows every element to be saved in same memory location.
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
	PAD1, // message was multiple of 64 bytes
	FINISH
};

// See section 4.1.2
// declaration of sigma
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

// See section 3.2.
uint32_t rotr(uint32_t n, uint32_t x);
uint32_t shr(uint32_t n, uint32_t x);

// See section 4.1.2
uint32_t SIG0(uint32_t x);
uint32_t SIG1(uint32_t x);

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);

// Retrieve next message block
// pass in file
// pointer to msgblock
// pointer to status
// number of bits
int nextmsgblock(FILE *file, union msgblock *M, enum status *S, uint64_t *nobits);

// Adapted from - http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/
#define SWAP_UINT32(x) (((x) >> 24) | (((x)&0x00FF0000) >> 8) | (((x)&0x0000FF00) << 8) | ((x) << 24))
#define littleToBigEndian(x) (((x >> 24) & 0x000000ff) | ((x >> 8) & 0x0000ff00) | ((x << 8) & 0x00ff0000) | ((x << 24) & 0xff000000));

//https://www.reddit.com/r/C_Programming/comments/2wji9z/endianness_bugs/
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
unsigned int LitToBigEndian(unsigned int x);
unsigned int LitToBigEndian(unsigned int x)
{
	return (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000));
}
// Calculate the SHA256 hash of a file
void sha256(FILE *file);

// ================================ Main Method ================================

/*
* The main method.
*/
int main(int argc, char *argv[])
{
	// File pointer
	FILE *file;

	// Open the file from first cmdline argument.
	//.================ TODO ERROR CHECK
	file = fopen(argv[1], "r");

	printf("========= Secure Hash Algorithim ========= \n");
	// Run Secure Hash Algorithim on the file.
	sha256(file);

	return 0;
}
// ================================ SHA 256 Hash Computation ================================

/**
 * Sha256
*/
void sha256(FILE *file)
{
	// Define union  current message block instance.
	union msgblock M;

	// Current number of bits read from file.
	uint64_t nobits = 0;

	// set status to read() status of message block in terms of padding
	enum status S = READ;
	// TODO add check file success!!!!!

	/*
 * The K constants.
 * SHA-256 use the same sequence of sixty-four constant 32-bit words, 
 * These words represent the first thirty-two bits of the fractional 
 * parts of the cube roots of the first sixty-four prime numbers.
 */
	const uint32_t K[] = {
		0x248a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

	// Message Schedule (Section 6.2).
	uint32_t W[64];
	// Working variables (Section 6.2).
	uint32_t a, b, c, d, e, f, g, h;
	// Two temporary variables (Section 6.2).
	uint32_t T1, T2;

	// The hash value (section 6.2).
	// The values come from section 5.3.3
	// SHOULD MALLOC THIS (SO ITS ON HEAP)
	uint32_t H[8] = {
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19};

	// ================================ Hash Computation ================================
	// Each meessage block is processed in order.

	// Loop variables.
	int i, t;

	// Iterate through message blocks as per page 22.
	while (nextmsgblock(file, &M, &S, &nobits))
	//	for (i = 0; i < 1; i++)
	{

		// ================================ Step 1
		// Page 22, W[t]= M[t] for 0 <= t <= 15.
		// W is word in message.
		for (t = 0; t < 16; t++)
			W[t] = M.t[t]; // Copy M to W

		// Page 22, W[t]= ...
		for (t = 16; t < 64; t++)
			W[t] = sig1(W[t - 2]) + W[t - 7] + sig0(W[t - 15]) + W[t - 16];

		// ================================ Step 2
		// Initialize a,b,c, ... ,h as per step 2, Page 22.
		// Initialize the eight working variables, a, b, c, d, e, f, g,  and h, with the (i-1)st hash value:
		a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		d = H[4];
		e = H[5];
		f = H[6];
		f = H[7];

		// ================================ Step 3.
		// create new values for working variables.
		for (t = 0; t < 64; t++)
		{
			T1 = h + SIG1(e) + Ch(e, f, g) + K[t] + W[t];
			T2 = SIG0(a) + Maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		// ================================ Step 4.
		// Compute the ith intermediate hash value H(i):
		H[0] = a + H[0];
		H[1] = b + H[1];
		H[2] = c + H[2];
		H[3] = d + H[3];
		H[4] = e + H[4];
		H[5] = f + H[5];
		H[6] = g + H[6];
		H[7] = h + H[7];

	} // End of while loop.

	// ================================ DEBUG AND TESTING ================================

	// Testing pprinting has value

	printf("%08x %08x %08x %08x %08x %08x %08x %08x \n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);

	// printing hash as big indian
	printf("%x %x %x %x %x %x %x %x \n", SWAP_UINT32(H[0]), SWAP_UINT32(H[1]), SWAP_UINT32(H[2]),
		   SWAP_UINT32(H[3]), SWAP_UINT32(H[4]), SWAP_UINT32(H[5]), SWAP_UINT32(H[6]), SWAP_UINT32(H[7]));

	// printing hash as big indian
	printf("%08x %08x %08x %08x %08x %08x %08x %08x \n", SWAP_UINT32(H[0]), SWAP_UINT32(H[1]), SWAP_UINT32(H[2]),
		   SWAP_UINT32(H[3]), SWAP_UINT32(H[4]), SWAP_UINT32(H[5]), SWAP_UINT32(H[6]), SWAP_UINT32(H[7]));

	// Must check if machine is big endian or little endian
	if (IS_BIG_ENDIAN)
	{
		printf("Is Big Endian: ");
		printf("%08x %08x %08x %08x %08x %08x %08x %08x \n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
	}
	else
	{
		printf("Is NOT Big Endian: ");
		//	printf("%08x %08x %08x %08x %08x %08x %08x %08x \n", SWAP_UINT32(H[0]), SWAP_UINT32(H[1]), SWAP_UINT32(H[2]),
		//  SWAP_UINT32(H[3]), SWAP_UINT32(H[4]), SWAP_UINT32(H[5]), SWAP_UINT32(H[6]), SWAP_UINT32(H[7]));
		printf("%08x %08x %08x %08x %08x %08x %08x %08x \n", LitToBigEndian(H[0]), LitToBigEndian(H[1]), LitToBigEndian(H[2]),LitToBigEndian(H[3]), LitToBigEndian(H[4]), LitToBigEndian(H[5]), LitToBigEndian(H[6]), LitToBigEndian(H[7]));
	}
} // void sha256()

// ================================ Bit operations ================================

/**
 * Rotate right
 * Rotate x rigth by n places. Place overhanging bits back to begining.
 * See section 3.2
 */
uint32_t rotr(uint32_t n, uint32_t x)
{
	return (x >> n) | (x << (32 - n));
}

/**
 * Shift right
 * Shift x n positions.
 */
uint32_t shr(uint32_t n, uint32_t x)
{
	return (x >> n);
}

/**
 * sig0
 * Rotate right with 7 XOR it with rotate right 18 and XOR it with shift right 3.
 */
uint32_t sig0(uint32_t x)
{
	// See section 3.2 and 4.1.2 for definitions
	return (rotr(7, x) ^ rotr(18, x) ^ shr(3, x));
}

/**
 * sig1
 * Rotate right by 17 XOR it with rotate right by 19 and XOR it with shift right by 10.
 */
uint32_t sig1(uint32_t x)
{
	// See section 3.2 and 4.1.2 for definitions
	return (rotr(17, x) ^ rotr(19, x) ^ shr(10, x));
}

/**
 * SIG0
 * Rotate right by 2 XOR it with rotate right by 13 and XOR it with rotate right by 22.
 */
uint32_t SIG0(uint32_t x)
{
	return (rotr(2, x) ^ rotr(13, x) ^ rotr(22, x));
}

/**
 * SIG1
 * Rotate right by 6 XOR it with rotate right by 11 and XOR it with shift right by 10.
 */
uint32_t SIG1(uint32_t x)
{
	return (rotr(6, x) ^ rotr(11, x) ^ (25, x));
}

/**
 * Ch - Choose
 * x input chooses if the output is from y or from z.
 * For each bit index, that result bit is according to the bit from y (or respectively z ) at this index, 
 * depending on if the bit from x at this index is 1 (or respectively 0).
 */
uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) ^ ((!x) & z));
}

/**
 * Maj - Majority
 * for each bit index, that result bit is according to the majority of the 3 inputs bits 
 *  for x y and z at this index.
 */
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z)
{
	return ((x & y) ^ (x & z) ^ (y & z));
}

// ================================ NEXT MESSAGE BLOCK ================================

/**
 * Retrieve next message block
 * pass in file
 * pointer to msgblock
 * pointer to status
 * number of bits
 */
int nextmsgblock(FILE *file, union msgblock *M, enum status *S, uint64_t *nobits)
{

	// Current number of bytes read(returnes) from fread.
	uint64_t nobytes;

	// loop variable.
	int i;

	// Check if we have finished all msgblocks then S == Finish.
	if (*S == FINISH)
		return 0;

	// If status is PAD0 or PAD1 add block padding where first 448 bits are 0's
	// in the last block, put in the 64 bit big endian integer
	// which represents the number of bits in the original message

	// IANS COMMENT - otherwise,check if we another block of padding.
	if (*S == PAD0 || *S == PAD1)
	{
		// Set first 56 bytes to all zero bits.
		for (i = 0; i < 56; i++)
		{
			M->e[i] = 0x00;
			printf("[DEBUG set bytes to 0  \n");
		}
		// Set the last 64 bits to th number of bits in the file( should be big-endian)
		M->s[7] = *nobits;
		// set state to finish
		*S = FINISH;

		//printf("[DEBUG x](S == PAD0 || S == PAD1)");
		//printf("[DEBUG llu]nobits %llu", nobits);
		//printf("[DEBUG x]nobits %x", nobits);

		// If S is PAD1, then set first bit of M to one.
		if (*S == PAD1)
		{
			M->e[0] = 0x80; // add a "1"
		}
		// Keep the loop in sha256 going for one more iteration.
		return 1;
	}

	// IF we enter here,we havnt finsihed reading the file (S == READ)
	nobytes = fread(M->e, 1, 64, file); // read 64 bytes at a time from file into M->e.
	//printf("Read %2llu bytes\n", nobytes);

	// Add 8 bits for each byte;
	// Keep track of number of bytes read.
	*nobits += (nobytes * 8);

	// Check if less than 56 bytes have been read.
	// If so put all padding in this message block.
	if (nobytes < 56)
	{
		//printf("Block found with less than 55 bytes !\n");

		// Change to one byte.
		// Add one bit as per the standard.
		M->e[nobytes] = 0x80; // 00000001

		// Add 0 bits until last 64bits.
		while (nobytes < 56)
		{
			nobytes = nobytes + 1; // Add one as index into block.((actually loop var))
			M->e[nobytes] = 0x00;  // Set all bytes to 0.
			printf("[DEBUG set bytes to 0  \n");
		}
		// Set last 4 bytes as 64 biit integer as number of bits read from file.

		// Set last element to nobits(number of bits in origonal msg).
		// Append the file size in bits as a (should be big-endian) unsigned 64bit int.
		M->s[7] = *nobits; // TODO  make sure its big indian integer

		/*
            printf("[DEBUG todollu]nobits %2llu \n",nobits);
            printf("[DEBUG tdodx]nobits %x \n",nobits);
            printf("[DEBUG d]nobits %d \n",nobits);
            printf("[Debug noobitsinhex] %016llX\n", nobits);
            */
		// Set6 S to FINISH state.
		*S = FINISH;
	}
	// If 56 to 64 bytes read we have to have an extra message block full of padding
	// cannot append "1" and add a 64 bit integer to the original message block
	// IANS COMMENT- otherwise check if we can put some padding into this msgblock.
	else if (nobytes < 64)
	{
		// Tell S we need another messageblock with padding but no 1 bit
		*S = PAD0; // need another message block
		// Put the 1 bit into the current block.
		M->e[nobytes] = 0x80; // append 1

		// add some padding to the current message block
		// IANS COMMENT - pad rest of block with 0 bits
		while (nobytes < 64)
		{
			nobytes = nobytes + 1;
			M->e[nobytes] = 0x00; // fill block with "0"
								  //printf("[DEBUG set bytes to 0  \n");

		} // end while
	}
	// If I have finished reading everything from the file and it was exactly 512 bits in length
	// i.e I kept reading 64 bytes from the file & the last time I read from the file
	// I read 64 bytes and it brought me to the exact end of the file
	// IANS COMMENT - otherwise check if we are the the end of file and if there is more to read.
	else if (feof(file))
	{
		// Tell S that we need a message block with all the padding.
		*S = PAD1;
	}

	// DEBUG
	for (int i = 0; i < 64; i++)
	{
		// Print elements of M as 64 individual bytes.
		printf("%x ", M->e[i]);
	}
	printf("\n");

	// If we get this far , then return 1 to call this function again.
	return 1;
}