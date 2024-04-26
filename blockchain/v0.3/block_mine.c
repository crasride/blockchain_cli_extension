#include "blockchain.h"

/**
* block_mine - Mines a Block to insert it in the Blockchain
* @block: Pointer to the Block to be mined
*/
void block_mine(block_t *block)
{
	/* nonce is a 64-bit number that is typically used once */
	uint64_t nonce = 0;
	/* hash is a 256-bit number */
	uint8_t hash[SHA256_DIGEST_LENGTH];

	while (1)
	{
		/* Update the block's info with the new nonce */
		block->info.nonce = nonce;
		/* Compute the hash of the block */
		block_hash(block, hash);
		/* Check if the hash matches the difficulty */
		if (hash_matches_difficulty(hash, block->info.difficulty))
			break;
		/* Increment the nonce */
		nonce++;
	}
	/* Copy the resulting hash to the block's data */
	memcpy(block->hash, hash, SHA256_DIGEST_LENGTH);
}
