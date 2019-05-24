
#include "short_types.h"
#include "hkdf.h"
#include "compiler.h"
#include "secp256k1.h"
#include "bip32.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#define BIP32_ENTROPY_LEN_256 32


#define fatal(fmt, ...) do { fprintf(stderr, fmt "\n", __VA_ARGS__); exit(1); } while (0)
#define fatal1(...) do { fprintf(stderr, __VA_ARGS__); exit(1); } while (0)

/* General 256-bit secret, which must be private.  Used in various places. */
struct secret {
	u8 data[32];
};

static struct {
	struct secret hsm_secret;
	struct ext_key bip32;
} secretstuff;

struct bip32_key_version {
	u32 bip32_pubkey_version;
	u32 bip32_privkey_version;
};



/* Version codes for BIP32 extended keys in libwally-core.
 * It's not suitable to add this struct into client struct,
 * so set it static.*/
static struct  bip32_key_version  bip32_key_version;

bool read_all(int fd, void *data, size_t size)
{
	while (size) {
		ssize_t done;

		done = read(fd, data, size);
		if (done < 0 && errno == EINTR)
			continue;
		if (done <= 0)
			return false;
		data = (char *)data + done;
		size -= done;
	}

	return true;
}



static void populate_secretstuff(void)
{
	u8 bip32_seed[BIP32_ENTROPY_LEN_256];
	u32 salt = 0;
	struct ext_key master_extkey, child_extkey;
	const u32 flags = SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN;
	secp256k1_context *ctx = secp256k1_context_create(flags);

	bip32_key_version = (struct bip32_key_version) {
		.bip32_pubkey_version = BIP32_VER_MAIN_PUBLIC,
		.bip32_privkey_version = BIP32_VER_MAIN_PRIVATE
	};

	assert(bip32_key_version.bip32_pubkey_version == BIP32_VER_MAIN_PUBLIC
			|| bip32_key_version.bip32_pubkey_version == BIP32_VER_TEST_PUBLIC);

	assert(bip32_key_version.bip32_privkey_version == BIP32_VER_MAIN_PRIVATE
			|| bip32_key_version.bip32_privkey_version == BIP32_VER_TEST_PRIVATE);

	/* Fill in the BIP32 tree for bitcoin addresses. */
	/* In libwally-core, the version BIP32_VER_TEST_PRIVATE is for testnet/regtest,
	 * and BIP32_VER_MAIN_PRIVATE is for mainnet. For litecoin, we also set it like
	 * bitcoin else.*/
	do {
		hkdf_sha256(bip32_seed, sizeof(bip32_seed),
			    &salt, sizeof(salt),
			    &secretstuff.hsm_secret,
			    sizeof(secretstuff.hsm_secret),
			    "bip32 seed", strlen("bip32 seed"));
		salt++;
	} while (bip32_key_from_seed(ctx, bip32_seed, sizeof(bip32_seed),
				     bip32_key_version.bip32_privkey_version,
				     0, &master_extkey) != WALLY_OK);

	/* BIP 32:
	 *
	 * The default wallet layout
	 *
	 * An HDW is organized as several 'accounts'. Accounts are numbered,
	 * the default account ("") being number 0. Clients are not required
	 * to support more than one account - if not, they only use the
	 * default account.
	 *
	 * Each account is composed of two keypair chains: an internal and an
	 * external one. The external keychain is used to generate new public
	 * addresses, while the internal keychain is used for all other
	 * operations (change addresses, generation addresses, ..., anything
	 * that doesn't need to be communicated). Clients that do not support
	 * separate keychains for these should use the external one for
	 * everything.
	 *
	 *  - m/iH/0/k corresponds to the k'th keypair of the external chain of
	 * account number i of the HDW derived from master m.
	 */
	/* Hence child 0, then child 0 again to get extkey to derive from. */
	if (bip32_key_from_parent(ctx, &master_extkey, 0,
				  BIP32_FLAG_KEY_PRIVATE,
				  &child_extkey) != WALLY_OK)
		/*~ status_failed() is a helper which exits and sends lightningd
		 * a message about what happened.  For hsmd, that's fatal to
		 * lightningd. */
		fatal1("Can't derive child bip32 key");

	if (bip32_key_from_parent(ctx, &child_extkey, 0,
				  BIP32_FLAG_KEY_PRIVATE,
				  &secretstuff.bip32) != WALLY_OK)
		fatal1("Can't derive private bip32 key");
}

static void load_hsm(const char *secretfile)
{
	int fd = open(secretfile ? secretfile : "hsm_secret", O_RDONLY);
	if (fd < 0)
		fatal("opening: %s", strerror(errno));
	if (!read_all(fd, &secretstuff.hsm_secret, sizeof(secretstuff.hsm_secret)))
		fatal("reading: %s", strerror(errno));
	close(fd);

	populate_secretstuff();
}

void usage()
{
	fprintf(stderr, "usage: clightning-dumpkeys <hsmd_secretfile>\n");
	exit(42);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
		usage();

	const char *secretfile = argv[1];

	assert_bip32_assumptions();
	load_hsm(secretfile);

	return 0;
}
