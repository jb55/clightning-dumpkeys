
BIN = clightning-dumpkeys
PREFIX ?= /usr/local

LDFLAGS = -lsecp256k1
CFLAGS = $(DEFS) -DHAVE_LITTLE_ENDIAN -O2 -g -I src -Wall -Werror -Wextra -std=c99

OBJS  = sha256.o
OBJS += sha512.o
OBJS += ripemd160.o
OBJS += hmac.o
OBJS += hkdf.o
OBJS += bip32.o
OBJS += compiler.o
OBJS += ec.o
OBJS += hash.o
OBJS += base58.o
OBJS += descriptor.o

SRCS=$(OBJS:.o=.c)

all: $(BIN)

$(BIN): clightning-dumpkeys.o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(OBJS) clightning-dumpkeys.o check.o $(BIN) check

check: check.o $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	./check
