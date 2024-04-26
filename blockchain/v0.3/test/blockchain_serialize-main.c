#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

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

	block = block_create(block, (int8_t *)"Holberton", 9);
	block_hash(block, block->hash);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"School", 6);
	block_hash(block, block->hash);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"of", 2);
	block_hash(block, block->hash);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"Software", 8);
	block_hash(block, block->hash);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"Engineering", 11);
	block_hash(block, block->hash);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

	blockchain_serialize(blockchain, "save.hblk");

	blockchain_destroy(blockchain);

	return (EXIT_SUCCESS);
}
