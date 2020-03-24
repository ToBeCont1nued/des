#include <cstring>

#include "base_n.h"
#include "des.h"

inline static uint64_t extend(uint32_t src)
{
	uint64_t x = src;
	return ((x & 1) << 47) | (x >> 31) |
		((x & 0xf8000000) << 15) | ((x & 0x1f800000) << 13) |
		((x & 0x1f80000) << 11) | ((x & 0x1f8000) << 9) |
		((x & 0x1f800) << 7) | ((x & 0x1f80) << 5) |
		((x & 0x1f8) << 3) | ((x & 0x1f) << 1);
}

static const uint32_t s[8][64] = {
	{
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	}, 
	{
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	},
	{
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	},
	{
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	},
	{
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	},
	{
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	},
	{
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	},
	{
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	},
};

static const uint32_t p[] = {
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25
};

static const uint32_t pc1[] = {
	57,49,41,33,25,17,9,1,
	58,50,42,34,26,18,10,2,
	59,51,43,35,27,19,11,3,
	60,52,44,36,
	63,55,47,39,31,23,15,7,
	62,54,46,38,30,22,14,6,
	61,53,45,37,29,21,13,5,
	28,20,12,4
};

static const uint32_t pc2[] = {
	14,17,11,24,1,5,3,28,
	15,6,21,10,23,19,12,4,
	26,8,16,7,27,20,13,2,
	41,52,31,37,47,55,30,40,
	51,45,33,48,44,49,39,56,
	34,53,46,42,50,36,29,32
};

static const uint32_t shifts[16] = {
	1,1,2,2,2,2,2,2,
	1,2,2,2,2,2,2,1
};

inline void init_perm(uint32_t &odd, uint32_t &even)
{
	uint32_t x;
	x = ((odd >> 4) ^ even) & 0x0f0f0f0f;
	even ^= x;
	odd ^= x << 4;
	x = ((odd >> 16) ^ even) & 0xffff;
	even ^= x;
	odd ^= x << 16;
	x = ((even >> 2) ^ odd) & 0x33333333;
	odd ^= x;
	even ^= x << 2;
	x = ((even >> 8) ^ odd) & 0xff00ff;
	odd ^= x;
	even ^= x << 8;
	x = ((odd >> 1) ^ even) & 0x55555555;
	even ^= x;
	odd ^= x << 1;
}

inline void inv_perm(uint32_t &odd, uint32_t &even)
{
	uint32_t x;
	x = ((odd >> 1) ^ even) & 0x55555555;
	even ^= x;
	odd ^= x << 1;
	x = ((even >> 8) ^ odd) & 0xff00ff;
	odd ^= x;
	even ^= x << 8;
	x = ((even >> 2) ^ odd) & 0x33333333;
	odd ^= x;
	even ^= x << 2;
	x = ((odd >> 16) ^ even) & 0xffff;
	even ^= x;
	odd ^= x << 16;
	x = ((odd >> 4) ^ even) & 0x0f0f0f0f;
	even ^= x;
	odd ^= x << 4;
}

static uint64_t sub_key(uint64_t &key28H, uint64_t &key28L, int shift)
{
	uint64_t maskL = (1 << (28 - shift)) - 1;
	uint64_t maskH = (1 << shift) - 1;
	key28H = ((key28H & maskL) << shift) | ((key28H >> (28 - shift)) & maskH);
	key28L = ((key28L & maskL) << shift) | ((key28L >> (28 - shift)) & maskH);
	uint64_t key56 = key28H << 28 | key28L;

	uint64_t key48 = replace_bit(key56, 56, pc2, 48);
	return key48;
}

static void DES_encrypt(uint32_t &m32L, uint32_t &m32R, uint64_t key)
{
	uint32_t i;
	uint32_t t = m32L;
	m32L = m32R;

	//const int e[] = {
	//	32,1,2,3,4,5,4,5,6,7,8,9,
	//	8,9,10,11,12,13,12,13,14,15,16,17,
	//	16,17,18,19,20,21,20,21,22,23,24,25,
	//	24,25,26,27,28,29,28,29,30,31,32,1
	//};
	//m32R = replace_bit(m32R, 32, e, 48) ^ key;
	uint64_t m48 = extend(m32R) ^ key;

	m32R = 0;
	for (i = 0; i != 8; ++i) {
		uint32_t x = m48 & 63;
		uint32_t index = x & 32 | ((x & 1) << 4) |
			(x >> 1) & 15;
		m32R |= s[7 - i][index] << (4 * i);
		m48 >>= 6;
	}

	m32R = replace_bit(m32R, 32, p, 32) ^ t;
}

