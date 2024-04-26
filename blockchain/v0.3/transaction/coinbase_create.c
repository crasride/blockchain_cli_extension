#include "transaction.h"


int coinbase_create_continue(transaction_t *transaction, uint8_t *pub,
							uint32_t block_index);

/**
* coinbase_create - create coinbase transaction
* @receiver: coin receiver
* @block_index: block index to which tx belongs
* Return: pointer to tx struct
*/
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *transaction; /* New transaction */
	uint8_t pub[EC_PUB_LEN]; /* Buffer for public key */

	/* Check whether the receiver and public key conversion are successful */
	if (!receiver || !ec_to_pub(receiver, pub))
		return (NULL);

	/* Allocate memory for the transaction */
	transaction = calloc(1, sizeof(*transaction));
	if (!transaction)
		return (NULL);

	/* Initialize the transaction input and output lists */
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs || !transaction->outputs)
	{
		free(transaction);
		return (NULL);
	}

	/* Create the coinbase transaction */
	if (!coinbase_create_continue(transaction, pub, block_index))
	{
		free(transaction);
		return (NULL);
	}

	/* Return new coinbase transaction */
	return (transaction);
}

/**
* coinbase_create_continue - continues coinbase transaction creation
* @transaction: pointer to transaction struct
* @pub: public key buffer
* @block_index: block index to which tx belongs
* Return: 1 on success, 0 on failure
*/
int coinbase_create_continue(transaction_t *transaction, uint8_t *pub,
							uint32_t block_index)
{
	tx_out_t *output; /* New transaction output */
	tx_in_t *input; /* New transaction input */

	/* Allocate memory for transaction output */
	output = tx_out_create(COINBASE_AMOUNT, pub);
	if (!output)
		return (0);

	/* Allocate memory for transaction input */
	input = calloc(1, sizeof(*input));
	if (!input)
	{
		free(output);
		return (0);
	}
	/* Copy block index to first 4 bytes of transaction output hash */
	memcpy(input->tx_out_hash, &block_index, 4);

	/* Add input and output to corresponding transaction lists */
	if (llist_add_node(transaction->inputs, input, ADD_NODE_REAR) ||
		llist_add_node(transaction->outputs, output, ADD_NODE_REAR))
	{
		free(output);
		free(input);
		llist_destroy(transaction->inputs, 0, NULL);
		llist_destroy(transaction->outputs, 0, NULL);
		return (0);
	}

	/* Calculate transaction hash */
	if (!transaction_hash(transaction, transaction->id))
	{
		free(output);
		free(input);
		llist_destroy(transaction->inputs, 0, NULL);
		llist_destroy(transaction->outputs, 0, NULL);
		return (0);
	}
	/* Success in creating coinbase transaction */
	return (1);
}
