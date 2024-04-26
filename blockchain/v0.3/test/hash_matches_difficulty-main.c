#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

/**
 * _print_hex_buffer - Prints a buffer in its hexadecimal form
 *
 * @buf: Pointer to the buffer to be printed
 * @len: Number of bytes from @buf to be printed
 */
static void _print_hex_buffer(uint8_t const *buf, size_t len)
{
	size_t i;

	for (i = 0; buf && i < len; i++)
		printf("%02x", buf[i]);

	fflush(NULL);
}

/**
 * test_hash_matches_difficulty - Test the hash_matches_difficulty function
 *
 * @hash:       Tested hash
 * @difficulty: Tested difficulty
 */
void test_hash_matches_difficulty(uint8_t const *hash, uint32_t difficulty)
{
	printf("Hash: ");
	_print_hex_buffer(hash, SHA256_DIGEST_LENGTH);
	printf("\nDifficulty: %u\n", difficulty);
	if (hash_matches_difficulty(hash, difficulty))
		printf("Matches difficulty\n");
	else
		printf("Doesn't match difficulty\n");
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];
	uint32_t difficulty;

	/* Test case 1 */
	memset(hash, 0, SHA256_DIGEST_LENGTH);
	hash[31] = 1;
	difficulty = SHA256_DIGEST_LENGTH * 8 - 1;
	test_hash_matches_difficulty(hash, difficulty);

	/* Test case 2 */
	memset(hash, 0xFF, SHA256_DIGEST_LENGTH);
	hash[0] = 0;
	hash[1] = 98;
	difficulty = 9;
	test_hash_matches_difficulty(hash, difficulty);

	/* Test case 3 */
	memset(hash, 0xFF, SHA256_DIGEST_LENGTH);
	hash[0] = 0;
	hash[1] = 98;
	difficulty = 10;
	test_hash_matches_difficulty(hash, difficulty);

	return (EXIT_SUCCESS);
}
