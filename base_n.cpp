#include "base_n.h"

uint64_t str_to_bin(const char src[8], int size)
{
	uint64_t bin = 0;
	for (int i = 0; i != 8; ++i)
	{
		bin <<= 8;
		if (i < size)
			bin |= (unsigned char)src[i];
	}
	return bin;
}

char *bin_to_str(uint64_t bin, char dest[8])
{
	int i = 8;
	while (i > 0) {
		--i;
		dest[i] = bin & 0xff;
		bin >>= 8;
	}
	return dest;
}
