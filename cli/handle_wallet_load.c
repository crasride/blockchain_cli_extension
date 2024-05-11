#include "cli.h"

/**
 * getLastFolderName - get the last folder name from a path
 * @path: the path to get the last folder name from
 * Return: the last folder name
 */
char *getLastFolderName(char *path)
{
	char *lastSlash = strrchr(path, '/');

	if (lastSlash == NULL)
		return (path);
	if (lastSlash[1] == '\0')
	{
		lastSlash[0] = '\0';
		return (getLastFolderName(path));
	}
	return (lastSlash + 1);
}

/**
* handle_wallet_load - handle the wallet_load command
* @state: pointer to store EC key
* @path: path to load the key pair
* Return: 0 if success, -1 if failed
*/
int handle_wallet_load(state_t *state, char *path)
{
	if (state->wallet)
		EC_KEY_free(state->wallet);
	state->wallet = ec_load(path);
	state->name = getLastFolderName(path);
	if (state->wallet)
	{
		printf("Loaded %s wallet\n", state->name);
		/* Print <base64 encoded data> Key pair */
		/* EC_KEY_print_fp(stdout, state->wallet, 0); */
		return (0);
	}
	else
	{
		printf("Failed to load EC key\n");
		if (state->wallet)
			EC_KEY_free(state->wallet);
		return (-1);
	}
}
