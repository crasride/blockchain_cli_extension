#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <sys/stat.h>
#include <unistd.h>

# define EC_CURVE NID_secp256k1

/* EC_KEY public key octet string length (using 256-bit curve) */
# define EC_PUB_LEN 65
/* Maximum signature octet string length (using 256-bit curve) */
# define SIG_MAX_LEN 72

# define PRI_FILENAME "key.pem"
# define PUB_FILENAME "key_pub.pem"

/**
* struct sign_s - EC Signature structure
*
* @sig: Signature buffer. The whole space may not be used
* @len: Actual signature size. Can't exceed SIG_MAX_LEN,
* therefore stored on a byte
*/
typedef struct sign_s
{
	/*
	* @sig must stay first, so we can directly use the structure as
	* an array of char
	*/
	uint8_t sig[SIG_MAX_LEN];
	uint8_t len;
} sign_t;

/*Function Tasks 0*/
uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH]);

/*Function Tasks 1*/
EC_KEY *ec_create(void);

/*Function Tasks 2*/
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);

/*Function Tasks 3*/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN]);

/*Function Tasks 4*/
int ec_save(EC_KEY *key, char const *folder);

/*Function Tasks 5*/
EC_KEY *ec_load(char const *folder);

/*Function Tasks 6*/
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
		sign_t *sig);

/*Function Tasks 7*/
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
		sign_t const *sig);

#endif /* _CRYPTO_H_ */
