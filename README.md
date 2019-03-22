# 4th-year-SHA-256-Algorithm
## References
- (ch maj)[https://crypto.stackexchange.com/questions/5358/what-does-maj-and-ch-mean-in-sha-256-algorithm/5360]
  
## Preprocessing
- padding the meesage
- parsing msg into msg blocks
- setting initial hash value

### Padding the message(To become a 51 bit padded message)
- apepend "1" bit to end
- add enough "0"s so left with 64 bits at the end
- In remaining 64 encode length of msg in binary big indian
