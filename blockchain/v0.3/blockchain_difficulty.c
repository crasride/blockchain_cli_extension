#include "blockchain.h"


/**
* blockchain_difficulty - get difficulty of the next block in the blockchain
* @blockchain: pointer to the blockchain
*
* Return: difficulty of the next block in the blockchain
*/
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *last_block, *adjusted_block;
	uint64_t expected_time, actual_time;
	uint32_t new_difficulty;

	if (!blockchain)
		return (0);
	/* Get the last block in the chain */
	last_block = llist_get_tail(blockchain->chain);
	if (!last_block)
		return (0);
	/*
	* the last block index is 0 or not a multiple of the adjustment interval,
	* return the difficulty of the last block
	*/
	if (last_block->info.index == 0 ||
		last_block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL)
		return (last_block->info.difficulty);
	/* Get the block at the difficulty adjustment interval */
	adjusted_block = llist_get_node_at(blockchain->chain,
		llist_size(blockchain->chain) - DIFFICULTY_ADJUSTMENT_INTERVAL);
	if (!adjusted_block)
		return (0);
	/* Compute the expected and actual time between the last two blocks */
	expected_time = DIFFICULTY_ADJUSTMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
	actual_time = last_block->info.timestamp - adjusted_block->info.timestamp;
	/*
	* Adjust the difficulty based on the difference between actual
	* and expected times
	*/
	if (actual_time < expected_time / 2)
		new_difficulty = last_block->info.difficulty + 1;
	/* Difficulty should never go below 0 */
	else if (actual_time > expected_time * 2)
		new_difficulty = last_block->info.difficulty > 0 ?
			last_block->info.difficulty - 1 : 0;
	/* If the difficulty stays the same */
	else
		new_difficulty = last_block->info.difficulty;

	return (new_difficulty);
}
