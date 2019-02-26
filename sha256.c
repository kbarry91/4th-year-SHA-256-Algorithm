// Author: 		Kevin Barry
// Module: 		Theory Of Algorithms
// Description:	SHA-256

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
		sig1(W[t - 2]) + W[t - 7] + sig0(W[t - 15]) + W[t - 16];
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
// rotate right
// See section 3.2
uint32_t rotr(uint32_t n, uint32_t x)
{
	// shove x down n places or x up 32 -n places.
	return (x >> n) | (x << (32 - n));
}

// shift right
uint32_t shr(uint32_t n, uint32_t x)
{
	// Shove x down n spots.
	return (x >> n);
}

uint32_t sig0(uint32_t x)
{
	// See section 3.2 and 4.1.2 for definitions
	return (rotr(7, x) ^ rotr(18, x) ^ shr(3, x));
}

uint32_t sig1(uint32_t x)
{
	// See section 3.2 and 4.1.2 for definitions
	return (rotr(17, x) ^ rotr(19, x) ^ shr(10, x));
}