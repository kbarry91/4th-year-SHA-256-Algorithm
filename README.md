# 4th-year-SHA-256-Algorithm
## References
- [ch maj](https://crypto.stackexchange.com/questions/5358/what-does-maj-and-ch-mean-in-sha-256-algorithm/5360)
- [endian conversion](//http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/)
- [endian check](https://www.reddit.com/r/C_Programming/comments/2wji9z/endianness_bugs/)
## Preprocessing
- padding the meesage
- parsing msg into msg blocks
- setting initial hash value

### Padding the message(To become a 51 bit padded message)
- apepend "1" bit to end
- add enough "0"s so left with 64 bits at the end
- In remaining 64 encode length of msg in binary big indian

## Steps
- Read 64bytes at a time from file to our message block
- if last fread <56 bytes put atll padding into last message block(add a 1bit and 7 0 bits)
- if we dont have 9bytes left at end of file
  - new essage block
  - only contains padding
-  if file was exactly 512 bits
   -  anothe message block first bit is 1,
   -   then 0s 
   -   then last 64 are num bits in origonal file

## File Checking
A file must be succesfully opened in order to run the program. This is simply achieved by using ``fopen()`` and a check for ``NULL``.
```
	FILE *file;

	// Open the file from first cmdline argument.
	file = fopen(argv[1], "r");

	// Confirm check on file
	if (file == NULL)
	{
		printf("[ERROR]: Could not open file.");
	}
	else
	{
		// Run Secure Hash Algorithim on the file.
		printf("[FILE READ SUCCESS]: Now running sha256");
		sha256(file);
	}
```
## Testing
Tested sha256 using a copy of README called TESTFILE.md.
In other to verify the resulys each checksum was compared with the results got from 2 resources
1. [sha256_checksum](https://emn178.github.io/online-tools/sha256_checksum.html).
2. [onlinemd5](http://onlinemd5.com/).

### Test 1
printing as : 	
```
printf("%08x %08x %08x %08x %08x %08x %08x %08x \n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
```

| Expected Result    | Actual Result    | PASS/FAIL |
| --------|---------|-------|
| 0eb382a00674c80ec15b64799bf57ec38aa331b5eca0d0e3231311230f6ac31f  |  38eca61c 78c8f046 899dd682 e2a0733f dfdb82c5 1de696cc 8d08eb5c 4ce1912f   | **FAIL**  |

### Test 2
printing as : 
```
printf("%x %x %x %x %x %x %x %x\n", SWAP_UINT32(H[0]), SWAP_UINT32(H[1]), SWAP_UINT32(H[2]),
		   SWAP_UINT32(H[3]), SWAP_UINT32(H[4]), SWAP_UINT32(H[5]), SWAP_UINT32(H[6]), SWAP_UINT32(H[7]));
```

| Expected Result    | Actual Result    | PASS/FAIL |
| --------|---------|-------|
| 0eb382a00674c80ec15b64799bf57ec38aa331b5eca0d0e3231311230f6ac31f  | 1ca6ec38 46f0c878 82d69d89 3f73a0e2 c582dbdf cc96e61d 5ceb088d 2f91e14c   | **FAIL**   |

### Test 3
printing as : 
```
printf("%08x %08x %08x %08x %08x %08x %08x %08x \n", SWAP_UINT32(H[0]), SWAP_UINT32(H[1]), SWAP_UINT32(H[2]),
		   SWAP_UINT32(H[3]), SWAP_UINT32(H[4]), SWAP_UINT32(H[5]), SWAP_UINT32(H[6]), SWAP_UINT32(H[7]));
```


| Expected Result    | Actual Result    | PASS/FAIL |
| --------|---------|-------|
| 0eb382a00674c80ec15b64799bf57ec38aa331b5eca0d0e3231311230f6ac31f  | 1ca6ec38 46f0c878 82d69d89 3f73a0e2 c582dbdf cc96e61d 5ceb088d 2f91e14c   | **FAIL** _(No difference to test 2 )_  |

### Test 4
Changed algoritihm to 

```
unsigned int LitToBigEndian(unsigned int x)
{
	return (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000));
}

```



| Expected Result    | Actual Result    | PASS/FAIL |
| --------|---------|-------|
| 0eb382a00674c80ec15b64799bf57ec38aa331b5eca0d0e3231311230f6ac31f  | 1ca6ec38 46f0c878 82d69d89 3f73a0e2 c582dbdf cc96e61d 5ceb088d 2f91e14c   | **FAIL** _(No difference to test 2 or 3) _ |