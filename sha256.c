// Author: 		Kevin Barry
// Module: 		Theory Of Algorithms
// Description:	SHA-256 as defined at https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

#include <stdio.h>
#include <stdint.h>

void sha256();

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

int main(int argc, char *argv[])
{
	printf("Hello World");
	sha256();

	return 0;
}

void sha256()
{

	// Message Schedule (Section 6.2).
	uint32_t W[64];
	// Working variables (Section 6.2).
	uint32_t a, b, c, d, e, f, g, h;
	// Two temporary variables (Section 6.2).
	uint32_t T1, T2;

	// The hash value (section 6.2).
	// The values come from section 5.3.3
	uint32_t H[8] = {
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19};

	// The current message block.
	uint32_t M[16];

	// ================================ Hash Computation ================================
	// Each meessage block is processed in order.

	int t;

	// ================================ Step 1
	// Page 22, W[t]= M[t] for 0 <= t <= 15.
	// W is word in message.
	for (t = 0; t < 16; t++)
	{
		W[t] = M[t];
	}

	// Page 22, W[t]= ...
	for (t = 16; t < 64; t++)
	{
		W[t] = sig1(W[t - 2]) + W[t - 7] + sig0(W[t - 15]) + W[t - 16];
	}

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