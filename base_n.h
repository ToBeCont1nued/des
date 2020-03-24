#ifndef base_n
#define base_n

#include <cstdint>

uint64_t str_to_bin(const char src[8], int size);
char *bin_to_str(uint64_t bin, char dest[8]);

#endif
