#include "cli.h"

/**
* handle_wallet_save - handle the wallet_save command
* @state: pointer to store EC key
* @path: path to save the key pair
* Return: 0 if success, -1 if failed
*/
int handle_wallet_save(state_t *state, char *path)
{
	if (!state->wallet)
	{
		printf("No EC key available\n");
		return (-1);
	}
	if (ec_save(state->wallet, path))
	{
		state->name = strdup(getLastFolderName(path));
		printf("Save wallet (EC key pair)\n");
		return (0);
	}
	else
	{
		printf("Failed to save EC key\n");
		return (-1);
	}
}
