#include "cli.h"


/**
* generate_sorted_unspent_list - generate sorted unspent transactions list
* @blockchain: blockchain
* Return: 0 if success, -1 if failed
*/
int generate_sorted_unspent_list(blockchain_t *blockchain)
{

	int sort_result = 0;

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

	sort_result = llist_sort(blockchain->unspent,
									(node_cmp_t)compares_unspent_tx_out, NULL, 0);
	if (sort_result != 0)
	{
		printf("Error: Failed to sort unspent transactions list\n");
		return (-1);
	}

	llist_for_each(blockchain->unspent, (node_func_t)print_unspent_tx_out_info,
					NULL);

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
	unspent_tx_out_t *utxo = NULL;
	(void)idx;
	(void)arg;

	printf(C_GREEN"\n========================================================\n");
	printf("    ================= List Utxo ==================    ");
	printf("\n========================================================\n\n"
	C_RESET);

	utxo = (unspent_tx_out_t *)llist_get_node_data(node);
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
