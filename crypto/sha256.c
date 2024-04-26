#include "hblk_crypto.h"

/**
* sha256 - Computes the hash of a sequence of bytes using SHA-256
* @s: Sequence of bytes to be hashed
* @len: Number of bytes to hash in @s
* @digest: Buffer to store the resulting hash
*
* Return: Pointer to @digest, or NULL on failure
*/
uint8_t *sha256(int8_t const *s, size_t len,
				uint8_t digest[SHA256_DIGEST_LENGTH])
{
	/* Initialize a SHA-256 operation */
	SHA256_CTX sha256_context;

	/* Validate parameters */
	if (s == NULL || digest == NULL)
		return (NULL);

	/* Compute the SHA-256 hash of a sequence of bytes */
	if (!SHA256_Init(&sha256_context))
		return (NULL);

	/* Add the data to be hashed */
	if (!SHA256_Update(&sha256_context, s, len))
		return (NULL);

	/* Finalize the hash and store it in @digest */
	if (!SHA256_Final(digest, &sha256_context))
		return (NULL);

	return (digest);
}
