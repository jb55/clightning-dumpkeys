/* from wally_core */

#include "ec.h"
#include <stdbool.h>
#include "short_types.h"
#include "compiler.h"

int wally_ec_public_key_from_private_key(
	const secp256k1_context *ctx,
	const unsigned char *priv_key, size_t priv_key_len,
	unsigned char *bytes_out, size_t len)
{
	secp256k1_pubkey pub;
	size_t len_in_out = EC_PUBLIC_KEY_LEN;
	bool ok;

	if (!ctx)
		return WALLY_ENOMEM;

	ok = priv_key && priv_key_len == EC_PRIVATE_KEY_LEN &&
		bytes_out && len == EC_PUBLIC_KEY_LEN &&
		secp256k1_ec_pubkey_create(ctx, &pub, priv_key) &&
		secp256k1_ec_pubkey_serialize(ctx, bytes_out, &len_in_out, &pub, SECP256K1_EC_COMPRESSED) &&
		len_in_out == EC_PUBLIC_KEY_LEN;

	if (!ok && bytes_out)
		memclear(bytes_out, len);
	memclear(&pub, sizeof(pub));
	return ok ? WALLY_OK : WALLY_EINVAL;
}
