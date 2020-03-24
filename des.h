#ifndef des_h
#define des_h

#include <cstdint>

uint64_t replace_bit(uint64_t src, int size_src, const uint32_t table[], int size_dest);

uint64_t DES_encrypt(const char key[8], const char m[8]);
uint64_t DES_encrypt(const char key[8], uint64_t m);
uint64_t DES_decrypt(const char key[8], const char c[8]);
uint64_t DES_decrypt(const char key[8], uint64_t c);
uint64_t tri_des_encrypt(const char key[24], const char m[8]);
uint64_t tri_des_decrypt(const char key[24], const char c[8]);
uint64_t tri_des_decrypt(const char key[24], uint64_t c);
uint64_t tri_des_encrypt(const char key[24], uint64_t m);

#endif
