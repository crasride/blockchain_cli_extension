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
	llist_t *selected_utxos = NULL;

	selected_utxos = utxo_list_selection(state);
	str_to_key(receiver_address, &receiver_key);
	if (selected_utxos)
	{
		tx = transaction_create_custom(state->wallet, receiver_key, amount,
							state->blockchain->unspent, state->tx_pool,
							selected_utxos);
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
