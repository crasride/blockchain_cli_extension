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
	size_t list_size = llist_size(blockchain->unspent);
	void *arg_json[2];


	arg_json[1] = (void *)list_size;

	if(list_size == 0)
	{
		printf("Error: No unspent transactions available\n");
		return (-1);
	}

	if (!blockchain || !blockchain->unspent)
	{
		printf("Error: Blockchain or unspent transactions list is invalid\n");
		return (-1);
	}

	if (llist_size(blockchain->unspent) == 0)
	{
		printf("Error: No unspent transactions available\n");
		return (-1);
	}
	file = fopen("data_utxo.json", "w");


	if (!file)
	{
		printf("Error: Failed to open file for writing UTXO data\n");
		return (-1);
	}

	arg_json[0] = (void *)file;

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
	size_t size = (size_t)ptr[1];

	if (!node)
	{
		printf("Node is NULL\n");
		return (-1);
	}

	utxo = (unspent_tx_out_t *)llist_get_node_data(node);


	fprintf(file, "{\n");
	fprintf(file, "  \"block_hash\": \"%s\",\n", bytes_to_hex(utxo->block_hash,
			SHA256_DIGEST_LENGTH));
	fprintf(file, "  \"transaction_id\": \"%s\",\n", bytes_to_hex(utxo->tx_id,
			SHA256_DIGEST_LENGTH));
	fprintf(file, "  \"output\": {\n");
	fprintf(file, "    \"amount\": %u,\n", utxo->out.amount);
	fprintf(file, "    \"public_address\": \"%s\",\n", bytes_to_hex(utxo->out.pub,
			EC_PUB_LEN));
	fprintf(file, "    \"output_hash\": \"%s\"\n", bytes_to_hex(utxo->out.hash,
			SHA256_DIGEST_LENGTH));
	fprintf(file, "  }\n");

	if (idx == size - 1)

	{
		fprintf(file, "}\n");
	}
	else
	{
		fprintf(file, "},\n");
	}

	printf("Block Hash: %s\n", bytes_to_hex(utxo->block_hash,
														SHA256_DIGEST_LENGTH));
	printf("Transaction ID: %s\n", bytes_to_hex(utxo->tx_id,
														SHA256_DIGEST_LENGTH));
	printf("Output Info:\n");
	printf("    - Amount: %u\n", utxo->out.amount);
	printf("    - Public Address: %s\n", bytes_to_hex(utxo->out.pub, EC_PUB_LEN));
	printf("    - Output Hash: %s\n", bytes_to_hex(utxo->out.hash,
														SHA256_DIGEST_LENGTH));
	printf(C_GREEN"\n========================================================\n\n"
	C_RESET);

	return (0);
}
