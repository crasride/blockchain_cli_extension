#include "transaction.h"

/**
* transaction_create_custom - creates a transaction
* @sender: key of a sender
* @receiver: key of a receiver
* @amount: amount to send to @receiver
* @all_unspent: all unspent transactions, balance of @send
* @tx_pool: list of used unspent transactions
* @selected_unspent: list of selected unspent transactions
* Return: Pointer to transaction or NULL
*/
transaction_t *transaction_create_custom(EC_KEY const *sender,
									EC_KEY const *receiver,
									uint32_t amount, llist_t *all_unspent,
									llist_t *tx_pool, llist_t *selected_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	transaction_t *transaction;
	llist_t *tr_in = NULL, *tr_out = NULL;
	void *args[4];
	uint32_t unspent_amount = 0;
	unsigned int i = 0;

	if (!sender || !receiver || !all_unspent)
		return (NULL);
	transaction = calloc(1, sizeof(*transaction));
	if (!transaction)
		return (NULL);
	tr_in = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(sender, pub);
	args[0] = pub, args[1] = tr_in, args[2] = &unspent_amount, args[3] = tx_pool;
	for (i = 0; i < (unsigned int)llist_size(selected_unspent); i++)
	{
		unspent_tx_out_t *unspent = llist_get_node_at(selected_unspent, i);

		select_unspent_in(unspent, i, args);
		if (unspent_amount >= amount)
			break;
	}
	if (unspent_amount < amount)
	{
		free(transaction);
		return (NULL);
	}
	tr_out = send_amount(sender, receiver, amount, unspent_amount);
	if (!tr_out)
	{
		free(transaction);
		return (NULL);
	}
	transaction->inputs = tr_in, transaction->outputs = tr_out;
	transaction_hash(transaction, transaction->id);
	args[0] = transaction->id, args[1] = (void *)sender, args[2] = all_unspent;
	llist_for_each(transaction->inputs, sign_transaction_inputs, args);
	return (transaction);
}
