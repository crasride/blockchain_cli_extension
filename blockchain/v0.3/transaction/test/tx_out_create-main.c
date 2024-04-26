#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "hblk_crypto.h"
#include "transaction.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _tx_out_print - Print a transaction output
 *
 * @out: Pointer to the transaction output to print
 */
static void _tx_out_print(tx_out_t const *out)
{
	if (!out)
		return;

	printf("Transaction output: {\n");

	printf("\tamount: %u,\n", out->amount);
	printf("\tpub: ");
	_print_hex_buffer(out->pub, sizeof(out->pub));
	printf(",\n");
	printf("\thash: ");
	_print_hex_buffer(out->hash, sizeof(out->hash));
	printf("\n");

	printf("}\n");
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	EC_KEY *receiver;
	uint8_t receiver_pub[EC_PUB_LEN];
	tx_out_t *out;

	receiver = ec_create();

	out = tx_out_create(972, ec_to_pub(receiver, receiver_pub));
	_tx_out_print(out);

	free(out);
	EC_KEY_free(receiver);

	return (EXIT_SUCCESS);
}
