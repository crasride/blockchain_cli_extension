#include "cli.h"



/**
* handle_info_selection - handle the info selection
* @state: CLI state
* Return: 0 if success, -1 if failed
*/
int handle_info_selection(state_t *state)
{
	char input[256];

	printf("Select info type:\n");
	printf("1. Blockchain Info\n");
	printf("2. Blocks Info\n");
	printf("3. UTXOs Info\n");
	printf("Enter option number: ");

	if (fgets(input, sizeof(input), stdin) != NULL)
	{
		int selected_option = atoi(input);

		switch (selected_option)
		{
			case 1:
				return handle_info(state);
			case 2:
				return handle_info_block(state);
			case 3:
				return generate_unspent_list(state->blockchain);
			default:
				printf("Invalid option\n");
				return -1;
		}
	}
	else
	{
		printf("Error reading input\n");
		return -1;
	}
}


/**
* find_command - find the command to execute
* @cmd: command
* @arg1: argument 1
* @arg2: argument 2
* @state: pointer to store EC key
* @receiver_address: receiver address
* Return: 0 if ok, 1 if exit
*/
int find_command(char *cmd, char *arg1, char *arg2, state_t *state,
					char *receiver_address)
{
	if (strcmp(cmd, "exit") == 0)
		return (handle_exit());
	else if (strcmp(cmd, "wallet_create") == 0)
	{
		if (arg1)
		{
			printf("Usage: wallet_create\n");
			printf("Too many arguments\n");
			return (-1);
		}
		else
			return (handle_wallet_create(state));
	}
	else if (strcmp(cmd, "wallet_save") == 0)
	{
		if (!arg1)
		{
			printf("Usage: wallet_save <path>\n");
			return (-1);
		}
		else
			return (handle_wallet_save(state, arg1));
	}
	else if (strcmp(cmd, "wallet_load") == 0)
	{
		if (!arg1)
		{
			printf("Usage: wallet_load <path>\n");
			return (-1);
		}
		else
			return (handle_wallet_load(state, arg1));
	}
	else if (strcmp(cmd, "mine") == 0)
	{
		if (arg1)
		{
			if (is_number(arg1))
			{
				if (!arg2)
					arg2 = "0";
				if (is_number(arg2))
					return (handle_mine_auto(state, atoi(arg1), atoi(arg2)));
				else
				{
					printf("Argument 2 must be a number\n");
					return (-1);
				}
			}
			else
			{
				printf("Argument1 must be a number\n");
				return (-1);
			}
		}
		else
			return (handle_mine(state));
	}
	else if (strcmp(cmd, "send") == 0)
	{
		if (!arg1 || !arg2)
		{
			printf("Usage: send <amount> <address>\n");
			return (-1);
		}
		else
		{
			int amount = atoi(arg1);

			strncpy(receiver_address, arg2, (EC_PUB_LEN * 2) + 1);
			return (handle_send(amount, receiver_address, state));
		}
	}
	else if (strcmp(cmd, "custom_send") == 0)
	{
		if (!arg1 || !arg2)
		{
			printf("Usage: custom_send <amount> <address>\n");
			return (-1);
		}
		else
		{
			int amount = atoi(arg1);

			strncpy(receiver_address, arg2, (EC_PUB_LEN * 2) + 1);
			return (handle_send_custom(amount, receiver_address, state));
		}
	}
	else if (strcmp(cmd, "info_blockchain") == 0)
		return (handle_info(state));
	else if (strcmp(cmd, "info_block") == 0)
		return handle_info_block(state);
	else if (strcmp(cmd, "help") == 0)
	{
		print_help();
		return 0;
	}
	else if (strcmp(cmd, "info_utxo") == 0)
	{
		if (arg1)
		{
			printf("Usage: list\n");
			printf("Too many arguments\n");
			return (-1);
		}
		else
		{
			return generate_unspent_list(state->blockchain);
		}
	}
	else if (strcmp(cmd, "clear") == 0)
	{
		system("clear");
		return 0;
	}
	else if (strcmp(cmd, "ls") == 0)
	{
		system("ls");
		return 0;
	}
	else if (strcmp(cmd, "load") == 0)
	{
		if (!arg1)
		{
			printf("Usage: load <path>\n");
			return (-1);
		}
		else
			return (handle_load(state, arg1));
	}
	else if (strcmp(cmd, "save") == 0)
	{
		if (!arg1)
		{
			printf("Usage: save <path>/<file>\n");
			return (-1);
		}
		else
			return (handle_save(state, arg1));
	}
	else if (strcmp(cmd, "info") == 0)
	{
		if (arg1)
		{
			if (strcmp(arg1, "blockchain") == 0)
			{
				return handle_info(state);
			}
			else if (strcmp(arg1, "block") == 0)
			{
				return handle_info_block(state);
			}
			else if (strcmp(arg1, "utxo") == 0 && strcmp(arg2, "sorted") == 0)
			{
				return generate_sorted_unspent_list(state->blockchain);
			}
			else if (strcmp(arg1, "utxo") == 0)
			{
				return generate_unspent_list(state->blockchain);
			}
			else
			{
				printf("Invalid option\n");
				return -1;
			}
		}
		else
		{
			return handle_info_selection(state);
		}
	}
	else
	{
		printf("unknown command: \"%s\"\n", cmd);
		return (-1);
	}
}

/**
* print_logo - prints the Holberton logo
*/
void print_logo(void)
{
	char *filename = "./logo.txt";
	FILE *f = fopen(filename, "r");
	char buffer[256];

	if (f == NULL)
	{
		printf("Error: could not open file %s", filename);
	}

	printf("\n                                  HOLBERTON SCHOOL\n");
	printf("\033[1;31m");
	while (fgets(buffer, 256, f))
		printf("%s", buffer);
	printf("\033[0m");
	printf("\n                                Define your future\n\n");

	fclose(f);
}


/**
* main - Entry point
* Return: 0 if ok, 1 if failed
*/
int main(void)
{
	int ret = 0;
	char *line = NULL, *cmd, *arg1, *arg2, receiver_address[EC_PUB_LEN * 2];
	state_t *state = NULL;
	blockchain_t *blockchain = blockchain_create();
	llist_t *tx_pool = llist_create(MT_SUPPORT_FALSE);

	print_logo();

	if (!blockchain)
	{
		fprintf(stderr, "Error: Fallo al crear la cadena de bloques\n");
		return (1);
	}
	if (!tx_pool)
	{
		fprintf(stderr, "Error: Fallo al crear la lista de transacciones\n");
		blockchain_destroy(blockchain);
		return (1);
	}
	state = calloc(1, sizeof(state_t));
	if (!state)
	{
		fprintf(stderr, "Error: Fallo al asignar memoria para el estado\n");
		blockchain_destroy(blockchain);
		llist_destroy(tx_pool, 1, NULL);
		return (1);
	}
	state->blockchain = blockchain;
	state->tx_pool = tx_pool;
	state->status = 0;
	state->wallet = NULL;

	while (1)
	{

		line = readline(C_PURPLE "Holbie blockchain> " C_RESET);

		if (!line)
			break;

		if (line[0] != '\0')
		{
			add_history(line);
			cmd = strtok(line, " ");
			arg1 = strtok(NULL, " ");
			arg2 = strtok(NULL, " ");

			ret = find_command(cmd, arg1, arg2, state, receiver_address);
			if (ret == 1)
				break;
		}

		free(line);
	}
	if (line)
		free(line);
	cleanup(state);
	return (ret != 0);
}
