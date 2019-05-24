
#ifndef HASH_H
#define HASH_H

#include <stdlib.h>

/** Output length for `wally_hash160` */
#define HASH160_LEN 20

int wally_hash160(const unsigned char *bytes, size_t bytes_len,
                  unsigned char *bytes_out, size_t len);

#endif /* HASH_H */
