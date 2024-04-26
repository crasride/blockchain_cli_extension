#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);

static block_t *_add_block(blockchain_t *blockchain, block_t const *prev,
	char const *s)
{
	block_t *block;

	usleep(300000); /* Simulate block mining ... */
	block = block_create(prev, (int8_t *)s, (uint32_t)strlen(s));

	block->info.difficulty = blockchain_difficulty(blockchain);

	block_hash(block, block->hash);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

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
