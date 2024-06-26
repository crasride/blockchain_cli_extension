#include "cli.h"


/**
* generate_sorted_unspent_list - generate sorted unspent transactions list
* @blockchain: blockchain
* Return: 0 if success, -1 if failed
*/
int generate_sorted_unspent_list(blockchain_t *blockchain)
{
	int sort_result = 0;
	FILE *file = NULL;
	size_t list_size = 0;
	void *arg_json[2];

	if (!blockchain)
	{
		printf("Error: Blockchain is invalid\n");
		return (-1);
	}
	if (!blockchain->unspent)
	{
		printf("Error sorted: unspent transactions list is invalid\n");
		return (-1);
	}
	file = fopen("data_utxo.json", "w");
	if (!file)
	{
		printf("Error: Failed to open file for writing UTXO data\n");
		return (-1);
	}

	arg_json[0] = (void *)file;
	list_size = llist_size(blockchain->unspent);
	arg_json[1] = (void *)&list_size;

	sort_result = llist_sort(blockchain->unspent,
								(node_cmp_t)compares_unspent_tx_out, NULL, 0);
	if (sort_result != 0)
	{
		printf("Error: Failed to sort unspent transactions list\n");
		fclose(file);
		return (-1);
	}

	printf(C_GREEN"\n========================================================\n");
	printf("    ================= List Utxo ==================    ");
	printf("\n========================================================\n\n"C_RESET);
	fprintf(file, "[\n");
	llist_for_each(blockchain->unspent, (node_func_t)print_unspent_tx_out_info, arg_json);
	fprintf(file, "]\n");
	if(list_size == 0)
	{
		printf("No unspent transactions available\n");
		return (-1);
	}

	fclose(file);

	return (0);
}

/**
* generate_unspent_list - generate sorted unspent transactions list
* @blockchain: blockchain
* Return: 0 if success, -1 if failed
*/
int generate_unspent_list(blockchain_t *blockchain)
{
	FILE *file = NULL;
	size_t list_size = 0;
	void *arg_json[2];

	if (!blockchain)
	{
		printf("Error: Blockchain is invalid\n");
		return (-1);
	}
	if (!blockchain->unspent)
	{
		printf("Error unsorted: unspent transactions list is invalid\n");
		return (-1);
	}
	file = fopen("data_utxo.json", "w");
	if (!file)
	{
		printf("Error: Failed to open file for writing UTXO data\n");
		return (-1);
	}

	arg_json[0] = (void *)file;
	list_size = llist_size(blockchain->unspent);
	arg_json[1] = (void *)&list_size;

	printf(C_GREEN"\n========================================================\n");
	printf("    ================= List Utxo ==================    ");
	printf("\n========================================================\n\n"C_RESET);
	fprintf(file, "[\n");
	llist_for_each(blockchain->unspent, (node_func_t)print_unspent_tx_out_info, arg_json);
	fprintf(file, "]\n");
	if(list_size == 0)
	{
		printf("No unspent transactions available\n");
		return (-1);
	}

	fclose(file);

	return (0);
}

/**
* compares_unspent_tx_out - compare unspent transaction outputs
* @first: first unspent transaction output
* @second: second unspent transaction output
* @arg: argument
* Return: -1 if first < second, 1 if first > second, 0 if equal
*/
int compares_unspent_tx_out(unspent_tx_out_t *first, unspent_tx_out_t *second,
																		void *arg)
{
	(void)arg;
	if (first->out.amount < second->out.amount)
	{
		return (-1);
	}
	else if (first->out.amount > second->out.amount)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

/**
* print_unspent_tx_out_info - print unspent transaction output information
* @node: unspent transaction output node
* @idx: index of the node
* @arg: argument
* Return: 0 if success, all other values if failure
*/
int print_unspent_tx_out_info(llist_node_t node, unsigned int idx, void *arg)
{
	void **ptr = arg;
	unspent_tx_out_t *utxo = NULL;
	FILE *file = (FILE *)ptr[0];
	size_t *size = (size_t *)ptr[1];
	char *str_block_hash = NULL, *str_tx_id = NULL, *str_pub = NULL;
	char *str_out_hash = NULL;

	if (!node)
	{
		printf("Node is NULL\n");
		return (-1);
	}

	utxo = (unspent_tx_out_t *)llist_get_node_data(node);

	str_block_hash = bytes_to_hex(utxo->block_hash,SHA256_DIGEST_LENGTH);
	str_tx_id = bytes_to_hex(utxo->tx_id, SHA256_DIGEST_LENGTH);
	str_pub = bytes_to_hex(utxo->out.pub, EC_PUB_LEN);
	str_out_hash = bytes_to_hex(utxo->out.hash, SHA256_DIGEST_LENGTH);

	fprintf(file, "{\n");
	fprintf(file, "  \"block_hash\": \"%s\",\n", str_block_hash);
	fprintf(file, "  \"transaction_id\": \"%s\",\n", str_tx_id);
	fprintf(file, "  \"output\": {\n");
	fprintf(file, "    \"amount\": %u,\n", utxo->out.amount);
	fprintf(file, "    \"public_address\": \"%s\",\n", str_pub);
	fprintf(file, "    \"output_hash\": \"%s\"\n", str_out_hash);
	fprintf(file, "  }\n");

	if (idx == *size - 1)
	{
		fprintf(file, "}\n");
	}
	else
	{
		fprintf(file, "},\n");
	}

	printf("Block Hash: %s\n", str_block_hash);
	printf("Transaction ID: %s\n", str_tx_id);
	printf("Output Info:\n");
	printf("    - Amount: %u\n", utxo->out.amount);
	printf("    - Public Address: %s\n", str_pub);
	printf("    - Output Hash: %s\n", str_out_hash);
	printf(C_GREEN"\n========================================================\n\n"
	C_RESET);
	if (str_block_hash)
		free(str_block_hash);
	if (str_tx_id)
		free(str_tx_id);
	if (str_pub)
		free(str_pub);
	if (str_out_hash)
		free(str_out_hash);

	return (0);
}
