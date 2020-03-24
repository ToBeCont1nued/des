#ifndef working_mode_h
#define	working_mode_h

char *ECB_encrypt(const char *key, const char *m, int size,
	char *dest, uint64_t(*Encrypt)(const char*, uint64_t));
char *ECB_decrypt(const char *key, const char *c, int size,
	char *dest, uint64_t(*Decrypt)(const char*, uint64_t));

char *CBC_encrypt(const char *key, const char *m,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t));
char *CBC_decrypt(const char *key, const char *c,
	const char IV[8], int size, char *dest,
	uint64_t(*Decrypt)(const char*, uint64_t));

char *CFB_encrypt(const char *key, const char *m,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t));
char *CFB_decrypt(const char *key, const char *c,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t));

char *OFB_encrypt(const char *key, const char *m,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t));
char *OFB_decrypt(const char *key, const char *c,
	const char IV[8], int size, char *dest,
	uint64_t(*Encrypt)(const char*, uint64_t));

#endif
