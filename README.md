# 4th-year-SHA-256-Algorithm
This repository contains a project completed for GMIT's Theory of Algorithms module. It involves writing a program in the C programming language to perform the Secure Hash Algorithm (SHA) algorithm , speciﬁcally the 256-bit version known as SHA-256. The implementation is a single C file thats calculates the **SHA-256** checksum of an input. The algorithm is based on the Secure Hash Standard document supplied by the  [National Institute of Standard and Technology](https://www.nist.gov/publications/secure-hash-standard).

-----

## About the SHA-256 standard
_This standard specifies hash algorithms that can be used to generate digests of messages. The digests are used to detect whether messages have been changed since the digests were generated_. The table below presents the basic properties of the algorithm.

| Algorithm | Message Size (bits) | Block Size (bits) | Word Size (bits) | Message Digest Size (bits) |
| --------- | ------------------- | ----------------- | ---------------- | -------------------------- |
| SHA-256   | <2<sup>64</sup>     | 512               | 42               | 256                        |


### Preprocessing
Before SHA-256 can be performed on an input some preproccesing is required. The steps are outlined in _Section 5_ of the [Processing Standards Publication 180-4](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf).
1. Padding the message (_Section 5.1_).
2. Parse message into message blocks (_Section 5.2_).
3. Set initial hash value (_Section 5.3_).

### Padding the message (To become a 512 bit padded message block)
- Append "1" bit to  the end.
- Add enough "0" bits so left with 64 bits at the end.
- In remaining 64 encode length(nobits) of message in binary big-endian.

### Steps
- Read 64bytes at a time from file to our message block.
- If last fread < 56 bytes put all padding into last message block (add a "1" bit and 7 "0" bits).
- If we dont have enough bytes left at the end of the block:
  - Create new message block.
  - Only contains padding.
- If file was exactly 512 bits:
  - Create another message block first bit is "1"
  - Then "0" bits 
  - Then last 64 bits are the number of bits in original file.
----

### SHA-256 Hash Computation 
Each Message block _M<sub>(1)</sub>, M<sub>(2)</sub>,.... M<sub>(N)</sub>_ is processed in order using the steps defined below:
1. Prepare message schedule _W<sup>(t)</sup>_ 
2. Iniitialze the eight working variables _a, b, c, d, e, f, g, h_ with their specified hash value
3. For t=0 to 63  create new values for working variables.
4. Compute the _i<sup>th</sup>_ intermediate hash value _H<sup>(i)</sup>_:

After repeating steps one to four _N_ times the resulting 256-bit message digest of _M_ is _H<sub>(0)</sub><sup>(N)</sup>_, _H<sub>(1)</sub><sup>(N)</sub>_,......_H<sub>(7)</sub><sup>(N)</sup>_.



