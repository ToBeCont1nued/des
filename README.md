# des

usage:  
由于每个分组密码的加密方式不一样，所以用法稍微有所不同  
ECB:  
``` cpp
ECB_encrypt(key, plain, size, cipher, DES_encrypt);
ECB_decrypt(key, cipher, size, p, DES_decrypt);
```
CBC:  
``` cpp
CBC_encrypt(key, plain, IV, size, cipher, DES_encrypt);
CBC_decrypt(key, cipher, IV, size, p, DES_decrypt);
```
CFB:  
``` cpp
CFB_encrypt(key, plain, IV, size, cipher, DES_encrypt);
CFB_decrypt(key, cipher, IV, size, p, DES_encrypt);
```
OFB：  
``` cpp
OFB_encrypt(key, plain, IV, size, cipher, DES_encrypt);
OFB_decrypt(key, cipher, IV, size, p, DES_encrypt);
```
