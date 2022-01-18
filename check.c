
#include "bip32.h"
#include "descriptor.h"
#include "short_types.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main()
{
	char checksum[9];
	const char desc[] = "combo(xpub661MyMwAqRbcEzDwwjzJhBnXBZpv9hdqY2JYRUtwzEZU6grFRhWkXx9c3HJ4EKR1Nvdwf5U3VoekstoKSKjfcJYhRrhMYeEzZzu2h7uZAQX/1/*)";
	const char *desc_check = "j38hxvh0";
	int ok = descriptor_checksum(desc, sizeof(desc)-1, checksum, sizeof(checksum));
	assert(ok == (int)strlen(desc_check));
	assert(streq(checksum, desc_check));
	printf("ok!\n");
	return 0;
}
