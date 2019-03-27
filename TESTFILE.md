# 4th-year-SHA-256-Algorithm
## References
- (ch maj)[https://crypto.stackexchange.com/questions/5358/what-does-maj-and-ch-mean-in-sha-256-algorithm/5360]
- (endian checks)[//http://www.firmcodes.com/write-c-program-convert-little-endian-big-endian-integer/]
  
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

## Testing
