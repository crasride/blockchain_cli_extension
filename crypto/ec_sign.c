#include "hblk_crypto.h"

/**
* ec_sign - Sign a set of bytes using a given private EC_KEY
*
* @key: Pointer to the EC_KEY struct containing the private key for signing
* @msg: Pointer to characters to be signed
* @msglen: Length of msg
* @sig: Address to store the signature
*
* Return: Pointer to the signature buffer on success, NULL on error
*/
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
				size_t msglen, sign_t *sig)
{
	uint32_t signature_len = 0;

	/* Check for NULL pointers */
	if (!key || !msg || !sig)
		return (NULL);

	/* Initialize the signature buffer to zeros */
	memset(sig->sig, 0, sizeof(sig->sig));

	/* Sign the message using ECDSA */
	if (ECDSA_sign(0, msg, msglen, sig->sig, &signature_len, (EC_KEY *)key) != 1)
		return (NULL);

	/* Set the length of the signature */
	sig->len = signature_len;

	/* Return a pointer to the signature buffer */
	return (sig->sig);
}
