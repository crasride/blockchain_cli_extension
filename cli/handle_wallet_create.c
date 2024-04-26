#include "cli.h"

/**
* handle_wallet_create - handle the wallet_create command
* @state: pointer to store EC key
* Return: 0 if success, -1 if failed
*/
int handle_wallet_create(state_t *state)
{
	if (state->wallet)
		EC_KEY_free(state->wallet);
	state->wallet = ec_create();
	if (state->wallet)
	{
		printf("Create wallet (EC key)\n");
		return (0);
	}
	else
	{
		printf("Failed to create EC key\n");
		return (-1);
	}
}
