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

void display_checkboxes(bool checkboxes[], int size, int currentCheckbox,
							llist_t *current_own_unspent)
{
	int i;
	unspent_tx_out_t *utxo = NULL;

	clear(); /* Clear screen */
	printw("Use arrow keys to navigate and 'Space' to select/deselect UTXOs:\n");
	for (i = 0; i < size; i++)
	{
		utxo = llist_get_node_at(current_own_unspent, i);

		(void)utxo;
		if (i == currentCheckbox)
		{
			printw("> [%c] utxo %d: %d HolbertonCoin %s\n", checkboxes[i] ? 'X' : ' ', i, utxo->out.amount, checkboxes[i] ? "Selected" : "Not Selected");
		}
		else
		{
			printw("  [%c] utxo %d: %d HolbertonCoin %s\n", checkboxes[i] ? 'X' : ' ', i, utxo->out.amount, checkboxes[i] ? "Selected" : "Not Selected");
		}
		utxo = (unspent_tx_out_t *)llist_get_node_at(current_own_unspent, i);
	}
	refresh(); /* Refresh screen */
}

llist_t *utxo_list_selection(state_t *state)
{
	llist_t *current_own_unspent = generate_own_sorted_unspent_list(state);
	int size = llist_size(current_own_unspent), i;
	int key;
	bool *checkboxes = (bool *)calloc(size, sizeof(bool)); /* Array of checkboxes */
	int currentCheckbox = 0; /* Index of the currently highlighted checkbox */
	llist_t *selected_utxos = llist_create(MT_SUPPORT_FALSE);

	/* Initialize curses */
	initscr();
	cbreak(); /* Line buffering disabled */
	noecho(); /* Don't echo input */
	keypad(stdscr, TRUE); /* Enable keyboard mapping */

	printw("Number of utxos: %d\n", llist_size(current_own_unspent));
	/* Display initial checkboxes */
	display_checkboxes(checkboxes, size, currentCheckbox, current_own_unspent);

	/* Allow user to navigate and select checkboxes until 'Enter' key is pressed */
	while (1)
	{
		key = getch(); /* Get user input */

		/* Handle arrow key inputs */
		switch (key)
		{
			case KEY_UP:
				currentCheckbox = (currentCheckbox - 1 + size) % size;
				break;
			case KEY_DOWN:
				currentCheckbox = (currentCheckbox + 1) % size;
				break;
			case ' ':
				checkboxes[currentCheckbox] = !checkboxes[currentCheckbox];
				break;
			case '\n': /* Enter key (exit loop) */
				goto end;
			default:
				break;
		}
		display_checkboxes(checkboxes, size, currentCheckbox, current_own_unspent);
	}

	end:
	for (i = 0; i < size; i++)
	{
		unspent_tx_out_t *utxo = NULL;

		if (checkboxes[i])
		{
			utxo = llist_get_node_at(current_own_unspent, i);
			printw("Selected index %d: %u\n", i, utxo->out.amount);
			llist_add_node(selected_utxos, utxo, ADD_NODE_REAR);
		}
	}
	if (llist_size(selected_utxos) == 0)
		printw("No UTXOs selected\n");
	llist_destroy(current_own_unspent, 0, NULL);
	free(checkboxes); /* Free memory allocated for checkboxes */
	refresh(); /* Refresh screen */
	getch(); /* Wait for user input before exiting */
	/* End ncurses */
	endwin();

	return (selected_utxos);
}