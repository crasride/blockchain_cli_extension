#include "transaction.h"

/**
 * print_utxo_message - print message for lack of utxo
 */
void print_utxo_message(void)
{
	printf(C_RED"\n==========================================================\n");
	printf("    ============ info message ===============    \n");
	printf("==========================================================\n\n"
			C_RESET);
	printf("Not enough amount in remaining utxo, if you have change\n");
	printf("in transactions that are not yet mined you will be able to\n");
	printf("send this amount after those transactions have been mined\n");
}

/**
* transaction_create - creates a transaction
* @sender: key of a sender
* @receiver: key of a receiver
* @amount: amount to send to @receiver
* @unspents: all unspent transactions, balance of @send
* @tx_pool: list of used unspent transactions
* @tr_out: list of new tx_out to be created
* Return: a new transaction or NULL on failure
*/
transaction_t *transaction_create(EC_KEY const *sender,
									EC_KEY const *receiver,
									uint32_t amount, llist_t *unspents,
									llist_t *tx_pool, llist_t *tr_out)
{
	uint8_t pub[EC_PUB_LEN];
	transaction_t *transaction;
	llist_t *tr_in = NULL;
	void *args[4];
	uint32_t unspent_amount = 0;
	unsigned int i = 0;

	if (!sender || !receiver || !unspents)
		return (NULL);
	transaction = calloc(1, sizeof(*transaction));
	if (!transaction)
		return (NULL);
	tr_in = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(sender, pub);
	args[0] = pub, args[1] = tr_in, args[2] = &unspent_amount, args[3] = tx_pool;
	for (i = 0; i < (unsigned int)llist_size(unspents); i++)
	{
		unspent_tx_out_t *unspent = llist_get_node_at(unspents, i);

		select_unspent_in(unspent, i, args);
		if (!tr_out && unspent_amount >= amount)
			break;
	}
	if (unspent_amount < amount)
	{
		print_utxo_message();
		free(transaction);
		return (NULL);
	}
	if (!tr_out)
	{
		tr_out = send_amount(sender, receiver, amount, unspent_amount);
		if (!tr_out)
		{
			free(transaction);
			return (NULL);
		}
	}
	transaction->inputs = tr_in, transaction->outputs = tr_out;
	transaction_hash(transaction, transaction->id);
	args[0] = transaction->id, args[1] = (void *)sender, args[2] = unspents;
	llist_for_each(transaction->inputs, sign_transaction_inputs, args);
	return (transaction);
}
