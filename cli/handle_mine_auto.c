#include "cli.h"

/**
* handle_mine_auto - handle the mine_auto command
* @state: CLI state
* @nb_block: number of blocks to mine
* @interval: interval between each block
* Return: 0 if success, -1 if failed
*/
int handle_mine_auto(state_t *state, int nb_block, int interval)
{
	int i;

	printf("auto mining started\n");
	for (i = 0; i < nb_block; i++)
	{
		sleep(interval);
		handle_mine(state);
	}
	printf("auto mining finished\n");

	return (0);
}
