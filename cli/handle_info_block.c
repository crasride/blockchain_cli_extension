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
	void *arg_tx[3];
	FILE *file = NULL;
	char *str_prev_hash = NULL, *str_block_hash;

	arg_tx[1] = (void *)state;

	if (!chain)
	{
		printf(C_RED "No blocks in the blockchain\n" C_RESET);
		return (-1);
	}
	file = fopen("data_block.json", "w");
	if (!file)
	{
		printf("Error: Failed to open file for writing block data\n");
		return (-1);
	}

	num_blocks = llist_size(chain);
	fprintf(file, "[\n");
	fprintf(file, "  {\n");
	for (i = 0; i < num_blocks; i++)
	{
		block = (block_t *)llist_get_node_at(chain, i);
		if (block != NULL)
		{
			arg_tx[0] = (void *)block;
			str_prev_hash = bytes_to_hex(block->info.prev_hash, SHA256_DIGEST_LENGTH);
			str_block_hash = bytes_to_hex(block->hash, SHA256_DIGEST_LENGTH);

			fprintf(file, "    \"Block %u\": [\n", block->info.index);
			fprintf(file, "      {\n");
			fprintf(file, "        \"index\": %u,\n", block->info.index);
			fprintf(file, "        \"difficulty\": %u,\n", block->info.difficulty);
			fprintf(file, "        \"timestamp\": %lu,\n", block->info.timestamp);
			fprintf(file, "        \"nonce\": %lu,\n", block->info.nonce);
			fprintf(file, "        \"previous_block_hash\": \"%s\",\n", str_prev_hash);
			fprintf(file, "        \"block_hash\": \"%s\",\n", str_block_hash);

			printf("\n");
			printf(C_GREEN"\n======================================================\n");
			printf("   ============ Infos for block %u ===============   \n", block->info.index);
			printf("======================================================\n\n"C_RESET);
			printf("\n");
			printf("Block Index: " C_GREEN "%u" C_RESET "\n", block->info.index);
			printf("Difficulty: " C_GREEN "%u" C_RESET "\n", block->info.difficulty);
			printf("Timestamp: " C_GREEN "%lu" C_RESET "\n", block->info.timestamp);
			printf("Nonce: " C_GREEN "%lu" C_RESET "\n", block->info.nonce);
			printf("Previous Block Hash: "C_GREEN "%s" C_RESET "\n", str_prev_hash);
			printf("Block Hash: " C_GREEN "%s" C_RESET "\n", str_block_hash);

			num_transactions = llist_size(block->transactions);
			block_size = sizeof(block_t);
			transactions_size = num_transactions * sizeof(transaction_t);
			total_size = block_size + transactions_size;
			fprintf(file, "        \"block_size\": %lu,\n", total_size);
			fprintf(file, "        \"num_transactions\": %u,\n", num_transactions);
			fprintf(file, "        \"transactions\": [\n");

			printf("Block Size: " C_GREEN "%lu" C_RESET " bytes\n", total_size);
			printf("Number of Transactions: " C_GREEN "%u" C_RESET "\n", num_transactions);


			arg_tx[2] = (void *)file;
			llist_for_each(block->transactions, print_transaction_info, arg_tx);
			fprintf(file, "      ]\n");

			fprintf(file, "    }\n");
			if (i < num_blocks - 1)
				fprintf(file, "    ],\n");
			else
				fprintf(file, "  ]\n");
			if (str_prev_hash)
				free(str_prev_hash);
			if (str_block_hash)
				free(str_block_hash);
		}
	}
	fprintf(file, "  }\n");
	fprintf(file, "]\n");
	fclose(file);

	return 0;
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
	state_t *state = (state_t *)ptr[1];
	FILE *file = (FILE *)ptr[2];
	transaction_t *transaction = (transaction_t *)node;
	void *arg_in_out[4];
	unsigned int num_transactions = llist_size(block->transactions);
	char *str_tx_id = NULL;

	arg_in_out[0] = (void *)block;
	arg_in_out[1] = (void *)transaction;
	arg_in_out[2] = (void *)state;
	arg_in_out[3] = (void *)file;

	str_tx_id = bytes_to_hex(transaction->id, SHA256_DIGEST_LENGTH);
	fprintf(file, "        {\n");
	fprintf(file, "          \"transaction_id\": \"%s\",\n", str_tx_id);
	printf("Transaction ID: ");
	printf(C_CYAN "%s\n" C_RESET, str_tx_id);


	fprintf(file, "          \"inputs\": [\n");
	printf("  Inputs:\n");
	llist_for_each(transaction->inputs, print_input_info, arg_in_out);
	fprintf(file, "          ],\n");

	fprintf(file, "          \"outputs\": [\n");
	printf("  Outputs:\n");
	llist_for_each(transaction->outputs, print_output_info, arg_in_out);
	fprintf(file, "          ]\n");

	if (idx < num_transactions - 1)
		fprintf(file, "        },\n");
	else
		fprintf(file, "        }\n");

	if (str_tx_id)
		free(str_tx_id);

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
	void **ptr = args;
	tx_in_t *input;
	transaction_t *tx = (transaction_t *)ptr[1];
	FILE *file = (FILE *)ptr[3];
	char *str_block_hash = NULL, *str_tx_id = NULL, *str_tx_out_hash = NULL;

	input = (tx_in_t *)llist_get_node_data(node);

	str_block_hash = bytes_to_hex(input->block_hash, SHA256_DIGEST_LENGTH);
	str_tx_id = bytes_to_hex(input->tx_id, SHA256_DIGEST_LENGTH);
	str_tx_out_hash = bytes_to_hex(input->tx_out_hash, SHA256_DIGEST_LENGTH);
	fprintf(file, "            {\n");
	fprintf(file, "              \"block_hash\": \"%s\",\n", str_block_hash);
	printf(C_RED"    - Block Hash: %s\n", str_block_hash);
	fprintf(file, "              \"transaction_id\": \"%s\",\n", str_tx_id);
	printf("    - Transaction ID: %s\n", str_tx_id);
	fprintf(file, "              \"transaction_output_hash\": \"%s\"\n", str_tx_out_hash);
	printf("    - Transaction Output Hash: %s\n\n" C_RESET, str_tx_out_hash);

	if (idx < ((unsigned int)llist_size(tx->inputs) - 1))
		fprintf(file, "            },\n");
	else
		fprintf(file, "            }\n");
	if (str_block_hash)
		free(str_block_hash);
	if (str_tx_id)
		free(str_tx_id);
	if (str_tx_out_hash)
		free(str_tx_out_hash);

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
	state_t *state = (state_t *)ptr[2];
	FILE *file = (FILE *)ptr[3];
	unsigned int num_out;
	llist_t *unspents = state->blockchain->unspent;
	uint8_t pub_wallet[EC_PUB_LEN] = {0};
	char *str_pub_wallet = NULL, *str_pub = NULL, *str_output_hash = NULL;

	if (state->wallet)
		ec_to_pub(state->wallet, pub_wallet);
	output = (tx_out_t *)llist_get_node_data(node);

	str_pub_wallet = bytes_to_hex(pub_wallet, EC_PUB_LEN);
	str_pub = bytes_to_hex(output->pub, EC_PUB_LEN);
	str_output_hash = bytes_to_hex(output->hash, SHA256_DIGEST_LENGTH);
	num_out = llist_size(tx->outputs);

	if (is_output_unspent(output->hash, block, tx, unspents))
	{
		fprintf(file, "            {\n");
		fprintf(file, "              \"amount\": %u,\n", output->amount);
		printf("    - Amount: %u\n", output->amount);
		fprintf(file, "              \"public_address\": \"%s\",\n", str_pub);
		if (strcmp(str_pub, str_pub_wallet) == 0)
			printf("    - Public Address:"C_PURPLE" %s\n" C_RESET, str_pub);
		else
			printf("    - Public Address: %s\n", str_pub);
		fprintf(file, "              \"output_hash\": \"%s\",\n", str_output_hash);
		printf("    - Output Hash: %s\n", str_output_hash);
		fprintf(file, "              \"status\": \"UTXO (Unspent)\"\n");
		printf("    - Status: "C_GREEN"Unspent\n\n" C_RESET);

	if (idx < num_out - 1)
		fprintf(file, "            },\n");
	else
		fprintf(file, "            }\n");
	} else {
		fprintf(file, "            {\n");
		fprintf(file, "              \"amount\": %u,\n", output->amount);
		printf("    - Amount: %u\n", output->amount);
		fprintf(file, "              \"public_address\": \"%s\",\n", str_pub);
		printf("    - Public Address: %s\n", str_pub);
		fprintf(file, "              \"output_hash\": \"%s\",\n", str_output_hash);
		printf("    - Output Hash: %s\n", str_output_hash);
		fprintf(file, "              \"status\": \"Spent\"\n");
		printf("    - Status: "C_RED"Spent\n\n" C_RESET);
	if (idx < num_out - 1)
		fprintf(file, "            },\n");
	else
		fprintf(file, "            }\n");
	}
	if (str_pub_wallet)
		free(str_pub_wallet);
	if (str_pub)
		free(str_pub);
	if (str_output_hash)
		free(str_output_hash);

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
