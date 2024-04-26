#include "cli.h"

/**
* cleanup - free allocated memory before exiting
* @state: pointer to store EC key
*/
void cleanup(state_t *state)
{
	if (state)
	{
		if (state->wallet)
			EC_KEY_free(state->wallet);
		if (state->blockchain)
			blockchain_destroy(state->blockchain);
		if (state->tx_pool)
			llist_destroy(state->tx_pool, 1, (node_dtor_t)transaction_destroy);
		free(state);
	}
}
