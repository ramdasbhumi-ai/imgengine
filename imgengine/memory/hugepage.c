// /* memory/hugepage.c */
// #include "memory.h"
// #include <sys/mman.h>

// void *img_hugepage_alloc(size_t size)
// {
//     size_t aligned_size = (size + HPAGE_SIZE - 1) & ~(HPAGE_SIZE - 1);

//     // MAP_HUGETLB: Requests 2MB pages from the kernel
//     void *ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
//                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

//     if (ptr == MAP_FAILED)
//     {
//         // Fallback to standard pages if HugePages aren't configured
//         ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
//                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//     }

//     // Pre-fault memory to prevent page-fault spikes during requests
//     madvise(ptr, aligned_size, MADV_WILLNEED | MADV_HUGEPAGE);
//     return ptr;
// }

/* memory/hugepage.c */
#include "memory/memory.h" // Updated path
#include <sys/mman.h>

void *img_hugepage_alloc(size_t size)
{
    // ... (Your existing code) ...
    // Note: HPAGE_SIZE is now defined in memory/memory.h
    size_t aligned_size = (size + HPAGE_SIZE - 1) & ~(HPAGE_SIZE - 1);

    void *ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

    if (ptr == MAP_FAILED)
    {
        ptr = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

    madvise(ptr, aligned_size, MADV_WILLNEED | MADV_HUGEPAGE);
    return ptr;
}
