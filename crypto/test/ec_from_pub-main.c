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
