#include "transaction.h"

/**
* check_unspent - checks if unspent transaction output is used in a transaction
* @node: current node
* @idx: index of @node
* @args: arguments:
* args[0] = unspent_tx_out_t *unspent (unspent transaction output to check)
* args[1] = int *used(1 if the unspent transaction output is used, 0 otherwise)
* Return: 0 on success, 1 on failure
*/
int check_unspent(llist_node_t node, unsigned int idx, void *args)
{
	transaction_t *tx = node;
	unspent_tx_out_t *unspent = args;
	unsigned int i;

	(void)idx; /* Avoid the warning "unused parameter" */

	for (i = 0; i < (unsigned int)llist_size(tx->inputs); i++)
	{
		tx_in_t *tx_in = llist_get_node_at(tx->inputs, i);

		if (!memcmp(tx_in->block_hash, unspent->block_hash,
												sizeof(SHA256_DIGEST_LENGTH))
			&& !memcmp(tx_in->tx_id, unspent->tx_id, sizeof(SHA256_DIGEST_LENGTH))
			&& !memcmp(tx_in->tx_out_hash, unspent->out.hash,
													sizeof(SHA256_DIGEST_LENGTH)))
			return (1);
	}
	return (0);
}

/**
* used_in_pool - checks if an unspent transaction output used in a transaction
* @unspent: unspent transaction output to check
* @tx_pool: list of used unspent transactions
* Return: 1 if unspent transaction output is used in a transaction, 0 otherwise
*/
int used_in_pool(llist_t *tx_pool, unspent_tx_out_t *unspent)
{
	if (!tx_pool)
	{
		printf("tx_pool is NULL\n");
		return (0);
	}
	if (llist_for_each(tx_pool, check_unspent, unspent) != 0)
		return (1);
	return (0);
}
/**
* select_unspent_in - selects unspent transactions that match the sender's
* public key
* @node: current node
* @idx: index of @node
* @args: arguments:
* args[0] = uint8_t const pub[EC_PUB_LEN] (public key of the sender)
* args[1] = llist_t *inputs (list to store selected transaction inputs)
* args[2] = uint32_t *amount (total amount of selected unspent transactions)
* Return: 0 on success, 1 on failure
*/
int select_unspent_in(llist_node_t node, unsigned int idx, void *args)
{
	unspent_tx_out_t *unspent = node;/* Unspent transaction output */
	void **ptr = args;/* Array of arguments */
	tx_in_t *tx_in;/* Transaction input */
	llist_t *inputs = ptr[1];/* List to store selected transaction inputs */
	uint32_t *amount = ptr[2];/* Total amount of selected unspent transactions */
	llist_t *tx_pool = ptr[3];/* List of used unspent transactions */

	(void)idx; /* Avoid the warning "unused parameter" */
	/* Check public key of the unspent trans matches the sender's public key*/
	if (!memcmp(unspent->out.pub, ptr[0],
								EC_PUB_LEN) && !used_in_pool(tx_pool, unspent))
	{
		/* Create transaction input from selected unspent transaction output */
		tx_in = tx_in_create(unspent);
		/* Add the transaction input to the list */
		llist_add_node(inputs, tx_in, ADD_NODE_REAR);
		/* Update the total amount of selected unspent transactions */
		*amount += unspent->out.amount;
	}
	return (0);
}

/**
* sign_transaction_inputs - signs transaction inputs
* @node: current node
* @idx: index of @node
* @args: arguments:
* args[0] = uint8_t tx_id[SHA256_DIGEST_LENGTH] (ID of the transaction)
* args[1] = EC_KEY const *sender (sender's private key)
* args[2] = llist_t *all_unspent (list of all unspent transactions)
* Return: 0 on success, 1 on failure
*/
int sign_transaction_inputs(llist_node_t node, unsigned int idx, void *args)
{
	void **ptr = args;
	tx_in_t *tx_in = node;

	(void)idx; /* Avoid the warning "unused parameter" */

	/* Sign the transaction input using the sender's private key */
	if (!tx_in_sign(tx_in, ptr[0], ptr[1], ptr[2]))
		return (1);

	return (0);
}

/**
* send_amount - Send a specified amount from sender to receiver.
*
* @sender: Key of the sender.
* @receiver: Key of the receiver.
* @amount: Amount to send to the receiver.
* @total: Total balance of the sender.
*
* Return: A list of transaction outputs or NULL on failure.
*/
llist_t *send_amount(EC_KEY const *sender, EC_KEY const *receiver,
					int32_t amount, uint32_t total)
{
	llist_t *transaction_outputs = NULL; /* List to store transaction outputs */
	uint32_t remaining_balance = total - amount; /* Calculate remaining balance */
	uint8_t receiver_pub_key[EC_PUB_LEN]; /* Buffer for receiver's public key */
	tx_out_t *output_node = NULL; /* Pointer to a transaction output node */

	/* Create a list to store transaction outputs */
	transaction_outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction_outputs)
		/* Failed to create the list, return NULL */
		return (NULL);

	/* Create a transaction output for the specified amount */
	ec_to_pub(receiver, receiver_pub_key);/* Get the public key of the receiver */
	output_node = tx_out_create(amount, receiver_pub_key); /* Create the output */
	if (!output_node)
	{
		/* Failed to create the output, clean up and return NULL */
		llist_destroy(transaction_outputs, 1, NULL);
		return (NULL);
	}
	/* Add the output to the list */
	llist_add_node(transaction_outputs, output_node, ADD_NODE_REAR);
	/* If there is a remaining balance, create another transaction output */
	if (remaining_balance != 0)
	{
		uint8_t sender_pub_key[EC_PUB_LEN]; /* Buffer for sender's public key */
		/* Get the public key of the sender */
		ec_to_pub(sender, sender_pub_key);
		/* Create a transaction output for the remaining balance */
		output_node = tx_out_create(remaining_balance, sender_pub_key);
		if (!output_node)
		{
			/* Failed to create the output, clean up and return NULL */
			llist_destroy(transaction_outputs, 1, NULL);
			return (NULL);
		}
		/* Add the output to the list */
		llist_add_node(transaction_outputs, output_node, ADD_NODE_REAR);
	}
	return (transaction_outputs);
}
