#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

#include "base_n.h"
#include "des.h"
#include "working_mode.h"

using std::cin;			using std::cout;
using std::endl;		using std::hex;
using std::setw;		using std::setfill;
using std::ifstream;

int main(int argc, char **argv)
{
	size_t size;
	const char *plain = "hello world";
	const char *key = "12345678";
	const char *IV = "87654321";

	size = strlen(plain);

	char cipher[64];
	memset(cipher, 'a', size);
	OFB_encrypt(key, plain, IV, size,
		cipher, DES_encrypt);
	for (size_t i = 0; i != size; ++i)
		cout << setw(2) << setfill('0') << hex << (uint32_t)(uint8_t)cipher[i];
	cout << endl;
	char p[64];
	OFB_decrypt(key, cipher, IV, size,
		p, DES_encrypt);
	p[size] = 0;
	for (size_t i = 0; i != size; ++i)
		cout << setw(2) << setfill('0') << hex << (uint32_t)(uint8_t)p[i];
	cout << endl;
	cout << p << endl;

	return 0;
}
