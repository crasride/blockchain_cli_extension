#include "cli.h"

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
	if (state->wallet)
	{
		printf("Load wallet (EC key pair)\n");
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
