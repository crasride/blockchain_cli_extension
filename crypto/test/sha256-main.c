#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hblk_crypto.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * main - Entry point
 *
 * @ac: Arguments counter
 * @av: Arguments vector
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int ac, char **av)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];
	uint8_t *test_ptr;

	if (ac < 2)
	{
		fprintf(stderr, "Usage: %s arg\n", av[0]);
		return (EXIT_FAILURE);
	}

	/* Test `sha256()` */
	test_ptr = sha256((int8_t *)av[1], strlen(av[1]), hash);
	if (!test_ptr)
	{
		fprintf(stderr, "sha256() failed\n");
		return (EXIT_FAILURE);
	}
	if (test_ptr != hash)
	{
		fprintf(stderr, "Return value and pointer differ\n");
		return (EXIT_FAILURE);
	}

	printf("\"%s\" hash is: ", av[1]);
	_print_hex_buffer(hash, SHA256_DIGEST_LENGTH);
	printf("\n");

	return (EXIT_SUCCESS);
}
