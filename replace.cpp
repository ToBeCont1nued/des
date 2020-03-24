#include "des.h"

uint64_t replace_bit(uint64_t src, int size_src, const uint32_t table[], int size_dest)
{
	uint64_t dest = 0;
	for (int i = 0; i != size_dest; ++i)
		dest |= ((src >> (size_src - table[i])) & 1) << (size_dest - 1 - i);
	return dest;
}

