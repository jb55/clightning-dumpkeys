
#include "ripemd160.h"
#include "sha256.h"
#include "compiler.h"
#include "hash.h"

int hash160(const unsigned char *bytes, size_t bytes_len,
                  unsigned char *bytes_out, size_t len)
{
	struct sha256 sha;
	struct ripemd160 ripemd;
	bool aligned = alignment_ok(bytes_out, sizeof(ripemd.u.u32));

	if (!bytes || !bytes_out || len != HASH160_LEN)
		return 0;

	BUILD_ASSERT(sizeof(ripemd) == HASH160_LEN);

	sha256(&sha, bytes, bytes_len);
	ripemd160(aligned ? (struct ripemd160 *)bytes_out : &ripemd, &sha, sizeof(sha));
	if (!aligned) {
		memcpy(bytes_out, &ripemd, sizeof(ripemd));
		memclear(&ripemd, sizeof(ripemd));
	}
	memclear(&sha, sizeof(sha));
	return 1;
}

