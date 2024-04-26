#include <stdlib.h>

#include "transaction.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	transaction_t *coinbase;
	EC_KEY *owner;

	owner = ec_create();
	coinbase = coinbase_create(owner, 1);
	transaction_destroy(coinbase);

	/* Cleanup */
	EC_KEY_free(owner);

	return (EXIT_SUCCESS);
}
