#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);
void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _print_unspent - Print an unspent transaction output
 *
 * @unspent: Pointer to the unspent transaction output to print
 * @idx:     Unused
 * @indent:  Indentation
 *
 * Return: 0
 */
static int _print_unspent(unspent_tx_out_t const *unspent, unsigned int idx,
	char const *indent)
{
	printf("%s{\n", indent);

	printf("%s\tblock_hash: ", indent);
	_print_hex_buffer(unspent->block_hash, SHA256_DIGEST_LENGTH);
	printf("\n");

	printf("%s\ttx_id: ", indent);
	_print_hex_buffer(unspent->tx_id, SHA256_DIGEST_LENGTH);
	printf("\n");

	printf("%s\tamount: %u\n", indent, unspent->out.amount);

	printf("%s\tpub: ", indent);
	_print_hex_buffer(unspent->out.pub, EC_PUB_LEN);
	printf("\n");

	printf("%s}\n", indent);

	(void)idx;
	return (0);
}

/**
 * _print_all_unspent - Print a list of unspent transaction outputs
 *
 * @unspent: List of unspent transaction outputs to print
 */
static void _print_all_unspent(llist_t *unspent)
{
	printf("Unspent transaction outputs [%u]: [\n", llist_size(unspent));

	llist_for_each(unspent, (node_func_t)_print_unspent, "\t");

	printf("]\n");
}

/**
 * _add_block - Add a block to a blockchain
 *
 * @blockchain: Pointer to the Blockchain to add the Block to
 * @prev:       Pointer to the previous Block in the chain
 * @data:       Data buffer to be put in the Block
 * @miner:      EC key of the miner (receiver of the coinbase transaction)
 *
 * Return: A pointer to the created Block
 */
static block_t *_add_block(blockchain_t *blockchain, block_t const *prev,
	char const *data, EC_KEY *miner)
{
	block_t *block;
	transaction_t *coinbase;

	block = block_create(prev, (int8_t *)data, (uint32_t)strlen(data));
	block->info.difficulty = 20;

	coinbase = coinbase_create(miner, block->info.index);
	llist_add_node(block->transactions, coinbase, ADD_NODE_FRONT);

	block_mine(block);

	if (block_is_valid(block, prev, blockchain->unspent) == 0)
	{
		printf("Block mined: [%u] ", block->info.difficulty);
		_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
		printf("\n");

		/* Update all unspent */
		blockchain->unspent = update_unspent(block->transactions,
			block->hash, blockchain->unspent);

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
	EC_KEY *miner;

	miner = ec_create();
	blockchain = blockchain_create();

	block = llist_get_head(blockchain->chain);
	block = _add_block(blockchain, block, "Holberton", miner);
	_print_all_unspent(blockchain->unspent);
	block = _add_block(blockchain, block, "School", miner);
	_print_all_unspent(blockchain->unspent);

	_blockchain_print_brief(blockchain);

	blockchain_destroy(blockchain);
	EC_KEY_free(miner);

	return (EXIT_SUCCESS);
}
