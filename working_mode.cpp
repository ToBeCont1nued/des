#include "base_n.h"
#include "working_mode.h"

char *ECB_encrypt(const char *key, const char *m, int size,
	char *dest, uint64_t(*Encrypt)(const char*, uint64_t))
{
	uint64_t cipher;
	for (int i = 0; i < size; i += 8) {
		cipher = Encrypt(key, str_to_bin(m + i, size - i));
		bin_to_str(cipher, dest + i);
	}
	return dest;
}
char *ECB_decrypt(const char *key, const char *c, int size,
	char *dest, uint64_t(*Decrypt)(const char*, uint64_t))
{
	size = (size + 8) & -8;
	return ECB_encrypt(key, c, size,
		dest, Decrypt);
}

char *CBC_encrypt(const char *key, const char *m, 
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t))
{
	uint64_t x = str_to_bin(IV, 8);
	for (int i = 0; i < size; i += 8) {
		x ^= str_to_bin(m + i, size - i);
		x = Encrypt(key, x);
		bin_to_str(x, dest + i);
	}
	return dest;
}
char *CBC_decrypt(const char *key, const char *c,
	const char IV[8], int size, char *dest,
	uint64_t(*Decrypt)(const char*, uint64_t))
{
	size = (size + 8) & -8;
	uint64_t x = str_to_bin(IV, 8);
	uint64_t y, z;
	for (int i = 0; i < size; i += 8) { 
		z = str_to_bin(c + i, 8);
		y = Decrypt(key, z) ^ x;
		bin_to_str(y, dest + i);
		x = z;
	}
	return dest;
}

char *CFB_encrypt(const char *key, const char *m,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t)) 
{
	// size = (size + 8) & (int)-8;
	uint64_t x = str_to_bin(IV, 8);
	char y;
	unsigned char b;
	for (int i = 0; i != size; ++i) {
		b = Encrypt(key, x) >> 56;
		y = m[i] ^ b;
		dest[i] = y;
		x &= 0xffffffffffffff;
		x <<= 8;
		x |= (unsigned char)y;
	}
	return dest;
}
char *CFB_decrypt(const char *key, const char *c,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t))
{
	uint64_t x = str_to_bin(IV, 8);
	char y;
	unsigned b;
	for (int i = 0; i!= size; ++i) {
		b = Encrypt(key, x) >> 56;
		y = c[i];
		dest[i] = y ^ b;
		x &= 0xffffffffffffff;
		x <<= 8;
		x |= (unsigned char)y;
	}
	return dest;
}

char *OFB_encrypt(const char *key, const char *m,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t))
{
	//size = (size + 8) & (int)-8;
	uint64_t x = str_to_bin(IV, 8);
	unsigned char b;
	for (int i = 0; i != size; ++i) {
		b = Encrypt(key, x) >> 56;
		dest[i] = m[i] ^ b;
		x &= 0xffffffffffffff;
		x <<= 8;
		x |= b;
	}
	return dest;
}
char *OFB_decrypt(const char *key, const char *c,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t))
{
	return OFB_encrypt(key, c, IV, size,
		dest, Encrypt);
}
