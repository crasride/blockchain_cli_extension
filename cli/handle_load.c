#include "cli.h"

/**
* handle_load - load the blockchain from a file
* @state: cli state
* @path: path to load the blockchain
* Return: 2 if number of arguments are wrong,
*         EXIT_FAILURE if call to a function fails
*         EXIT_SUCCESS otherwise
*/
int handle_load(state_t *state, char *path)
{
	blockchain_t *blockchain = NULL;

	blockchain = blockchain_deserialize(path);

	if (!blockchain)
	{
		fprintf(stdout, "Failed to load blockchain from %s\n",
			path);
		return (-1);
	}
	blockchain_destroy(state->blockchain);
	state->blockchain = blockchain;
	fprintf(stdout, "Loaded blockchain from %s\n", path);
	return (0);
}
