#ifndef _UTILS_H_
#define _UTILS_H_

#define MIN(_a, _b)                                             \
({                                                              \
        typeof(_a) __a = (_a);                                  \
        typeof(_b) __b = (_b);                                  \
        __a <= __b ? __a : __b;                                 \
})
#define MAX(_a, _b)                                             \
({                                                              \
        typeof(_a) __a = (_a);                                  \
        typeof(_b) __b = (_b);                                  \
        __a >= __b ? __a : __b;                                 \
})

// Rounding operations (efficient when n is a power of 2)
// Round down to the nearest multiple of n
#define ROUNDDOWN(a, n)                                         \
({                                                              \
        uint32_t __a = (uint32_t) (a);                          \
        (typeof(a)) (__a - __a % (n));                          \
})
// Round up to the nearest multiple of n
#define ROUNDUP(a, n)                                           \
({                                                              \
        uint32_t __n = (uint32_t) (n);                          \
        (typeof(a)) (ROUNDDOWN((uint32_t) (a) + __n - 1, __n)); \
})


#endif // _UTILS_H_
