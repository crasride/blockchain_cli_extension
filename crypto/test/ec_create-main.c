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
