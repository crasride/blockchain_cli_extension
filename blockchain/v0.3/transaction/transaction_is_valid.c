#include "transaction.h"

/**
* in_out_match - Checks wheter the tx inputs matches any
* tx output in all_unspent list
* @tx_input: transaction input
* @utxo: transaction output
* Return: 0 for no match, 1 if match is found
*/

int in_out_match(tx_in_t *tx_input, unspent_tx_out_t *utxo)
{
	/* Check if the tx input matches the tx output */
	if (memcmp(tx_input->tx_out_hash, utxo->out.hash, SHA256_DIGEST_LENGTH))
		return (0);
	/* Check if the tx ID and block hash match */
	if (memcmp(tx_input->tx_id, utxo->tx_id, SHA256_DIGEST_LENGTH))
		return (0);
	/* Check if the block hash matches */
	if (memcmp(tx_input->block_hash, utxo->block_hash, SHA256_DIGEST_LENGTH))
		return (0);
	return (1);
}

/**
* transaction_is_valid - Checks whether a transaction is valid
* @transaction: Points to the tx to verify
* @all_unspent: List of all unspent tx outputs to date
* Return: 1 if tx is valid, 0 otherwise
* Steps of verification:
* -- Computed hash of tx must match hash stored in it
* -- Each input in tx must refer to an unspent output in `all_unspent`
* -- Each input's sig must be verified using pubkey stored in ref utxo
* -- Total amount of inputs must match the total amount of ouputs
*/

int transaction_is_valid(transaction_t const *transaction,
						 llist_t *all_unspent)
{
	uint8_t hash_buff[SHA256_DIGEST_LENGTH] = {0}; /* to rehash tx and compare */
	tx_in_t *in_node = NULL;/* to iterate through tx inputs */
	unspent_tx_out_t *out_node = NULL;/* to iterate through all_unspent */
	EC_KEY *unspent_key = NULL;/* to verify each input's sig */
	int i, j, unspent_len = llist_size(all_unspent);
	uint32_t amount_in = 0, amount_out = 0;/* to compare total amounts */

	if (!transaction || !all_unspent)
		return (0);
	transaction_hash(transaction, hash_buff);/* Compute the hash of the tx */
	if (memcmp(transaction->id, hash_buff, SHA256_DIGEST_LENGTH))
		return (0); /* hash && computed hash conflict */
	/* Loop through all inputs & outputs */
	for (i = 0; i < llist_size(transaction->inputs); i++)
	{ /* Find the input in the list of unspent outputs */
		in_node = llist_get_node_at(transaction->inputs, i);
		for (j = 0; j < unspent_len; j++)
		{/* Get the unspent output */
			out_node = llist_get_node_at(all_unspent, j);
			/* Check if the tx input matches the tx output */
			if (in_out_match(in_node, out_node))
				break;
		}
		if (j == unspent_len)
			return (0); /* We iterated to the max and found nothing */
		/* Using the pub key stored in ref unspent, verify each input's sig */
		unspent_key = ec_from_pub(out_node->out.pub);
		if (!ec_verify(unspent_key, transaction->id, SHA256_DIGEST_LENGTH,
					&in_node->sig))
		{
			EC_KEY_free(unspent_key);
			return (0);
		}
		EC_KEY_free(unspent_key);
		amount_in += out_node->out.amount;
	}
	for (i = 0; i < llist_size(transaction->outputs); i++)
		amount_out +=
			((tx_out_t *)llist_get_node_at(transaction->outputs, i))->amount;
	return (amount_in == amount_out);
}
