// ./include/security/poision.h

// #ifndef IMGENGINE_POISON_H
// #define IMGENGINE_POISON_H
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#include <sanitizer/asan_interface.h>
#define IMG_POISON_MEMORY(addr, size) ASAN_POISON_MEMORY_REGION(addr, size)
#define IMG_UNPOISON_MEMORY(addr, size) ASAN_UNPOISON_MEMORY_REGION(addr, size)
#endif
#endif

#ifndef IMG_POISON_MEMORY
#define IMG_POISON_MEMORY(addr, size) ((void)(addr), (void)(size))
#define IMG_UNPOISON_MEMORY(addr, size) ((void)(addr), (void)(size))
#endif

/*
 * img_poison_block() — call this instead of a standalone free_block().
 *
 * With ASAN: poisons the memory region (detects use-after-free).
 * Without ASAN: expands to nothing — zero overhead.
 *
 * Usage in slab/arena/numa: call img_poison_block(ptr, size)
 * when returning memory to the free list.
 */
#define img_poison_block(ptr, size) IMG_POISON_MEMORY(ptr, size)

// // ./include/security/poision.h

// #ifndef IMGENGINE_POISON_H
// #define IMGENGINE_POISON_H

// #if defined(__has_feature)
// #if __has_feature(address_sanitizer)
// #include <sanitizer/asan_interface.h>
// #define IMG_POISON_MEMORY(addr, size) ASAN_POISON_MEMORY_REGION(addr, size)
// #define IMG_UNPOISON_MEMORY(addr, size) ASAN_UNPOISON_MEMORY_REGION(addr, size)
// #endif
// #endif

// #ifndef IMG_POISON_MEMORY
// #define IMG_POISON_MEMORY(addr, size) ((void)0)
// #define IMG_UNPOISON_MEMORY(addr, size) ((void)0)
// #endif

// #endif