#include "transaction.h"

/**
* unspent_tx_out_create - Creates an unspent transaction output structure
* @block_hash: Hash of the Block containing the transaction output
* @tx_id: Hash of the referenced transaction
* @out: Address of the referenced transaction output
*
* Return: A pointer to the allocated unspent transaction output
*/
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out)
{
	unspent_tx_out_t *unspent;

	if (!block_hash || !tx_id || !out)
		return (NULL);
	unspent = calloc(1, sizeof(unspent_tx_out_t));
	if (!unspent)
		return (NULL);
	memcpy(unspent->block_hash, block_hash, sizeof(unspent->block_hash));
	memcpy(unspent->tx_id, tx_id, sizeof(unspent->tx_id));
	memcpy(&unspent->out, out, sizeof(unspent->out));

	return (unspent);
	/* //TODO: */
	/* 2 output avec meme receiver, meme amount, (donc meme output hash) */
	/* qui vont dans la meme transaction (donc meme tx_id) et donc meme */
	/* block_hash (car dans le meme tx): Ils sont donc complètement jumeaux ! */
	/* Comment les différencier ??? */
}
