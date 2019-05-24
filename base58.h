
#ifndef LIBWALLY_BASE58_H
#define LIBWALLY_BASE58_H

#include <stdint.h>
#include <stdlib.h>

/**
 * Calculate the base58 checksum of a block of binary data.
 *
 * @bytes: Binary data to calculate the checksum for.
 * @len: The length of @bytes in bytes.
 */
uint32_t base58_get_checksum(
	const unsigned char *bytes,
	size_t len);

int wally_base58_from_bytes(const unsigned char *bytes, size_t bytes_len,
                            uint32_t flags, char **output);

int wally_base58_to_bytes(
	const char *str_in,
	uint32_t flags,
	unsigned char *bytes_out,
	size_t len,
	size_t *written);

#define BASE58_FLAG_CHECKSUM 0x1
#define BASE58_CHECKSUM_LEN 4

#endif /* LIBWALLY_BASE58_H */
