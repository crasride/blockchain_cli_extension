<h1 align="center">Blockchain - Crypto</h1>

Concepts
For this project, we expect you to look at these concepts:

* [[EYNTK] Blockchain - Introduction](./Introduction.md)
* [[EYNTK] Blockchain - Crypto](./Crypto.md)

## Resources

**Read or watch:**

* [OpenSSL crypto library](https://www.openssl.org/docs/manmaster/man3/)
* [Provided libraries and sources](https://github.com/hs-hq/holbertonschool-blockchain)


## Learning Objectives
* How a Blockchain is considered “unbreakable”
* What is a hash algorithm
* What **SHA** stands for
* How hash algorithms apply to Blockchains
* What is asymmetric cryptography
* How asymmetric cryptography applies to cryptocurrencies
* What **ECC** stands for
* What **ECDSA** stands for
* What a digital signature is
* How digital signatures apply to cryptocurrencies

## Requirements

### General

* Allowed editors: `vi`, `vim`, `emacs`
* All your files will be compiled on Ubuntu 14.04 LTS
* Your C programs and functions will be compiled with `gcc 4.8.4` using the flags `-Wall` `-Werror` `-Wextra,` `-pedantic` and the linker flags `-lssl` and `-lcrypto`
* All your files should end with a new line
* A README.md file, at the root of the folder of the project, is mandatory
* Your code should use the `Betty` style. It will be checked using [betty-style.pl](https://github.com/holbertonschool/Betty/blob/master/betty-style.pl) and [betty-doc.pl](https://github.com/holbertonschool/Betty/blob/master/betty-doc.pl)
* The prototypes of all your functions should be included in your header file called `crypto/hblk_crypto.h`
* All your header files should be include guarded

## Dependencies
For this project, you will need to Install the [OpenSSL](https://help.ubuntu.com/community/OpenSSL#Practical_OpenSSL_Usage) library (if not already installed).

### Additional information

* Unless specified otherwise, you are allowed to use the C standard library
* You are free to use any data structure that suits you as long as their purpose is well defined
* You are free to print any information **on stderr**, this stream will be discarded during correction. Since you are going to use these utility functions in your Blockchain project, you are free to manage errors as you wish.
* At the end of this project, the structure of your repository should look like this (you may have additional files):
```
holbertonschool-blockchain
├── crypto
│   ├── ec_create.c
│   ├── ec_from_pub.c
│   ├── ec_load.c
│   ├── ec_save.c
│   ├── ec_sign.c
│   ├── ec_to_pub.c
│   ├── ec_verify.c
│   ├── hblk_crypto.h
│   ├── libhblk_crypto.a
│   ├── Makefile
│   ├── provided
│   │   └── _print_hex_buffer.c
│   ├── sha256.c
│   └── test
│       ├── ec_create-main.c
│       ├── ec_from_pub-main.c
│       ├── ec_load-main.c
│       ├── ec_save-main.c
│       ├── ec_sign-main.c
│       ├── ec_to_pub-main.c
│       ├── ec_verify-main.c
│       └── sha256-main.c
└── README.md
```

# Tasks

### [0. SHA256](./sha256.c)
Write a function that computes the hash of a sequence of bytes
* Prototype: `uint8_t *sha256(int8_t const *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH]);`, where
	* `s` is the sequence of bytes to be hashed
	* `len` is the number of bytes to hash in s
* The resulting hash must be stored in `digest`
* Your function must return a pointer to `digest`
* If `digest` happens to be `NULL`, your function must do nothing and return NULL
* You are not allowed to allocate memory dynamically
```
alex@~/holbertonschool-blockchain/crypto$ cat test/sha256-main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * main - Entry point
 *
 * @ac: Arguments counter
 * @av: Arguments vector
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int ac, char **av)
{
    uint8_t hash[SHA256_DIGEST_LENGTH];
    uint8_t *test_ptr;

    if (ac < 2)
    {
        fprintf(stderr, "Usage: %s arg\n", av[0]);
        return (EXIT_FAILURE);
    }

    /* Test `sha256()` */
    test_ptr = sha256((int8_t *)av[1], strlen(av[1]), hash);
    if (!test_ptr)
    {
        fprintf(stderr, "sha256() failed\n");
        return (EXIT_FAILURE);
    }
    if (test_ptr != hash)
    {
        fprintf(stderr, "Return value and pointer differ\n");
        return (EXIT_FAILURE);
    }

    printf("\"%s\" hash is: ", av[1]);
    _print_hex_buffer(hash, SHA256_DIGEST_LENGTH);
    printf("\n");

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/crypto$ gcc -Wall -Wextra -Werror -pedantic -I. -o sha256-test test/sha256-main.c provided/_print_hex_buffer.c sha256.c -lssl -lcrypto
alex@~/holbertonschool-blockchain/crypto$ ./sha256-test Holberton
77c4925c01e8d9f79c8a6a61685c6b3182be10f2fa553de915f3733ee19c0204
alex@~/holbertonschool-blockchain/crypto$ ./sha256-test "Holberton School"
2a05534c3fd942dc5e5ab0a968433b747b2188d526feffdb5a6406f15bf4861c
alex@~/holbertonschool-blockchain/crypto$
```
### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/sha256.c`, `crypto/hblk_crypto.h`

---

### [1. EC_KEY creation](./ec_create.c)
Write a function that creates a new EC key pair.
* Prototype: `EC_KEY *ec_create(void);`
* Your function must return a pointer to an `EC_KEY` structure, containing both the public and private keys, or `NULL` upon failure
* Both the private and the public keys must be generated
* You have to use the [secp256k1](https://en.bitcoin.it/wiki/Secp256k1) elliptic curve to create the new pair (See `EC_CURVE` macro)
```
alex@~/holbertonschool-blockchain/crypto$ cat test/ec_create-main.c
#include <stdlib.h>
#include <stdio.h>

#include "hblk_crypto.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    EC_KEY *key;

    /* Test `ec_create()` */
    key = ec_create();
    if (!key)
    {
        fprintf(stderr, "ec_create() failed\n");
        return (EXIT_FAILURE);
    }
    if (!EC_KEY_check_key(key))
    {
        fprintf(stderr, "Key verification failed\n");
        return (EXIT_FAILURE);
    }

    printf("Successfully created EC key pair\n");

    /* Cleanup */
    EC_KEY_free(key);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/crypto$ gcc -Wall -Wextra -Werror -pedantic -I. -o ec_create-test test/ec_create-main.c ec_create.c -lssl -lcrypto
alex@~/holbertonschool-blockchain/crypto$ valgrind ./ec_create-test
==17436== Memcheck, a memory error detector
==17436== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==17436== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==17436== Command: ./ec_create-test
==17436==
Successfully created EC key pair
==17436==
==17436== HEAP SUMMARY:
==17436==     in use at exit: 0 bytes in 0 blocks
==17436==   total heap usage: X allocs, X frees, Y bytes allocated
==17436==
==17436== All heap blocks were freed -- no leaks are possible
==17436==
==17436== For counts of detected and suppressed errors, rerun with: -v
==17436== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
alex@~/holbertonschool-blockchain/crypto$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/ec_create.c`

---

### [2. EC_KEY to public key](./ec_to_pub.c)
Write a function that extracts the public key from an `EC_KEY` opaque structure

* Prototype: `uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);`, where:
	* `key` is a pointer to the `EC_KEY` structure to retrieve the public key from. If it is `NULL`, your function must do nothing and fail
	* `pub` is the address at which to store the extracted public key (The key shall not be compressed)
* Your function must return a pointer to `pub`
* `NULL` must be returned upon failure, and there should not be any memory leak

NOTE: It is also possible to extract the private key from an EC_KEY structure, but we’re never going to store one’s private key anywhere in the Blockchain. So we don’t really need it.
```
alex@~/holbertonschool-blockchain/crypto$ cat test/ec_to_pub-main.c
#include <stdlib.h>
#include <stdio.h>

#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    EC_KEY *key;
    uint8_t pub[EC_PUB_LEN];
    uint8_t *test_ptr;

    /* Prerequisites */
    key = ec_create();
    if (!key)
    {
        fprintf(stderr, "ec_create() failed\n");
        return (EXIT_FAILURE);
    }
    printf("Successfully created EC key pair\n");

    /* Test `ec_to_pub()` */
    test_ptr = ec_to_pub(key, pub);
    if (!test_ptr)
    {
        fprintf(stderr, "ec_to_pub() failed\n");
        EC_KEY_free(key);
        return (EXIT_FAILURE);
    }
    if (test_ptr != pub)
    {
        fprintf(stderr, "Return value and pointer differ\n");
        EC_KEY_free(key);
        return (EXIT_FAILURE);
    }

    printf("Public key: ");
    _print_hex_buffer(pub, EC_PUB_LEN);
    printf("\n");

    /* Cleanup */
    EC_KEY_free(key);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/crypto$ gcc -Wall -Wextra -Werror -pedantic -I. -o ec_to_pub-test test/ec_to_pub-main.c provided/_print_hex_buffer.c ec_to_pub.c ec_create.c -lssl -lcrypto
alex@~/holbertonschool-blockchain/crypto$ valgrind ./ec_to_pub-test
==18243== Memcheck, a memory error detector
==18243== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==18243== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==18243== Command: ./ec_to_pub-test
==18243==
Successfully created EC key pair
Public key: 04a6dedb9d6180946b7866fc1a63ceff2aa8012161e0a01c351fb8e408b5863de5a1732497e7f4da0f7ff96e6650a51d0ca64eccd969415f8f53e956aa046991df
==18243==
==18243== HEAP SUMMARY:
==18243==     in use at exit: 0 bytes in 0 blocks
==18243==   total heap usage: X allocs, X frees, Y bytes allocated
==18243==
==18243== All heap blocks were freed -- no leaks are possible
==18243==
==18243== For counts of detected and suppressed errors, rerun with: -v
==18243== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
alex@~/holbertonschool-blockchain/crypto$
```
### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/ec_to_pub.c`

---

### [3. EC_KEY from public key](./ec_from_pub.c)
Write a function that creates an `EC_KEY` structure given a public key

* Prototype: `EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN]);`, where:
	* `pub` contains the public key to be converted
* Your function must return a pointer to the created `EC_KEY` structure upon success, or `NULL` upon failure
* The created `EC_KEY`‘s private key does not have to be initialized/set, we only care about the public one
```
alex@~/holbertonschool-blockchain/crypto$ cat test/ec_from_pub-main.c
#include <stdlib.h>
#include <stdio.h>

#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    EC_KEY *key, *key2;
    uint8_t pub[EC_PUB_LEN], pub2[EC_PUB_LEN];

    /* Prerequisites */
    key = ec_create();
    if (!key)
    {
        fprintf(stderr, "ec_create() failed\n");
        return (EXIT_FAILURE);
    }
    printf("Successfully created EC key pair\n");

    if (!ec_to_pub(key, pub))
    {
        fprintf(stderr, "ec_to_pub() failed\n");
        EC_KEY_free(key);
        return (EXIT_FAILURE);
    }

    printf("Public key: ");
    _print_hex_buffer(pub, EC_PUB_LEN);
    printf("\n");

    /* Test `ec_from_pub()` */
    key2 = ec_from_pub(pub);
    if (!key2)
    {
        fprintf(stderr, "ec_from_pub() failed\n");
        EC_KEY_free(key);
        return (EXIT_FAILURE);
    }
    if (!EC_KEY_check_key(key2))
    {
        fprintf(stderr, "Key2 verification failed\n");
        return (EXIT_FAILURE);
    }
    printf("Successfully created EC key from public key\n");

    /* Extract public key from new key */
    if (!ec_to_pub(key2, pub2))
    {
        fprintf(stderr, "ec_to_pub() failed\n");
        EC_KEY_free(key);
        EC_KEY_free(key2);
        return (EXIT_FAILURE);
    }
    printf("Public key: ");
    _print_hex_buffer(pub2, EC_PUB_LEN);
    printf("\n");

    /* Cleanup */
    EC_KEY_free(key);
    EC_KEY_free(key2);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/crypto$ gcc -Wall -Wextra -Werror -pedantic -I. -o ec_from_pub-test test/ec_from_pub-main.c provided/_print_hex_buffer.c ec_from_pub.c ec_to_pub.c ec_create.c -lssl -lcrypto
alex@~/holbertonschool-blockchain/crypto$ valgrind ./ec_from_pub-test
==18821== Memcheck, a memory error detector
==18821== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==18821== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==18821== Command: ./ec_from_pub-test
==18821==
Successfully created EC key pair
Public key: 042f6c2676461c03b57d97562794e6e42c50f7af817b4a5ff0546d09ac046cb46dd28e8b2ee6fbab94409e94eafde30ec89b425ea80e6ad66a85a2164e1f5f14e2
Successfully created EC key from public key
Public key: 042f6c2676461c03b57d97562794e6e42c50f7af817b4a5ff0546d09ac046cb46dd28e8b2ee6fbab94409e94eafde30ec89b425ea80e6ad66a85a2164e1f5f14e2
==18821==
==18821== HEAP SUMMARY:
==18821==     in use at exit: 0 bytes in 0 blocks
==18821==   total heap usage: X allocs, X frees, Y bytes allocated
==18821==
==18821== All heap blocks were freed -- no leaks are possible
==18821==
==18821== For counts of detected and suppressed errors, rerun with: -v
==18821== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
alex@~/holbertonschool-blockchain/crypto$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/ec_from_pub.c`

---

### [4. EC_KEY - Save to file](./ec_save.c)
Write a function that saves an existing EC key pair on the disk.

* Prototype: `int ec_save(EC_KEY *key, char const *folder);`, where
	* `key` points to the EC key pair to be saved on disk
	* `folder` is the path to the folder in which to save the keys (e.g. `/home/hblk/alex`)
* Your function must respectively return 1 or 0 upon success or failure
* `folder` must be created if it doesn’t already exist
	* `<folder>/key.pem` will contain the **private** key, in the **PEM** format. The file must be created, or overridden if it already exists (e.g. `/home/alex/.hblk/alex/key.pem`)
	* `<folder>/key_pub.pem` will contain the **public** key, in the **PEM** format. The file must be created, or overridden if it already exists (e.g. `/home/alex/.hblk/alex/key_pub.pem`)
```
#include <stdlib.h>
#include <stdio.h>

#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * main - Entry point
 *
 * @ac: Arguments count
 * @av: Arguments vector
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int ac, char **av)
{
    EC_KEY *key;
    uint8_t pub[EC_PUB_LEN];

    if (ac < 2)
    {
        fprintf(stderr, "Usage: %s <path>\n", av[0]);
        return (EXIT_FAILURE);
    }

    key = ec_create();
    ec_to_pub(key, pub);

    printf("Public key: ");
    _print_hex_buffer(pub, EC_PUB_LEN);
    printf("\n");

    /* Test `ec_save()` */
    ec_save(key, av[1]);

    /* Cleanup */
    EC_KEY_free(key);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/crypto$ gcc -Wall -Wextra -Werror -pedantic -I. -o ec_save-test test/ec_save-main.c ec_save.c ec_create.c ec_to_pub.c provided/_print_hex_buffer.c -lssl -lcrypto
alex@~/holbertonschool-blockchain/crypto$ ls -l alex
ls: cannot access alex: No such file or directory
alex@~/holbertonschool-blockchain/crypto$ ./ec_save-test alex
Public key: 047867deed2d85786a6d59aec306aad371c3820c30551ea6fd3009a52de7c7815c2234f648d67027ee31ad0930f6d463532d129bdec8add761438393ec9958ff69
alex@~/holbertonschool-blockchain/crypto$ ls -l alex
total 8
-rw-r--r-- 1 vagrant vagrant 438 Dec  9 06:47 key.pem
-rw-r--r-- 1 vagrant vagrant 390 Dec  9 06:47 key_pub.pem
alex@~/holbertonschool-blockchain/crypto$ openssl ec -text -noout -in alex/key.pem
read EC key
Private-Key: (256 bit)
priv:
    16:28:7c:53:cd:d0:0e:d8:01:b4:01:3f:6f:b8:e4:
    1d:69:7f:b2:95:d4:d6:f4:a9:21:bb:3e:7f:1e:4b:
    a5:ba
pub:
    04:78:67:de:ed:2d:85:78:6a:6d:59:ae:c3:06:aa:
    d3:71:c3:82:0c:30:55:1e:a6:fd:30:09:a5:2d:e7:
    c7:81:5c:22:34:f6:48:d6:70:27:ee:31:ad:09:30:
    f6:d4:63:53:2d:12:9b:de:c8:ad:d7:61:43:83:93:
    ec:99:58:ff:69
Field Type: prime-field
Prime:
    00:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:
    ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:fe:ff:
    ff:fc:2f
A:    0
B:    7 (0x7)
Generator (uncompressed):
    04:79:be:66:7e:f9:dc:bb:ac:55:a0:62:95:ce:87:
    0b:07:02:9b:fc:db:2d:ce:28:d9:59:f2:81:5b:16:
    f8:17:98:48:3a:da:77:26:a3:c4:65:5d:a4:fb:fc:
    0e:11:08:a8:fd:17:b4:48:a6:85:54:19:9c:47:d0:
    8f:fb:10:d4:b8
Order:
    00:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:ff:
    ff:fe:ba:ae:dc:e6:af:48:a0:3b:bf:d2:5e:8c:d0:
    36:41:41
Cofactor:  1 (0x1)
alex@~/holbertonschool-blockchain/crypto$ cat alex/key.pem
-----BEGIN EC PRIVATE KEY-----
MIIBEwIBAQQgFih8U83QDtgBtAE/b7jkHWl/spXU1vSpIbs+fx5LpbqggaUwgaIC
AQEwLAYHKoZIzj0BAQIhAP////////////////////////////////////7///wv
MAYEAQAEAQcEQQR5vmZ++dy7rFWgYpXOhwsHApv82y3OKNlZ8oFbFvgXmEg62ncm
o8RlXaT7/A4RCKj9F7RIpoVUGZxH0I/7ENS4AiEA/////////////////////rqu
3OavSKA7v9JejNA2QUECAQGhRANCAAR4Z97tLYV4am1ZrsMGqtNxw4IMMFUepv0w
CaUt58eBXCI09kjWcCfuMa0JMPbUY1MtEpveyK3XYUODk+yZWP9p
-----END EC PRIVATE KEY-----
alex@~/holbertonschool-blockchain/crypto$ cat alex/key_pub.pem
-----BEGIN PUBLIC KEY-----
MIH1MIGuBgcqhkjOPQIBMIGiAgEBMCwGByqGSM49AQECIQD/////////////////
///////////////////+///8LzAGBAEABAEHBEEEeb5mfvncu6xVoGKVzocLBwKb
/NstzijZWfKBWxb4F5hIOtp3JqPEZV2k+/wOEQio/Re0SKaFVBmcR9CP+xDUuAIh
AP////////////////////66rtzmr0igO7/SXozQNkFBAgEBA0IABHhn3u0thXhq
bVmuwwaq03HDggwwVR6m/TAJpS3nx4FcIjT2SNZwJ+4xrQkw9tRjUy0Sm97Irddh
Q4OT7JlY/2k=
-----END PUBLIC KEY-----
alex@~/holbertonschool-blockchain/crypto$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/ec_save.c`

---

### [5. EC_KEY - Load from file](./ec_load.c)
Write a function that loads an EC key pair from the disk.

* Prototype: `EC_KEY *ec_load(char const *folder);`, where
	* `folder` is the path to the folder from which to load the keys (e.g. `/home/hblk/alex`)
* Your function must return a pointer to the created EC key pair upon success, or `NULL` upon failure
* From the folder `folder`:
	* `<folder>/key.pem` will contain the **private** key, in the **PEM** format.
	* `<folder>/key_pub.pem` will contain the **public** key, in the **PEM** format.
```
alex@~/holbertonschool-blockchain/crypto$ cat test/ec_load-main.c
#include <stdlib.h>
#include <stdio.h>

#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * main - Entry point
 *
 * @ac: Arguments count
 * @av: Arguments vector
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int ac, char **av)
{
    EC_KEY *key;
    EC_KEY *key2;
    uint8_t pub[EC_PUB_LEN];

    if (ac < 2)
    {
        fprintf(stderr, "Usage: %s <path>\n", av[0]);
        return (EXIT_FAILURE);
    }

    key = ec_create();
    ec_to_pub(key, pub);

    printf("Public key: ");
    _print_hex_buffer(pub, EC_PUB_LEN);
    printf("\n");

    ec_save(key, av[1]);

    /* Test `ec_load()` */
    key2 = ec_load(av[1]);
    ec_to_pub(key2, pub);

    printf("Public key: ");
    _print_hex_buffer(pub, EC_PUB_LEN);
    printf("\n");

    /* Cleanup */
    EC_KEY_free(key);
    EC_KEY_free(key2);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/crypto$ gcc -Wall -Wextra -Werror -pedantic -I. -o ec_load-test test/ec_load-main.c ec_load.c ec_save.c ec_create.c ec_to_pub.c provided/_print_hex_buffer.c -lssl -lcrypto
alex@~/holbertonschool-blockchain/crypto$ ls -l alex
ls: cannot access alex: No such file or directory
alex@~/holbertonschool-blockchain/crypto$ ./ec_load-test alex
Public key: 04acfb9b1f03ea8a23c913104d68ee52bff7f6212e13d7bb33c1727b01280ae7adc135fcafd04cdb09687ef0879d671af74ddbffccbbe4abb3589e1ac867ce8336
Public key: 04acfb9b1f03ea8a23c913104d68ee52bff7f6212e13d7bb33c1727b01280ae7adc135fcafd04cdb09687ef0879d671af74ddbffccbbe4abb3589e1ac867ce8336
alex@~/holbertonschool-blockchain/crypto$
```
### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/ec_load.c`

---

### [6. Signature](./ec_sign.c)
Write a function that signs a given set of bytes, using a given EC_KEY **private key**

* Prototype: `uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen, sign_t *sig);`, where:
	* `key` points to the `EC_KEY` structure containing the private key to be used to perform the signature
	* `msg` points to the `msglen` characters to be signed
	* `sig` holds the address at which to store the signature
	* If either `key` or `msg` is NULL, your function must fail
* `sig->sig` does not need to be zero-terminated. If you feel like you want to zero-terminate it, make sure that `sig->len` holds the size of the signature without the trailing zero byte
* Your function must return a pointer to the signature buffer upon success (`sig->sig`)
* `NULL` must be returned upon failure
```
alex@~/holbertonschool-blockchain/crypto$ cat test/ec_sign-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * test_ec_sign - Test the ec_sign function
 *
 * @key: Pointer to the EC Key pair to use to sign the message
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
static int test_ec_sign(EC_KEY const *key)
{
    uint8_t const str[] = "Holberton";
    sign_t sig;

    if (!ec_sign(key, str, strlen((char *)str), &sig))
    {
        fprintf(stderr, "ec_sign() failed\n");
        return (EXIT_FAILURE);
    }
    printf("Signature of \"%s\": ", str);
    _print_hex_buffer(sig.sig, sig.len);
    printf("\n");

    return (EXIT_SUCCESS);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    EC_KEY *key;

    /* Prerequisites */
    key = ec_create();
    if (!key)
    {
        fprintf(stderr, "ec_create() failed\n");
        return (EXIT_FAILURE);
    }
    printf("Successfully created EC key pair\n");

    /* Test `ec_sign()` */
    if (test_ec_sign(key) != EXIT_SUCCESS)
    {
        EC_KEY_free(key);
        return (EXIT_FAILURE);
    }
    if (test_ec_sign(key) != EXIT_SUCCESS)
    {
        EC_KEY_free(key);
        return (EXIT_FAILURE);
    }

    /* Cleanup */
    EC_KEY_free(key);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/crypto$ gcc -Wall -Wextra -Werror -pedantic -I. -o ec_sign-test test/ec_sign-main.c provided/_print_hex_buffer.c ec_sign.c ec_create.c -lssl -lcrypto
alex@~/holbertonschool-blockchain/crypto$ ./ec_sign-test
Successfully created EC key pair
Signature of "Holberton": 3044022071847445398a7aee5c3c3b112fc7d067d8c6d11a91bfb8c681c5300a0e9541b8022045f0741c75b7ce499c0cb5508649520f62160b10ba5925fabe1918e641e80255
Signature of "Holberton": 304502204349c8265cdcf0cb533a2c44f591440350040d44d426ff7304a07b838f5c5e7202210092edcbdaa61d16d7a9933177fde5b1a8151699bf1b19e6ae09e774f045617966
alex@~/holbertonschool-blockchain/crypto$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/ec_sign.c`
---

### [7. Signature verification](./ec_verify.c)
Write a function that verifies the signature of a given set of bytes, using a given EC_KEY **public key**

* Prototype: `int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen, sign_t const *sig);`, where:
	* `key` points to the `EC_KEY` structure containing the public key to be used to verify the signature
	* `msg` points to the `msglen` characters to verify the signature of
	* `sig` points to the signature to be checked
	* If either `key`, `msg` or `sig` is NULL, your function must fail
* Your function must return 1 if the signature is valid, and 0 otherwise
```
alex@~/holbertonschool-blockchain/crypto$ cat test/ec_verify-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * test_ec_sign - Test the ec_sign function
 *
 * @key: Pointer to the EC Key pair to use to sign the message
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
static int test_ec_sign(EC_KEY const *key)
{
    uint8_t const str[] = "Holberton";
    sign_t sig;

    if (!ec_sign(key, str, strlen((char *)str), &sig))
    {
        fprintf(stderr, "ec_sign() failed\n");
        return (EXIT_FAILURE);
    }
    printf("Signature of \"%s\": ", str);
    _print_hex_buffer(sig.sig, sig.len);
    printf("\n");

    if (!ec_verify(key, str, strlen((char *)str), &sig))
    {
        fprintf(stderr, "ec_verify() failed\n");
        return (EXIT_FAILURE);
    }
    printf("Signature verified\n");

    return (EXIT_SUCCESS);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    EC_KEY *key;

    /* Prerequisites */
    key = ec_create();
    if (!key)
    {
        fprintf(stderr, "ec_create() failed\n");
        return (EXIT_FAILURE);
    }
    printf("Successfully created EC key pair\n");

    /* Test `ec_verify()` */
    if (test_ec_sign(key) != EXIT_SUCCESS)
    {
        EC_KEY_free(key);
        return (EXIT_FAILURE);
    }
    if (test_ec_sign(key) != EXIT_SUCCESS)
    {
        EC_KEY_free(key);
        return (EXIT_FAILURE);
    }

    /* Cleanup */
    EC_KEY_free(key);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/crypto$ gcc -Wall -Wextra -Werror -pedantic -I. -o ec_verify-test test/ec_verify-main.c provided/_print_hex_buffer.c ec_verify.c ec_sign.c ec_create.c -lssl -lcrypto
alex@~/holbertonschool-blockchain/crypto$ ./ec_verify-test
Successfully created EC key pair
Signature of "Holberton": 304402204e6f121cc2ecdc9a49c81e163ac7b41c53905cb241d48f537dc1db569bb47c750220451dec8ada9ad9ac412e923e91f061742ec6eb2586b8726acff8da05d915271b
Signature verified
Signature of "Holberton": 3044022078a379f78868c573c3c9e8afe1c68318f8d4b71be12cd249c064c1aa9b0e780a022063dd736e033cf5d8d3522adb838fdcf44defc2d8f3e917910f3151f2ab707773
Signature verified
alex@~/holbertonschool-blockchain/crypto$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/ec_verify.c`

---

### [8. Library](./Makefile)
Write a Makefile that compiles all the previous functions and archives them into a static library for future use.

The library must be called `libhblk_crypto.a`, and your Makefile must define a rule for this file.
```
alex@~/holbertonschool-blockchain/crypto$ make libhblk_crypto.a
[...]
alex@~/holbertonschool-blockchain/crypto$ ar -t libhblk_crypto.a
sha256.o
ec_create.o
ec_to_pub.o
ec_from_pub.o
ec_save.o
ec_load.o
ec_sign.o
ec_verify.o
alex@~/holbertonschool-blockchain/crypto$ nm --defined-only libhblk_crypto.a | grep T | cut -d ' ' -f3
sha256
ec_create
ec_to_pub
ec_from_pub
ec_save
ec_load
ec_sign
ec_verify
alex@~/holbertonschool-blockchain/crypto$
```
### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `crypto/Makefile`

---
