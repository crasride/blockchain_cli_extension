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
