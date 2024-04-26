#include "hblk_crypto.h"

/**
* ec_verify - Verify the signature of a given set of bytes
* using a given EC_KEY public key
* @key: Pointer to the EC_KEY structure containing the public key
* @msg: Pointer to the characters to verify the signature of
* @msglen: Length of the message
* @sig: Pointer to the signature to be checked
*
* Return: 1 if the signature is valid, 0 otherwise
*/
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
				sign_t const *sig)
{
	/* Check for NULL pointers */
	if (!key || !msg || !sig)
		return (0);

	/* Verify the key */
	if (!EC_KEY_check_key(key))
		return (0);

	/* Verify the signature using ECDSA_verify */
	if (ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key) != 1)
		return (0);

	return (1);
}
