#include "cli.h"

/**
* handle_save - save blockchain to a file
* @state: cli state
* @path: path to save the blockchain
* Return: 2 if number of arguments are wrong,
*         EXIT_FAILURE if call to a function fails
*         EXIT_SUCCESS otherwise
*/
int handle_save(state_t *state, char *path)
{
	if (blockchain_serialize(state->blockchain, path) == -1)
	{
		fprintf(stdout, "Failed to save blockchain in file %s\n",
			path);
		return (-1);
	}

	fprintf(stdout, "Saved blockchain to %s\n", path);
	return (0);
}
