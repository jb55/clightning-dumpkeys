
#include "short_types.h"
#include <stdlib.h>
#include <stdio.h>

static const char CHECKSUM_CHARSET[] = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

static const char INPUT_CHARSET[] =
	"0123456789()[],'/*abcdefgh@:$%{}"
	"IJKLMNOPQRSTUVWXYZ&+-.;<=>?!^_|~"
	"ijklmnopqrstuvwxyzABCDEFGH`#\"\\ ";

static inline int charset_find(char ch) {
	for (size_t i = 0; i < sizeof(INPUT_CHARSET); i++) {
		if (INPUT_CHARSET[i] == ch)
			return i;
	}
	return -1;
}

static u64 polymod(u64 c, int val)
{
	u8 c0 = c >> 35;
	c = ((c & 0x7ffffffff) << 5) ^ val;
	if (c0 & 1) c ^= 0xf5dee51989;
	if (c0 & 2) c ^= 0xa9fdca3312;
	if (c0 & 4) c ^= 0x1bab10e32d;
	if (c0 & 8) c ^= 0x3706b1677a;
	if (c0 & 16) c ^= 0x644d626ffd;
	return c;
}


int descriptor_checksum(const char *descriptor, int desc_size, char *checksum,
			int checksum_capacity)
{
	if (checksum_capacity < 8+1)
		return 0;

	checksum[0] = 0;

	int j;
	u64 c = 1;
	int cls = 0;
	int clscount = 0;

	for (int i = 0; i < desc_size; i++) {
		char ch = descriptor[i];
		int pos = charset_find(ch);
		if (pos == -1) {
			checksum[0] = 0;
			return 0;
		}
		// Emit a symbol for the position inside the group, for every character.
		c = polymod(c, pos & 31); 

		// Accumulate the group numbers
		cls = cls * 3 + (pos >> 5); 

		if (++clscount == 3) {
			c = polymod(c, cls);
			cls = 0;
			clscount = 0;
		}
	}

	if (clscount > 0)
		c = polymod(c, cls);

	// Shift further to determine the checksum.
	for (j = 0; j < 8; ++j)
		c = polymod(c, 0);

	// Prevent appending zeroes from not affecting the checksum.
	c ^= 1;

	for (j = 0; j < 8; ++j)
		checksum[j] = CHECKSUM_CHARSET[(c >> (5 * (7 - j))) & 31];

	checksum[8] = 0;

	return 8;
}
