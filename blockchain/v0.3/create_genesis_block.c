#include "blockchain.h"


/**
* print_hash - Print a hash
* @hash: Hash to print
*/
void print_hash(const uint8_t *hash)
{
	int i;

	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		printf("%02x", hash[i]);
	}
}

/**
* create_genesis_block - Create a genesis block
* Return: A pointer to the genesis block
*/
block_t *create_genesis_block(void)
{
	block_t *genesis_block = malloc(sizeof(block_t));

	if (!genesis_block)
		return (NULL);

	/*Initialize the genesis block*/
	memset(genesis_block, 0, sizeof(block_t));
	genesis_block->info.index = 0;
	genesis_block->info.difficulty = 0;
	genesis_block->info.timestamp = (int64_t)time(NULL);
	genesis_block->info.nonce = 0;
	memset(genesis_block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);
	memcpy(genesis_block->data.buffer,
							"This block has been created for the occasion", 44);
	genesis_block->data.len = 44;

	/*Initialize transaction list*/
	genesis_block->transactions = llist_create(MT_SUPPORT_FALSE);

	/*Calculate the hash of the genesis block*/
	block_hash(genesis_block, genesis_block->hash);

	return (genesis_block);
}

/*
int main(void)
{

	block_t *genesis_block = create_genesis_block();

	if (!genesis_block)
	{
		fprintf(stderr, "Error al crear el bloque génesis\n");
		return (1);
	}


	printf("Hash del bloque génesis: ");
	print_hash(genesis_block->hash);
	printf("\n");


	free(genesis_block);

	return (0);
}
 */

