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

	blockchain = blockchain_deserialize("save.hblk");

	_blockchain_print_brief(blockchain);
	blockchain_destroy(blockchain);

	return (EXIT_SUCCESS);
}
