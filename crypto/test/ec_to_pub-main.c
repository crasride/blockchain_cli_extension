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
