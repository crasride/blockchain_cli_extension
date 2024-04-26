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
