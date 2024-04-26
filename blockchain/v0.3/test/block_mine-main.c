#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);

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
}

static block_t *_add_block(blockchain_t *blockchain, block_t const *prev,
	char const *s)
{
	block_t *block;

	block = block_create(prev, (int8_t *)s, (uint32_t)strlen(s));
	block->info.difficulty = 20;

	block_mine(block);

	if (block_is_valid(block, prev) == 0)
	{
		printf("Block mined: [%u] ", block->info.difficulty);
		_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
		printf("\n");
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	else
	{
		fprintf(stderr, "Invalid Block with index: %u\n",
			block->info.index);
	}

	return (block);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	blockchain_t *blockchain;
	block_t *block;

	blockchain = blockchain_create();
	block = llist_get_head(blockchain->chain);
	block = _add_block(blockchain, block, "Holberton");
	block = _add_block(blockchain, block, "School");
	block = _add_block(blockchain, block, "of");
	block = _add_block(blockchain, block, "Software");
	block = _add_block(blockchain, block, "Engineering");
	block = _add_block(blockchain, block, "972");
	block = _add_block(blockchain, block, "Mission");
	block = _add_block(blockchain, block, "Street");
	block = _add_block(blockchain, block, "San Francisco");
	block = _add_block(blockchain, block, "CA");
	block = _add_block(blockchain, block, "\\o/");

	_blockchain_print_brief(blockchain);

	blockchain_destroy(blockchain);

	return (EXIT_SUCCESS);
}
