
BIN = clightning-dumpkeys
PREFIX ?= /usr/local

LDFLAGS = -lsecp256k1
CFLAGS = $(DEFS) -Og -g -I src -Wall -Werror -Wextra -std=c99

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

SRCS=$(OBJS:.o=.c)

all: $(BIN)

$(BIN): clightning-dumpkeys.o $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) clightning-dumpkeys.o check.o $(BIN) check

check: check.o $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	./check
