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

char cipher[0x1000000];
char p[0x1000000];

int main(int argc, char **argv)
{
	size_t size;
	const char *key = "\xe1\xe1\xe1\xe1\xf0\xf0\xf0\xf0";
	const char *IV = "Le3tc0de";

	size = 0x100000;
	memset(cipher, 'a', size);
	OFB_encrypt(key, cipher, IV, size,
		cipher, DES_encrypt);
	/*for (size_t i = 0; i != size; ++i)
		cout << setw(2) << setfill('0') << hex << (uint32_t)(uint8_t)cipher[i];
	cout << endl;*/
	p[size] = 0;
	OFB_decrypt(key, cipher, IV, size,
		p, DES_encrypt);
	/*for (size_t i = 0; i != size; ++i)
		cout << setw(2) << setfill('0') << hex << (uint32_t)(uint8_t)p[i];
	cout << endl;
	cout << p << endl;*/

	return 0;
}