static uint64_t *getkeys(const char key[8], uint64_t keys[16])
{
	uint64_t key_bin = str_to_bin(key, 8);
	uint64_t key56 = replace_bit(key_bin, 64, pc1, 56);
	uint64_t key28L = key56 >> 28;
	uint64_t key28R = key56 & 0xfffffff;
	for (int i = 0; i != 16; ++i)
		keys[i] = sub_key(key28L, key28R, shifts[i]);
	return keys;
}

static uint64_t DES_encrypt(uint64_t keys[16], uint64_t m)
{
	//static const int ip[] = {
	//	58,50,42,34,26,18,10,2,
	//	60,52,44,36,28,20,12,4,
	//	62,54,46,38,30,22,14,6,
	//	64,56,48,40,32,24,16,8,
	//	57,49,41,33,25,17,9,1,
	//	59,51,43,35,27,19,11,3,
	//	61,53,45,37,29,21,13,5,
	//	63,55,47,39,31,23,15,7
	//};
	// uint64_t m64 = replace_bit(m, 64, ip, 64);
	// uint64_t m64 = ip_replace(m);
	uint32_t m32L = m >> 32;
	uint32_t m32R = m & 0xffffffff;
	init_perm(m32L, m32R);

	for (int i = 0; i != 16; ++i)
		DES_encrypt(m32L, m32R, keys[i]);

	//static const uint32_t ip_inverse[] = {
	//	40,8,48,16,56,24,64,32,
	//	39,7,47,15,55,23,63,31,
	//	38,6,46,14,54,22,62,30,
	//	37,5,45,13,53,21,61,29,
	//	36,4,44,12,52,20,60,28,
	//	35,3,43,11,51,19,59,27,
	//	34,2,42,10,50,18,58,26,
	//	33,1,41,9,49,17,57,25
	//};

	// uint32_t temp = m32L;
	// m32L = m32R;
	// m32R = temp;

	inv_perm(m32R, m32L);
	// cipher = replace_bit(cipher, 64, ip_inverse, 64);
	return ((uint64_t)m32R << 32) | m32L;
}

uint64_t DES_encrypt(const char key[8], uint64_t m)				// universal
{
	uint64_t keys[16];
	getkeys(key, keys);
	return DES_encrypt(keys, m);
}

uint64_t DES_encrypt(const char key[8], const char m[8])		// encrypt string
{
	return DES_encrypt(key, str_to_bin(m, strlen(m)));
}

static uint64_t DES_decrypt(uint64_t keys[16], uint64_t c)
{
	uint64_t keys_reverse[16];
	for (int i = 0; i != 16; ++i)
		keys_reverse[i] = keys[15 - i];
	return DES_encrypt(keys_reverse, c);
}

uint64_t DES_decrypt(const char key[8], uint64_t c)				// universal
{
	uint64_t keys[16];
	getkeys(key, keys);
	return DES_decrypt(keys, c);
}

uint64_t DES_decrypt(const char key[8], const char c[8])		// decrypt string
{
	return DES_decrypt(key, str_to_bin(c, strlen(c)));
}

uint64_t tri_des_encrypt(const char key[24], uint64_t m)
{
	uint64_t c_bin;
	c_bin = DES_encrypt(key, m);
	c_bin = DES_decrypt(key + 8, c_bin);
	c_bin = DES_encrypt(key + 16, c_bin);
	return c_bin;
}

uint64_t tri_des_decrypt(const char key[24], uint64_t c)
{
	uint64_t m_bin;
	m_bin = DES_decrypt(key + 16, c);
	m_bin = DES_encrypt(key + 8, m_bin);
	m_bin = DES_decrypt(key, m_bin);
	return m_bin;
}

uint64_t tri_des_encrypt(const char key[24], const char m[8])
{
	return tri_des_encrypt(key, str_to_bin(m, 8));
}

uint64_t tri_des_decrypt(const char key[24], const char c[8])
{
	return tri_des_decrypt(key, str_to_bin(c, 8));
}


