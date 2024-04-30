#include "cli.h"

int select_unspent(llist_node_t node, unsigned int idx, void *args)
{
	unspent_tx_out_t *unspent = node;/* Unspent transaction output */
	void **ptr = args;/* Array of arguments */
	llist_t *current_own_unspent = ptr[1];
	llist_t *tx_pool = ptr[2];/* List of used unspent transactions */

	(void)idx; /* Avoid the warning "unused parameter" */
	/* Check public key of the unspent trans matches the sender's public key*/
	if (!memcmp(unspent->out.pub, ptr[0],
								EC_PUB_LEN) && !used_in_pool(tx_pool, unspent))
	{
		/* Add the transaction input to the list */
		llist_add_node(current_own_unspent, node, ADD_NODE_REAR);
	}
	return (0);
}

/**
* generate_own_sorted_unspent_list - generate short unspent list
* @state: CLI state
* Return: list of own unspent tx_out
*/
llist_t *generate_own_sorted_unspent_list(state_t *state)
{
	uint8_t pub[EC_PUB_LEN];
	llist_t *tx_pool = state->tx_pool;
	EC_KEY *sender = state->wallet;
	blockchain_t *blockchain = state->blockchain;
	llist_t *current_own_unspent = llist_create(MT_SUPPORT_FALSE);
	void *args[4];

	ec_to_pub(sender, pub);
	args[0] = pub, args[1] = current_own_unspent, args[2] = tx_pool;

	if (!blockchain || !blockchain->unspent)
	{
		printf("Error: Blockchain or unspent transactions list is invalid\n");
		return (NULL);
	}

	llist_for_each(blockchain->unspent, (node_func_t)select_unspent, args);
	printf("\n");
	return (current_own_unspent);
}

int checkline(char *line)
{
	int i = 0;

	while (line[i])
	{
		if (!isdigit(line[i]) && line[i] != ' ')
		{
			printf("only digit and spaces are allowed\n");
			return (1);
		}
		i++;
	}
	return (0);
}

char **get_line_tokens(void)
{
	char *line = NULL, *token = NULL;
	char **tokens = (char **)malloc(MAX_TOKENS * sizeof(char *));
	int i;

	if (tokens == NULL)
	{
		printf("Memory allocation failed.\n");
		return (NULL);
	}
	while (1)
	{
		line = readline("\nSelection: ");
		if (!line)
		{
			printf("realine failed.\n");
			free(tokens);
			return (NULL);
		}

		if (checkline(line) == 1)
			continue;
		else
			break;
	}

	token = strtok(line, " ");
	for (i = 0; token != NULL && i < MAX_TOKENS; i++)
	{
		/* while (*token && !isdigit(*token))
			token++; */
		tokens[i] = strdup(token);
		token = strtok(NULL, " ");
	}
	if (i >= MAX_TOKENS)
	{
		printf("Maximum number of tokens exceeded.\n");
		free(line);
		return (NULL);
	}
	free(line);

	return tokens;
}

int len(char **amounts)
{
	int i = 0;

	while (amounts[i])
		i++;
	return (i);
}

llist_t *utxo_list_selection(state_t *state)
{
	llist_t *selected_utxos = llist_create(MT_SUPPORT_FALSE);
	llist_t *current_own_unspent = generate_own_sorted_unspent_list(state);
	int size = llist_size(current_own_unspent), i, sum_selected = 0, size_selected;
	int index = 0;
	bool *checkboxes = (bool *)calloc(size, sizeof(bool)); /* Array of checkboxes */
	char **tokens_selected;

	printf("Number of utxos: %d\n", size);
	/* Display initial checkboxes */
	for (i = 0; i < size; i++)
	{
		unspent_tx_out_t *utxo = NULL;

		utxo = llist_get_node_at(current_own_unspent, i);
		printf("> UTXO %d: %u HolbertonCoin\n", i, utxo->out.amount);
	}

	tokens_selected = get_line_tokens();
	size_selected = len(tokens_selected);
	for (i = 0; i < size_selected; i++)
	{
		index = atoi(tokens_selected[i]);
		checkboxes[index] = true;
	}

	for (i = 0; i < size; i++)
	{
		unspent_tx_out_t *utxo = NULL;

		if (checkboxes[i])
		{
			utxo = llist_get_node_at(current_own_unspent, i);
			sum_selected += (int)utxo->out.amount;
			printf("Selected: UTXO index %d (%u HolbertonCoins)\n", i, utxo->out.amount);
			llist_add_node(selected_utxos, utxo, ADD_NODE_REAR);
		}
	}
	if (sum_selected != 0)
		printf("\nTotal selected for transaction: %d HolbertonCoins\n", sum_selected);

	llist_destroy(current_own_unspent, 0, NULL);
	free(checkboxes); /* Free memory allocated for checkboxes */

	return (selected_utxos);
}

llist_t *tx_out_custom_list(state_t *state, EC_KEY *receiver_key, int amount,
							int sum_selected)
{
	char **tokens_receiver, **tokens_sender;
	llist_t *tx_out_list = llist_create(MT_SUPPORT_FALSE);
	int i, change = 0, len_receiver = 0, len_sender = 0;
	uint32_t token = 0;
	uint8_t pub_receiver[EC_PUB_LEN] = {0};
	uint8_t pub_sender[EC_PUB_LEN] = {0};

	ec_to_pub(state->wallet, pub_sender);
	ec_to_pub(receiver_key, pub_receiver);

	while (1)
	{
		printf("\nEnter amount for each output you want to send, on the same line, separate by a space:\n");
		printf("Send amount: %d\n", amount);

		tokens_receiver = get_line_tokens();
		if (!tokens_receiver)
		{
			system("clear");
			printf("No receiver outputs chosen\n");
		}
		else
			break;
	}

	len_receiver = len(tokens_receiver);
	printf("Receiver outputs will be: ");
	for (i = 0; i < len_receiver; i++)
	{
		tx_out_t *tx_out = NULL;

		token = atoi(tokens_receiver[i]);
		printf("%u ", token);
		tx_out = tx_out_create(token, pub_receiver);
		llist_add_node(tx_out_list, tx_out, ADD_NODE_REAR);
	}

	readline("");
	system("clear");

	change = sum_selected - amount; /* Calculate change */
	printf("\nEnter each amount wanted in change on the same line, separate by a space:\n");
	printf("Change amount: %d\n", change);

	tokens_sender = get_line_tokens();
	if (!tokens_sender)
	{
		printf("No sender choice\n");
		return (NULL);
	}

	len_sender = len(tokens_sender);
	printf("Outputs back to sender will be: ");
	for (i = 0; i < len_sender; i++)
	{
		tx_out_t *tx_out = NULL;

		token = atoi(tokens_sender[i]);
		printf("%u ", token);
		tx_out = tx_out_create(token, pub_sender);
		llist_add_node(tx_out_list, tx_out, ADD_NODE_REAR);
	}

	readline("");
	system("clear");

	return (tx_out_list);
}
