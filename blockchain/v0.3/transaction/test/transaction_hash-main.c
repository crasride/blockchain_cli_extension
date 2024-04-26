#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

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
	tx_out_t *out, *out2;
	uint8_t pub[EC_PUB_LEN];
	EC_KEY *owner;
	llist_t *all_unspent;
	unspent_tx_out_t *unspent;
	tx_in_t *in;
	transaction_t transaction;

	/* Create a mock Block hash */
	sha256((int8_t *)"Block", strlen("Block"), block_hash);
	/*
	* Create a mock Transaction hash
	* This hash represents the transaction our mock output is from, and
	* our `owner` will receive the coins from
	*/
	sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
	/*
	* In this example, `owner` will be the receiver of the transaction
	* output, and the sender of the transaction input we want to sign
	*/
	owner = ec_create();
	/* Create a mock transaction output to give coins to our `owner` */
	out = tx_out_create(972, ec_to_pub(owner, pub));
	unspent = unspent_tx_out_create(block_hash, transaction_id, out);
	/*
	* The list of all unspent transaction outputs is normally located
	* in our Blockchain
	*/
	all_unspent = llist_create(MT_SUPPORT_FALSE);
	llist_add_node(all_unspent, unspent, ADD_NODE_REAR);

	transaction.inputs = llist_create(MT_SUPPORT_FALSE);
	transaction.outputs = llist_create(MT_SUPPORT_FALSE);
	/* Create the transaction input from the unspent transaction output */
	in = tx_in_create(unspent);
	llist_add_node(transaction.inputs, in, ADD_NODE_REAR);
	pub[1] *= 2; /* Pretend it's a different address :) */
	out2 = tx_out_create(972, pub);
	llist_add_node(transaction.outputs, out2, ADD_NODE_REAR);

	/* Compute transaction id (hash) */
	transaction_hash(&transaction, transaction.id);
	_transaction_print(&transaction);

	/* Cleanup */
	EC_KEY_free(owner);
	free(out);
	llist_destroy(all_unspent, 1, free);
	llist_destroy(transaction.inputs, 1, free);
	llist_destroy(transaction.outputs, 1, free);
	return (EXIT_SUCCESS);
}
