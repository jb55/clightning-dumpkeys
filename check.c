
#include "bip32.h"
#include <stdio.h>

int main()
{
	printf("asserting bip32 assumptions\n");
	assert_bip32_assumptions();
	printf("ok!\n");
	return 0;
}
