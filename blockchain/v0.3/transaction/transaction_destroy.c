#include "transaction.h"

/**
* transaction_destroy - deletes the transaction structure
* @transaction: pointer to the transaction structure to delete
*/
void transaction_destroy(transaction_t *transaction)
{
	/* Check if the transaction pointer is NULL */
	if (!transaction)
		return;
	/* Destroy the transaction inputs and outputs */
	llist_destroy(transaction->inputs, 1, NULL);
	/* Destroy the transaction outputs */
	llist_destroy(transaction->outputs, 1, NULL);
	/* Free the transaction structure */
	free(transaction);
}
