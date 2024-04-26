#include "transaction.h"

/**
* tx_in_create - Creates a transaction input structure
* @unspent: Pointer to the unspent transaction output to be converted
*           to a transaction input
*
* Return: A pointer to the created transaction input upon success,
*         or NULL upon failure
*/
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *input = calloc(1, sizeof(tx_in_t));

	if (!unspent)
		return (NULL);
	if (!input)
		return (NULL);

	memcpy(input->block_hash, unspent->block_hash, sizeof(input->block_hash));
	memcpy(input->tx_id, unspent->tx_id, sizeof(input->tx_id));
	memcpy(input->tx_out_hash, unspent->out.hash, sizeof(input->tx_out_hash));
	/* Zero out the signature */
	memset(&input->sig, 0, sizeof(input->sig));

	return (input);
}
