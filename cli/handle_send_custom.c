#include "cli.h"

/**
* handle_send_custom - handle the send command
* @amount: amount to send
* @receiver_address: receiver address
* @state: pointer to store EC key
* Return: 0 if success, -1 if failed
*/
int handle_send_custom(int amount, char *receiver_address, state_t *state)
{
	EC_KEY *receiver_key = NULL;
	transaction_t *tx = NULL;
	llist_t *selected_utxos = NULL, *new_tx_out = NULL;
	uint32_t size = 0, sum_selected = 0, i;
	/* char *dummy_line; */

	str_to_key(receiver_address, &receiver_key);
	system("clear");
	while(1)
	{
		selected_utxos = utxo_list_selection(state);
		if (!selected_utxos)
		{
			system("clear");
			return (0);
		}
		if (llist_size(selected_utxos) == 0)
		{
			system("clear");
			printf("\nNo UTXOs selected----\n");
		}
		else
			break;
	}
	size = llist_size(selected_utxos);
	for (i = 0; i < size; i++)
	{
		unspent_tx_out_t *utxo = NULL;

		utxo = llist_get_node_at(selected_utxos, i);
		sum_selected += utxo->out.amount;
	}

	readline("");
	system("clear");

	new_tx_out = tx_out_custom_list(state, receiver_key, amount, sum_selected);
	if (selected_utxos && new_tx_out)
	{
		tx = transaction_create(state->wallet, receiver_key, amount,
								selected_utxos, state->tx_pool, new_tx_out);
	}
	else
	{
		printf("List of UTXOs not created\n");
		EC_KEY_free(receiver_key);
		return (-1);
	}
	llist_destroy(selected_utxos, 0, NULL);
	if (tx)
		printf("Transaction created successfully\n");
	else
	{
		printf("Failed to create transaction\n");
		EC_KEY_free(receiver_key);
		return (-1);
	}
	if ((transaction_is_valid(tx, state->blockchain->unspent)) == 1)
		printf("Transaction is valid\n");
	else
	{
		printf("Invalid transaction\n");
		transaction_destroy(tx);
		EC_KEY_free(receiver_key);
		return (-1);
	}
	if (llist_add_node(state->tx_pool, tx, ADD_NODE_REAR) == -1)
	{
		printf("Failed to add transaction to pool\n");
		transaction_destroy(tx);
		EC_KEY_free(receiver_key);
		return (-1);
	}
	printf("Transaction sent successfully\n");
	EC_KEY_free(receiver_key);

	return (0);
}
