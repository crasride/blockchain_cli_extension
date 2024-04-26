#include "transaction.h"

/**
* is_empty - checks if buffer memory is all 0
* @buf: the buffer to check
* @size: size of buffer
* Return: 1 if empty else 0
*/
int is_empty(uint8_t *buf, size_t size)
{
	/* Check if buffer memory is all 0 */
	size_t i;

	/* Iterate through buffer */
	for (i = 0; i < size; i++)
	{
		/* If not 0, return 0 */
		if (buf[i] != 0)
		{
			return (0);
		}
	}
	/* If all 0, return 1 */
	return (1);
}

/**
* coinbase_is_valid - validates coinbase tx
* @coinbase: the tx to validate
* @block_index: index of block containing tx
* Return: 1 if valid else 0
*/
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t hash[SHA256_DIGEST_LENGTH]; /* Hash buffer */
	tx_in_t *tr_coin_input; /* Transaction input */
	tx_out_t *tr_coin_output;	/* Transaction output */

	/* Check if coinbase transaction is valid */
	if (!coinbase)
		return (0);

	/* Compute the hash of the coinbase transaction */
	if (!transaction_hash(coinbase, hash) ||
		memcmp(coinbase->id, hash, SHA256_DIGEST_LENGTH))
		return (0);

	/* Check if coinbase transaction has exactly 1 input and 1 output */
	if (llist_size(coinbase->inputs) != 1 ||
		llist_size(coinbase->outputs) != 1)
		return (0);

	/* Get the input and output of the coinbase transaction */
	tr_coin_input = llist_get_node_at(coinbase->inputs, 0);
	tr_coin_output = llist_get_node_at(coinbase->outputs, 0);
	/* Verify that input and output are not NULL */
	if (!tr_coin_input || !tr_coin_output)
		return (0);

	/* Check if the input's tx_out_hash first 4 bytes match the block_index */
	if (memcmp(tr_coin_input->tx_out_hash, &block_index, 4))
		return (0);

	/* Check if input's block_hash, tx_id, and signature are all zeros */
	if (!is_empty(tr_coin_input->block_hash, sizeof(tr_coin_input->block_hash)) ||
		!is_empty(tr_coin_input->tx_id, sizeof(tr_coin_input->tx_id)) ||
		!is_empty((uint8_t *)&tr_coin_input->sig, sizeof(tr_coin_input->sig)))
		return (0);

	/* Check if the output amount is exactly COINBASE_AMOUNT */
	if (tr_coin_output->amount != COINBASE_AMOUNT)
		return (0);
	/* Coinbase transaction is valid */
	return (1);
}
