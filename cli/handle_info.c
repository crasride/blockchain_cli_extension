#include "cli.h"


/**
* bytes_to_hex - converts an array of bytes to a hex string
* @bytes: array of bytes
* @len: number of bytes
* Return: pointer to the resulting hex string
*/
char *bytes_to_hex(const uint8_t *bytes, size_t len)
{
	size_t i;
	static const char hex_chars[] = "0123456789abcdef";
	char *hex_str = malloc(len * 2 + 1);

	if (hex_str)
	{
		for (i = 0; i < len; i++)
		{
			hex_str[i * 2] = hex_chars[(bytes[i] >> 4) & 0xF];
			hex_str[i * 2 + 1] = hex_chars[bytes[i] & 0xF];
		}
		hex_str[len * 2] = '\0';
	}
	return (hex_str);
}

/**
* handle_info - handle the info command
* @state: CLI state
* Return: 0 if success, -1 if failed
*/

int handle_info(state_t *state)
{
	blockchain_t *blockchain = state->blockchain;
	llist_t *unspent = blockchain->unspent;
	llist_t *tx_pool = state->tx_pool;
	uint32_t coins = 0, own_coins = 0;
	FILE *file	= NULL;
	uint8_t pub[EC_PUB_LEN] = {0};
	void *args[2];

	if (state->wallet)
		ec_to_pub(state->wallet, pub);
	if (!state || !blockchain)
	{
		printf("Error: State or Blockchain is NULL\n");
		return (-1);
	}

	if (!state->blockchain)
	{
		printf("Error: Blockchain is NULL\n");
		return (-1);
	}

	llist_for_each(state->blockchain->unspent, sum_unspent, &coins);
	args[0] = (void *)&own_coins;
	args[1] = (void *)state;
	llist_for_each(state->blockchain->unspent, sum_unspent_wallet, args);

	file = fopen("data_blockchain.json", "w");
	if (file)
	{
		fprintf(file, "{\n");
		fprintf(file, "  \"number_of_blocks\": %u,\n", llist_size(blockchain->chain));
		fprintf(file, "  \"number_of_unspent_outputs\": %u,\n", llist_size(unspent));

		if (tx_pool)
		{
			fprintf(file, "  \"transaction_pool\": {\n");
			fprintf(file, "    \"available\": true,\n");
			fprintf(file, "    \"pending_transactions\": %u\n", llist_size(tx_pool));
			fprintf(file, "  },\n");
		}
		else
		{
			fprintf(file, "  \"transaction_pool\": {\n");
			fprintf(file, "    \"available\": false\n");
			fprintf(file, "  },\n");
		}

		fprintf(file, "  \"Total Supply HolbertonCoins\": \"%u\",\n", coins);
		fprintf(file, "  \"Total coin on wallet\": \"%u\",\n", own_coins);
		fprintf(file, "  \"wallet_loaded\": \"%s\"\n", bytes_to_hex(pub, EC_PUB_LEN));
		fprintf(file, "}\n");

		fclose(file);
	}
	else
	{
		printf("Error: Failed to open file for writing blockchain data\n");
		return (-1);
	}

	printf(C_GREEN "\n======================================================\n");
	printf("   ========== Blockchain Information =============   \n");
	printf("======================================================\n\n"C_RESET);

	/* llist_for_each(state->blockchain->unspent, sum_unspent, &coins); */

	printf("Number of Blocks in Blockchain: " C_GREEN "%u" C_RESET "\n", llist_size(blockchain->chain));
	printf("Number of unspent transaction outputs: " C_GREEN "%u" C_RESET "\n", llist_size(unspent));

	if (tx_pool)
	{
		printf("Transaction pool is " C_GREEN "available" C_RESET "\n");
		printf("Number of pending transactions in transaction pool: " C_GREEN "%u" C_RESET "\n",
				llist_size(tx_pool));
	}
	else
	{
		printf("Error: Transaction pool is " C_RED "NULL\n" C_RESET "\n");
	}
	printf("Total Supply HolbertonCoins: " C_GREEN "%u" C_RESET "\n", coins);
	printf("Total coin on wallet: " C_GREEN "%u" C_RESET "\n", own_coins);
	printf("Wallet loaded: %s\n", bytes_to_hex(pub, EC_PUB_LEN));
	printf(C_GREEN "\n======================================================\n\n" C_RESET);

	return (0);
}
