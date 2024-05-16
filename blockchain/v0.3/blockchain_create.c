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
	/* genesis_block = malloc(sizeof(block_t)); */
	/* Initialize the blockchain */
	blockchain->chain = llist_create(MT_SUPPORT_TRUE);
	if (!blockchain->chain)
	{
		free(blockchain);
		/* free(genesis_block); */
		return (NULL);
	}
	/* Initialize the unspent transaction outputs list */
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->unspent)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		/* free(genesis_block); */
		return (NULL);
	}

	/* create genesis block*/
	genesis_block = create_genesis_block();
	if (!genesis_block)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		llist_destroy(blockchain->unspent, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	/* Add the genesis block to the blockchain */
	if (llist_add_node(blockchain->chain, genesis_block, ADD_NODE_REAR) == -1)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		llist_destroy(blockchain->unspent, 1, NULL);
		free(blockchain);
		free(genesis_block);
		return (NULL);
	}

	return (blockchain);
}
