
/* memory/numa.c */
#include <numaif.h> // Requires libnuma
// Implementation uses mbind() to tie the hugepages to a specific physical CPU socket.
