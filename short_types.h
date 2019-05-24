
#ifndef DK_SHORT_TYPES_H
#define DK_SHORT_TYPES_H

#include <stdint.h>

typedef uint64_t u64;
typedef int64_t s64;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint8_t u8;
typedef int8_t s8;

#define WALLY_OK      0 /** Success */
#define WALLY_ERROR  -1 /** General error */
#define WALLY_EINVAL -2 /** Invalid argument */
#define WALLY_ENOMEM -3 /** malloc() failed */


#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif /* DK_SHORT_TYPES_H */
