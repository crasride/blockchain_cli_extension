#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	blockchain_t *blockchain;
	block_t *block;
	EC_KEY *owner;

	owner = ec_create();
	blockchain = blockchain_create();
	block = llist_get_head(blockchain->chain);

	block = block_create(block, (int8_t *)"Holberton", 9);
	llist_add_node(block->transactions,
		coinbase_create(owner, block->info.index),
		ADD_NODE_REAR);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	_blockchain_print_brief(blockchain);

	block_hash(block, block->hash);
	_blockchain_print_brief(blockchain);

	blockchain_destroy(blockchain);
	EC_KEY_free(owner);
	return (EXIT_SUCCESS);
}
