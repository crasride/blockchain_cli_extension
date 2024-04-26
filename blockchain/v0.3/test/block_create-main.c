#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_destroy(blockchain_t *blockchain);

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
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	block = block_create(block, (int8_t *)"School", 6);
	llist_add_node(blockchain->chain, block, ADD_NODE_REAR);

	_blockchain_print(blockchain);
	_blockchain_destroy(blockchain);

	return (EXIT_SUCCESS);
}
