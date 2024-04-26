#include "cli.h"


/**
* handle_info_block - handle the info_block command
* @state: CLI state
* Return: 0 if success, -1 if failed
*/
/**
* handle_info_block - handle the info_block command
* @state: CLI state
* Return: 0 if success, -1 if failed
*/
int handle_info_block(state_t *state)
{
	blockchain_t *blockchain = state->blockchain;
	llist_t *chain = blockchain->chain;
	block_t *block = NULL;
	unsigned int num_blocks;
	unsigned int i;
	unsigned int num_transactions;
	size_t block_size;
	size_t transactions_size;
	size_t total_size;
	void *arg_tx[2];

	arg_tx[1] = (void *)blockchain;

	if (!chain)
	{
		printf(C_RED "No blocks in the blockchain\n" C_RESET);
		return (-1);
	}


	num_blocks = llist_size(chain);
	for (i = 0; i < num_blocks; i++)
	{
		block = (block_t *)llist_get_node_at(chain, i);
		if (block != NULL)
		{
			arg_tx[0] = (void *)block;
			printf("\n");
			printf(C_GREEN"\n======================================================\n");
			printf("   ============ Infos for block %u ===============   \n",block->info.index);
			printf("======================================================\n\n"C_RESET);
			printf("\n");
			printf("Block Index: " C_GREEN "%u" C_RESET "\n", block->info.index);
			printf("Timestamp: " C_GREEN "%lu" C_RESET "\n", block->info.timestamp);
			printf("Previous Block Hash: "C_GREEN "%s" C_RESET "\n", bytes_to_hex(block->info.prev_hash, SHA256_DIGEST_LENGTH));
			printf("Block Hash: " C_GREEN "%s" C_RESET "\n", bytes_to_hex(block->hash, SHA256_DIGEST_LENGTH));

			num_transactions= llist_size(block->transactions);
			block_size = sizeof(block_t);
			transactions_size = num_transactions * sizeof(transaction_t);
			total_size = block_size + transactions_size;
			printf("Block Size: " C_GREEN "%lu" C_RESET " bytes\n", total_size);
			printf("Number of Transactions: " C_GREEN "%u" C_RESET "\n", num_transactions);

			llist_for_each(block->transactions, print_transaction_info, arg_tx);



		}
	}
	printf(C_GREEN "\n======================================================\n\n" C_RESET);
	return (0);
}

/**
* print_transaction_info - print transaction information
* @node: transaction node
* @idx: index
* @args: argument
* Return: 0
*/
int print_transaction_info(llist_node_t node, unsigned int idx, void *args)
{
	void **ptr = args;
	block_t *block = (block_t *)ptr[0];
	blockchain_t *blockchain = (blockchain_t *)ptr[1];
	transaction_t *transaction = (transaction_t *)node;
	void *arg_out[3];


	arg_out[0] = (void *)block;
	arg_out[1] = (void *)transaction;
	arg_out[2] = (void *)blockchain->unspent;

	printf("Transaction ID: ");
	printf(C_CYAN "%s\n" C_RESET, bytes_to_hex(transaction->id, SHA256_DIGEST_LENGTH));

	printf("  Inputs:\n");
	llist_for_each(transaction->inputs, print_input_info, NULL);

	printf("  Outputs:\n");
	llist_for_each(transaction->outputs, print_output_info, arg_out);

	(void)idx;
	return (0);
}

/**
* print_input_info - print input information
* @node: input node
* @idx: index
* @args: argument
* Return: 0
*/
int print_input_info(llist_node_t node, unsigned int idx, void *args)
{
	tx_in_t *input;
	(void)idx;
	(void)args;


	input = (tx_in_t *)llist_get_node_data(node);
	printf(C_RED"    - Block Hash: %s\n", bytes_to_hex(input->block_hash, SHA256_DIGEST_LENGTH));
	printf("    - Transaction ID: %s\n", bytes_to_hex(input->tx_id, SHA256_DIGEST_LENGTH));
	printf("    - Transaction Output Hash: %s\n\n" C_RESET, bytes_to_hex(input->tx_out_hash, SHA256_DIGEST_LENGTH));
	return (0);
}

/**
* print_output_info - print output information
* @node: output node
* @idx: index
* @args: argument
* Return: 0
*/
int print_output_info(llist_node_t node, unsigned int idx, void *args)
{

	void **ptr = args;
	tx_out_t *output;


	block_t *block = (block_t *)ptr[0];
	transaction_t *tx = (transaction_t *)ptr[1];
	llist_t *unspents = (llist_t *)ptr[2];

	output = (tx_out_t *)llist_get_node_data(node);
	if (is_output_unspent(output->hash, block, tx, unspents))
	{
		printf(C_GREEN "    - Amount: %u\n", output->amount);
		printf("    - Public Address: %s\n", bytes_to_hex(output->pub, EC_PUB_LEN));
		printf("    - Output Hash: %s\n", bytes_to_hex(output->hash, SHA256_DIGEST_LENGTH));
		printf("    - Status: UTXO (Unspent)\n\n" C_RESET);
	} else {
		printf("    - Amount: %u\n", output->amount);
		printf("    - Public Address: %s\n", bytes_to_hex(output->pub, EC_PUB_LEN));
		printf("    - Output Hash: %s\n", bytes_to_hex(output->hash, SHA256_DIGEST_LENGTH));
		printf("    - Status: Spent\n\n");
	}

	(void)idx;
	return (0);
}

int is_output_unspent(uint8_t output_hash[SHA256_DIGEST_LENGTH], block_t *block, transaction_t *tx, llist_t *unspents)
{
	unsigned int i;
	unsigned int num_spent_outputs = llist_size(unspents);

	if (!unspents || !block || !tx || !output_hash)
	{
		printf(C_RED "missing argument to print right status\n" C_RESET);
		return (0);
	}

	for (i = 0; i < (unsigned int)num_spent_outputs; i++)
	{
		unspent_tx_out_t *unspent = (unspent_tx_out_t *) llist_get_node_at(unspents, i);

	if (!memcmp(block->hash, unspent->block_hash,
												sizeof(SHA256_DIGEST_LENGTH))
			&& !memcmp(tx->id, unspent->tx_id, sizeof(SHA256_DIGEST_LENGTH))
			&& !memcmp(output_hash, unspent->out.hash,
													sizeof(SHA256_DIGEST_LENGTH)))
			return (1);
	}
	return (0);
}