## Prerequisites
The only requirement for this program is a C compiler. There is 2 ways to do so
1. Install a C compiler on the specified machine.
2. Use the online service available at [onlinegdb](https://www.onlinegdb.com/online_c_compiler) to complie C file.

-----

## Running the program

### Download

Clone this reposiory to your machine.
- Navigate to directory
- In command prompt 
```bash
	> git clone https://github.com/kbarry91/4th-year-SHA-256-Algorithm.git
```
### Compile the program

Navigate to the downloaded repository and enter	:
```bash
	> gcc -o sha-256 sha-256.c
```	
This will compile the program and add a ``sha-256`` executable to the directory.

### Execute the program
The program has been designed to work in 3 different ways:
1. Enter file as a command line arguement. 
2. Enter the filename as a string at runtime.
3. Enter a string to be hashed at runtime.

#### Command line arguemnt
To hash a file from command line enter the executable and the file to be hashed.
```bash
> ./sha256 filename.txt
```

<p align="center">
  <img src = "https://i.imgur.com/Pi8xTr9.png/">
</p>

#### Runtime (File input)
The algorithm has be designed to check if a file was entered as an arguement. If not you will be given the option to slect **1** enter the file when the program starts. Simply enter the path and filename. To run the program:
```bash 
> ./sha256 
```
<p align="center">
  <img src = "https://i.imgur.com/lqAhnvm.png/">
</p>

#### Runtime (String Input)
The program allows for a user to enter a string to compute the checksum. Simply select option *2* at the main menu and enter the string. To run the program:
```bash 
> ./sha256 
```

<p align="center">
  <img src = "https://i.imgur.com/Uum7y5k.png/">
</p>

-----

## Testing
The algorithm was tested using the test vectors approved by the [National Institute Of Standards](https://www.nist.gov/publications/secure-hash-standard) available at [DI Management](https://www.di-mgt.com.au/sha_testvectors.html). In other to verify the results each checksum was compared with the results got from 2 other resources.
1. [sha256_checksum](https://emn178.github.io/online-tools/sha256_checksum.html).
2. [onlinemd5](http://onlinemd5.com/).

To run the following tests the corresponding test files have been added to the [test-files](https://github.com/kbarry91/4th-year-SHA-256-Algorithm/blob/master/test-files) folder. When testing via command line arguement or at runtime the files can be referenced by `test-files/test1.txt`
### Test 1
| Input | Expected Result                                                         | Actual Result                                                           | PASS/FAIL |
| ----- | ----------------------------------------------------------------------- | ----------------------------------------------------------------------- | --------- |
| abc   | ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad | ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad | **PASS**  |

### Test 2
| Input           | Expected Result                                                         | Actual Result                                                           | PASS/FAIL |
| --------------- | ----------------------------------------------------------------------- | ----------------------------------------------------------------------- | --------- |
| empty string "" | e3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855 | e3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855 | **PASS**  |

### Test 3
| Input                                                    | Expected Result                                                         | Actual Result                                                           | PASS/FAIL |
| -------------------------------------------------------- | ----------------------------------------------------------------------- | ----------------------------------------------------------------------- | --------- |
| abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq | 248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1 | 248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1 | **PASS**  |

### Test 4
| Input                        | Expected Result                                                  | Actual Result                                                           | PASS/FAIL |
| ---------------------------- | ---------------------------------------------------------------- | ----------------------------------------------------------------------- | --------- |
| testing on file **sha256.c** | 0eb382a00674c80ec15b64799bf57ec38aa331b5eca0d0e3231311230f6ac31f | 38eca61c 78c8f046 899dd682 e2a0733f dfdb82c5 1de696cc 8d08eb5c 4ce1912f | **PASS**  |

-----

## Features of the implementation
### File Checking
A file must be succesfully opened in order to run the program. This is simply achieved by using ``fopen()`` and a check for ``NULL``. If a file isn't provided as an argument the user can enter a file name and then that file will be checked. 
```cs
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
### File Writing
After successfully processing the SHA-256 of an input the checksum is saved to a new file in a folder called ```saved-hashes```. As the file entered may contain a file path and an extension, using the libary ```<libgen.h>``` the path and extension are removed from the file. The new file name is appended with _.txt_ and then appended to the path _saved-hashes/_ to save the file.

### User Input
The program allows the user to enter a string to generate its checksum. This is done by saving the string to a file at ```test-files/userinput.txt"```. Once saved the sha 256 is calculated as normal.

### Endian Check
Little and big endian are two ways of storing multibyte data-types ( int, float, etc). In little endian machines, last byte of binary representation of the multibyte data-type is stored first. On the other hand, in big endian machines, first byte of binary representation of the multibyte data-type is stored first.

To check if a machine uses big-endian or little-endian the following macro was used :

```c
	#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
```

The above code illustrates the comparison between an integer being compared against a cast character string  integer.

### Convert little-endian to big-endian

```c
	#define CONVERT_UINT32(x) (((x) >> 24) | (((x)&0x00FF0000) >> 8) | (((x)&0x0000FF00) << 8) | ((x) << 24))
```

### Convert big-endian to little-endian
```c
	#define CONVERT_UINT64(x) 
	((((x) >> 56) & 0x00000000000000FF) | (((x) >> 40) & 0x000000000000FF00) | 
	 (((x) >> 24) & 0x0000000000FF0000) | (((x) >> 8 ) & 0x00000000FF000000) |  
	 (((x) << 8 ) & 0x000000FF00000000) | (((x) << 24) & 0x0000FF0000000000) |  
	 (((x) << 40) & 0x00FF000000000000) | (((x) << 56) & 0xFF00000000000000))
```

## References
In order to complete this project alot of research went into both the **SHA-256** algorithm aswell as the **C** programming language. Any code adapted from outside sources is refernced in the [sha256.c](https://github.com/kbarry91/4th-year-SHA-256-Algorithm/blob/master/sha256.c) file and complies with all licenses and [^policies] .

Below is as list of some other resources used to conduct research:
- [ch maj](https://crypto.stackexchange.com/questions/5358/what-does-maj-and-ch-mean-in-sha-256-algorithm/5360)
- [endian conversion](//http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/)
- [endian check](https://www.reddit.com/r/C_Programming/comments/2wji9z/endianness_bugs/)
- [National Institute of Standard and Technology](https://www.nist.gov/publications/secure-hash-standard)
- [DI Management](https://www.di-mgt.com.au/sha_testvectors.html)
- [Binary representations](https://www.geeksforgeeks.org/little-and-big-endian-mystery/)
- [SHA Standard](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
- [String Manipulation](https://linux.die.net/man/3/basename)


[^policies] : This project complies with the Quality Assurance Framework at GMIT which includes the Code of Student Conduct and the Policy on Plagiarism.