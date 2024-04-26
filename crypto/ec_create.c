#include "hblk_crypto.h"

/**
* ec_create - Creates a new EC key pair using the secp256k1 curve
*
* Return: Pointer to an EC_KEY structure (containing public and private keys)
* NULL on failure
*/
EC_KEY *ec_create(void)
{
	EC_GROUP *group = EC_GROUP_new_by_curve_name(NID_secp256k1);
	EC_KEY *key = EC_KEY_new();

	/* EC_GROUP_new_by_curve_name - create a new EC_GROUP structure */
	if (!key)
	{
		fprintf(stderr, "Failed to create EC key\n");
		return (NULL);
	}
	/* EC_KEY_set_group - set the group of an EC_KEY object */
	if (!group)
	{
		fprintf(stderr, "Failed to create EC group\n");
		EC_KEY_free(key);
		return (NULL);
	}
	/* EC_KEY_set_group - set the group of an EC_KEY object */
	if (EC_KEY_set_group(key, group) != 1)
	{
		fprintf(stderr, "Failed to set EC group\n");
		EC_KEY_free(key);
		EC_GROUP_free(group);
		return (NULL);
	}
	EC_GROUP_free(group);
	/* EC_KEY_generate_key - generate a new public and private key */
	if (!EC_KEY_generate_key(key))
	{
		fprintf(stderr, "Failed to generate EC key pair\n");
		EC_KEY_free(key);
		return (NULL);
	}
	/* EC_KEY_check_key - check key */
	if (!EC_KEY_check_key(key))
	{
		fprintf(stderr, "Key verification failed\n");
		EC_KEY_free(key);
		return (NULL);
	}
	return (key);
}
