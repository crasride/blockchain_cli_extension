#include "cli.h"

/**
* handle_mine - mine a block
* @state: cli state
* Return: 0 if ok 1 if fail
*/
int handle_mine(state_t *state)
{
	int8_t block_data[1024] = {0};

	block_t *block = NULL;
	block_t *prev_block = llist_get_tail(state->blockchain->chain);
	transaction_t *coinbase_tx = NULL;

	if (!state->wallet)
	{
		fprintf(stderr, "mine: No wallet available\n");
		return (-1);
	}
	block = block_create(prev_block, block_data, 1024);
	if (!block)
	{
		fprintf(stderr, "mine: failed to create block\n");
		return (-1);
	}

	coinbase_tx = coinbase_create(state->wallet, block->info.index);
	if (!coinbase_tx)
	{
		fprintf(stderr, "mine: failed to create coinbase transaction\n");
		block_destroy(block);
		return (-1);
	}
	if (!coinbase_is_valid(coinbase_tx, block->info.index))
	{
		fprintf(stderr,
			"mine: failed to create valid coinbase transaction\n");
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return (-1);
	}

	return (mine(state, block, prev_block, coinbase_tx));
}
