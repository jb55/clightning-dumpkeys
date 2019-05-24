
#ifndef EC_H
#define EC_H

#include <stdlib.h>
#include "secp256k1.h"

/** The length of a private key used for EC signing */
#define EC_PRIVATE_KEY_LEN 32
/** The length of a public key used for EC signing */
#define EC_PUBLIC_KEY_LEN 33
/** The length of an uncompressed public key */
#define EC_PUBLIC_KEY_UNCOMPRESSED_LEN 65
/** The length of a message hash to EC sign */
#define EC_MESSAGE_HASH_LEN 32
/** The length of a compact signature produced by EC signing */
#define EC_SIGNATURE_LEN 64
/** The maximum encoded length of a DER encoded signature */
#define EC_SIGNATURE_DER_MAX_LEN 72
/** The maximum encoded length of a DER encoded signature created with EC_FLAG_GRIND_R */
#define EC_SIGNATURE_DER_MAX_LOW_R_LEN 71

/** Indicates that a signature using ECDSA/secp256k1 is required */
#define EC_FLAG_ECDSA 0x1
/** Indicates that a signature using EC-Schnorr-SHA256 is required */
#define EC_FLAG_SCHNORR 0x2
/** Indicates that the signature nonce should be incremented until the signature is low-R */
#define EC_FLAG_GRIND_R 0x4

int wally_ec_public_key_from_private_key(
	const secp256k1_context *ctx, const unsigned char *priv_key,
	size_t priv_key_len, unsigned char *bytes_out, size_t len);

#endif /* EC_H */
