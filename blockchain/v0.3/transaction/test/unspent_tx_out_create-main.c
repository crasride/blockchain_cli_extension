#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _unspent_tx_out_print - Print an unspent transaction output
 *
 * @unspent: Pointer to the unspent transaction output to print
 */
static void _unspent_tx_out_print(unspent_tx_out_t const *unspent)
{
	if (!unspent)
		return;

	printf("Unspent transaction output: {\n");

	printf("\tblock_hash: ");
	_print_hex_buffer(unspent->block_hash, sizeof(unspent->block_hash));
	printf(",\n");
	printf("\ttx_id: ");
	_print_hex_buffer(unspent->tx_id, sizeof(unspent->tx_id));
	printf(",\n");

	printf("\tout: {\n");

	printf("\t\tamount: %u,\n", unspent->out.amount);
	printf("\t\tpub: ");
	_print_hex_buffer(unspent->out.pub, sizeof(unspent->out.pub));
	printf(",\n");
	printf("\t\thash: ");
	_print_hex_buffer(unspent->out.hash, sizeof(unspent->out.hash));
	printf("\n");

	printf("\t}\n}\n");
}

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
	EC_KEY *receiver;
	unspent_tx_out_t *unspent;

	sha256((int8_t *)"Block", strlen("Block"), block_hash);
	sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
	receiver = ec_create();
	out = tx_out_create(972, ec_to_pub(receiver, pub));

	unspent = unspent_tx_out_create(block_hash, transaction_id, out);
	_unspent_tx_out_print(unspent);

	EC_KEY_free(receiver);
	free(out);
	free(unspent);

	return (EXIT_SUCCESS);
}
