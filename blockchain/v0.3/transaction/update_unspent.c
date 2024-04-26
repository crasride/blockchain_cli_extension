#include "transaction.h"

/**
* find_unspent - finds an unspent transaction output in a list
* @node: current node
* @hash: hash to compare
* Return: 0 if match, 1 if not
**/
int find_unspent(llist_node_t node, void *hash)
{
	/* Unspent transaction output */
	unspent_tx_out_t *unspent = (unspent_tx_out_t *)node;

	/* Compare the hash of the unspent transaction output */
	return (!memcmp(unspent->out.hash, hash, sizeof(unspent->out.hash)));
}

/**
* update_unspent - updates the list of unspent transaction outputs
* @transactions: list of transactions
* @block_hash: hash of the Block containing the transaction @tx_id
* @all_unspent: list of all unspent transaction outputs
* Return: updated list of unspent transaction outputs
**/
int update_unspent(llist_t *transactions,
				uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	int tr_index; /* Transaction index */
	int io_index; /* Input-output iterate transaction*/
	int io_count; /* Input-output count transaction*/
	int tr_count; /* Transaction count */
	transaction_t *transaction; /* Transaction */
	tx_in_t *in; /* Transaction input */
	tx_out_t *out; /* Transaction output */
	unspent_tx_out_t *unspent; /* Unspent transaction output */

	/* Check if the arguments are valid */
	if (!transactions || !block_hash || !all_unspent)
		return (1);
	/* Loop through all transactions */
	for (tr_index = 0, tr_count = llist_size(transactions); tr_index < tr_count;
			tr_index++)
	{
		transaction = llist_get_node_at(transactions, tr_index);
		/* Loop through all inputs of one transaction */
		for (io_index = 0, io_count = llist_size(transaction->inputs);
			io_index < io_count; io_index++)
		{
			in = llist_get_node_at(transaction->inputs, io_index);
			/* Remove the unspent transaction output */
			llist_remove_node(all_unspent, find_unspent, in->tx_out_hash, 1,
								(node_dtor_t)free);
		}
		/* Loop through all outputs of one transaction */
		for (io_index = 0, io_count = llist_size(transaction->outputs);
			io_index < io_count; io_index++)
		{/* Create a new unspent transaction output */
			out = llist_get_node_at(transaction->outputs, io_index);
			unspent = unspent_tx_out_create(block_hash, transaction->id, out);
			/* Add the unspent transaction output */
			llist_add_node(all_unspent, unspent, ADD_NODE_REAR);
		}
	}
	/* //TODO: ou est ce free(unspent); de m... */

	return (0);
}
