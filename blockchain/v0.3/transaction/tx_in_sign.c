#include "transaction.h"

/**
* tx_in_sign - signs a transaction input
* @in: pointer to the transaction input to sign
* @tx_id: hash of the transaction containing @in
* @sender: private key of the receiver of the referenced transaction output
* @all_unspent: list of all unspent transaction outputs to date
*
* Return: pointer to the resulting signature structure or NULL
*/
sign_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
					EC_KEY const *sender, llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	ssize_t index;
	unspent_tx_out_t *unspent_output;
	/* Verify that input parameters are not NULL */
	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);
	 /* Find the unspent output corresponding to the transaction input */
	for (index = 0; index < llist_size(all_unspent); index++)
	{
		unspent_output = llist_get_node_at(all_unspent, index);
		/* If the current unspent output is NULL, return NULL */
		if (!unspent_output)
			return (NULL);
		/* Check if the current unspent output matches the input's reference */
		if (!memcmp(in->tx_out_hash, unspent_output->out.hash,
			SHA256_DIGEST_LENGTH))
			break;
		/* Reset unspent_output to NULL if no match found */
		unspent_output = NULL;
	}
	/* If no matching unspent output was found, return NULL */
	if (!unspent_output)
		return (NULL);/* If the unspent output is NULL */
	/* Get the public key of the sender */
	if (!ec_to_pub(sender, pub) || memcmp(pub, unspent_output->out.pub,
		EC_PUB_LEN))
		return (NULL);/* If the public key of the sender is invalid */
	/* Sign the transaction input using the sender's private key */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);/* If the transaction input could not be signed */
	/* Return the resulting signature */
	return (&in->sig);
}
