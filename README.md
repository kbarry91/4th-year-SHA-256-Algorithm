# 4th-year-SHA-256-Algorithm
This repository contains a project completed for GMIT's Theory of Algorithms module. It involves writing a program in the C programming language to perform the Secure Hash Algorithm (SHA) algorithm , speciﬁcally the 256-bit version known as SHA-256. The implementation is a single C file thats calculates the **SHA-256** checksum of an input.The algorithm is based on the Secure Hash Standard document supplied by the  [National Institute of Standard and Technology](https://www.nist.gov/publications/secure-hash-standard).

-----

## About the SHA-256 standard
_This standard specifies hash algorithms that can be used to generate digests of messages. The digests are used to detect whether messages have been changed since the digests were generated_. The table below presents the basic properties of the algorithm.

| Algorithm | Message Size (bits) | Block Size (bits) | Word Size (bits) | Message Digest Size (bits) |
| --------- | ------------------- | ----------------- | ---------------- | -------------------------- |
| SHA-256   | <2<sup>64</sup>     | 512               | 42               | 256                        |

-----

## Prerequisites
The only requirement for this program is a C compiler. There is 2 ways to do so
1. Install a C compiler on the specified machine.
2. Use the online service available at [onlinegdb](https://www.onlinegdb.com/online_c_compiler) to complie C file.

-----

## Running the program
### Download
Clone this reposiory to your machine.
- Navigate to directory
- in command prompt 
  ```  
	https://github.com/kbarry91/4th-year-SHA-256-Algorithm.git
	```
### Compile the program
Navigate to the downloaded repository and enter	
```
gcc -o SHA-256 SHA-256.c
```	
This will compile the program and add a ``SHA-256`` executable to the directory.

### Execute the program
The program has been designed to work in 2 different ways:
1. Enter file as a command line arguement. 
2. Enter the filename as a string at runtime.

#### Command line arguemnt
To hash a file from command line run enter the exectuable and the file to be hashed.
```
./sha256 text1.txt
```

<p align="center">
  <img src = "https://i.imgur.com/Pi8xTr9.png/">
</p>

#### Runtime
The algorithm has be designed to check if a file was entered as an arguement. If not you will be given the option to enter the file when the program starts. Simply enter the path and filename. To run the program:
```
./sha256 
```
<p align="center">
  <img src = "https://i.imgur.com/lqAhnvm.png/">
</p>


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


-----

## Testing
The algorithm was tested using the test vectors approved by the [National Institute Of Standards](https://www.nist.gov/publications/secure-hash-standard) available at [DI Management](https://www.di-mgt.com.au/sha_testvectors.html). In other to verify the results each checksum was compared with the results got from 2 other resources.
1. [sha256_checksum](https://emn178.github.io/online-tools/sha256_checksum.html).
2. [onlinemd5](http://onlinemd5.com/).

### Test 1
| Input | Expected Result                                                  | Actual Result                                                           | PASS/FAIL |
| -|---------------------------------------------------------------- | ----------------------------------------------------------------------- | --------- |
|abc| ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad | ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad | **PASS**  |

### Test 2
| Input | Expected Result                                                  | Actual Result                                                           | PASS/FAIL |
| -|---------------------------------------------------------------- | ----------------------------------------------------------------------- | --------- |
|empty string ""| e3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855 | e3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855| **PASS**  |

### Test 3
| Input | Expected Result                                                  | Actual Result                                                           | PASS/FAIL |
| -|---------------------------------------------------------------- | ----------------------------------------------------------------------- | --------- |
|abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq| 	248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1 | 	248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1 | **PASS**  |

### Test 4
| Input | Expected Result                                                  | Actual Result                                                           | PASS/FAIL |
| -|---------------------------------------------------------------- | ----------------------------------------------------------------------- | --------- |
|testing on file **sha256.c**| 0eb382a00674c80ec15b64799bf57ec38aa331b5eca0d0e3231311230f6ac31f | 38eca61c 78c8f046 899dd682 e2a0733f dfdb82c5 1de696cc 8d08eb5c 4ce1912f | **PASS**  |

-----

## Features of the implementation
### File Checking
A file must be succesfully opened in order to run the program. This is simply achieved by using ``fopen()`` and a check for ``NULL``. If a file isn't provided as an argument the user can enter a file name and then that file will be checked. 
```
	FILE *file;

	// Check if file was entered as cmd argument.
	if (argv[1] == NULL)
	{
		printf("No file specified as argument.\nPlease enter a file name: ");
		scanf("%s", fileName);
		printf("Searching for %s.....\n",fileName);
		
		file = fopen(fileName, "r");
	}else{
		file = fopen(argv[1], "r");
	}

	// Check if file opened succesfully.
	if (file == NULL)
	{
		printf("[ERROR]: Could not open file.\n");
	}
	else
	{
		// Run Secure Hash Algorithim on the file.
		printf("[FILE READ SUCCESS]: Now running sha256 Hash Computation.....\n");
		sha256(file);
	}

```

### Endian Check
Little and big endian are two ways of storing multibyte data-types ( int, float, etc). In little endian machines, last byte of binary representation of the multibyte data-type is stored first. On the other hand, in big endian machines, first byte of binary representation of the multibyte data-type is stored first.

To check if a machine uses big-endian or little-indian the following macro was used :
```
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
```
The above code illustrates the comparison between an integer being compared against a cast character string  integer.

### Convert little-endian to big-endian
```
#define CONVERT_UINT32(x) (((x) >> 24) | (((x)&0x00FF0000) >> 8) | (((x)&0x0000FF00) << 8) | ((x) << 24))

```
### Convert big-endian to little-endian
```
#define CONVERT_UINT64(x)                                                      
	((((x) >> 56) & 0x00000000000000FF) | (((x) >> 40) & 0x000000000000FF00) | 
	 (((x) >> 24) & 0x0000000000FF0000) | (((x) >> 8 ) & 0x00000000FF000000) |  
	 (((x) << 8 ) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) |  
	 (((x) << 40) & 0x00FF000000000000) | (((x) << 56) & 0xFF00000000000000))
```

## References
- [ch maj](https://crypto.stackexchange.com/questions/5358/what-does-maj-and-ch-mean-in-sha-256-algorithm/5360)
- [endian conversion](//http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/)
- [endian check](https://www.reddit.com/r/C_Programming/comments/2wji9z/endianness_bugs/)
- [National Institute of Standard and Technology](https://www.nist.gov/publications/secure-hash-standard)
- [DI Management](https://www.di-mgt.com.au/sha_testvectors.html)
- [Binary representations](https://www.geeksforgeeks.org/little-and-big-endian-mystery/)
- 