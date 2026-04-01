/* security/poison.h */
#ifdef __SANITIZE_ADDRESS__
#include <sanitizer/asan_interface.h>
#define IMG_POISON_MEMORY(addr, size) ASAN_POISON_MEMORY_REGION(addr, size)
#define IMG_UNPOISON_MEMORY(addr, size) ASAN_UNPOISON_MEMORY_REGION(addr, size)
#else
#define IMG_POISON_MEMORY(addr, size) ((void)0)
#define IMG_UNPOISON_MEMORY(addr, size) ((void)0)
#endif
