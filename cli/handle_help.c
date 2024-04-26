#include "cli.h"


/**
* print_help - print help message
*/
void print_help(void)
{
	size_t i = 0;

	CommandInfo commands[] = {
	{"exit", "                Exit the program"},
	{"wallet_create", "        Create a new wallet"},
	{"wallet_save <path>", "Save the wallet to a file"},
	{"wallet_load <path>", "Load the wallet from a file"},
	{"mine", "                Mine new blocks"},
	{"send <amount> <address>", "Send coins to a specific address"},
	{"info", "                Display information about the blockchain"},
	{"info_block", "        Display information about the latest block"},
	{"list_utxo", "                List unspent transactions"},
	{"clear", "                Clear the screen"},
	{"ls", "                List files in the current directory"},
	{"load <path>", "        Load blockchain data from a file"},
	{"save <path>/<file>", "Save blockchain data to a file"},
	{"help", "                Display this help message"}
};

	printf("Available commands:\n");
	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i)
	{
		printf("%s\t\t\t%s\n", commands[i].command, commands[i].description);
	}
}
