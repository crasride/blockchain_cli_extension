#include "blockchain.h"

/**
* blockchain_create - Creates a Blockchain structure and initializes it
*
* Return: A pointer to the created blockchain, or NULL on failure
*/
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = NULL;
	block_t *genesis_block = NULL;
	/* Allocate memory for the blockchain */
	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);
	/* Allocate memory for the genesis block */
	genesis_block = malloc(sizeof(block_t));
	if (!genesis_block)
	{
		free(blockchain);
		return (NULL);
	}
	/* Initialize the blockchain */
	blockchain->chain = llist_create(MT_SUPPORT_TRUE);
	if (!blockchain->chain)
	{
		free(blockchain);
		free(genesis_block);
		return (NULL);
	}
	/* Initialize the unspent transaction outputs list */
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->unspent)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		free(genesis_block);
		return (NULL);
	}
	/* Initialize the block */
	memset(genesis_block, 0, sizeof(block_t));/* Initialize the genesis block */
	genesis_block->info.index = 0;
	genesis_block->info.difficulty = 0;
	genesis_block->info.timestamp = 1537578000;
	genesis_block->info.nonce = 0;
	memset(genesis_block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);
	memcpy(genesis_block->data.buffer, "Holberton School", 16);
	genesis_block->data.len = 16;
	memcpy(genesis_block->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);
	/* Add the genesis block to the blockchain */
	if (llist_add_node(blockchain->chain, genesis_block, ADD_NODE_REAR) == -1)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		free(genesis_block);
		return (NULL);
	}

	return (blockchain);
}
