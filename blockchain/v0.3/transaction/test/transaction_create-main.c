#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "transaction.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	uint8_t block_hash[SHA256_DIGEST_LENGTH];
	uint8_t transaction_id[SHA256_DIGEST_LENGTH];
	tx_out_t *out;
	uint8_t pub[EC_PUB_LEN];
	EC_KEY *sender, *receiver;
	llist_t *all_unspent;
	unspent_tx_out_t *unspent;
	transaction_t *transaction;

	/* Create a mock Block hash */
	sha256((int8_t *)"Block", strlen("Block"), block_hash);
	/*
	* Create a mock Transaction hash
	* This hash represents the transaction our mock output is from, and
	* our `sender` will receive the coins from
	*/
	sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
	/*
	* In this example, `sender` will be the receiver of the transaction
	* output, and the sender of the next transaction
	*/
	sender = ec_create();
	receiver = ec_create();
	/* Create a mock transaction output to give coins to our `sender` */
	out = tx_out_create(972, ec_to_pub(sender, pub));
	unspent = unspent_tx_out_create(block_hash, transaction_id, out);
	/*
	* The list of all unspent transaction outputs is normally located
	* in our Blockchain
	*/
	all_unspent = llist_create(MT_SUPPORT_FALSE);
	llist_add_node(all_unspent, unspent, ADD_NODE_REAR);

	/* Create transaction */
	transaction = transaction_create(sender, receiver, 500, all_unspent);
	_transaction_print(transaction);

	/* Cleanup */
	EC_KEY_free(sender);
	EC_KEY_free(receiver);
	free(out);
	llist_destroy(all_unspent, 1, free);
	return (EXIT_SUCCESS);
}
