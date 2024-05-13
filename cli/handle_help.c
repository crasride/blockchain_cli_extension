#include "cli.h"


/**
* print_help - print help message
*/
void print_help(void)
{
	size_t i = 0;

	CommandInfo commands[] = {
	{"wallet_create", "                Create a new wallet"},
	{"wallet_save <path>", "        Save the wallet to a file"},
	{"wallet_load <path>", "        Load the wallet from a file"},
	{"mine", "                        Mine new blocks"},
	{"mine <block quantity>", "        Mine a specified quantity of blocks"},
	{"mine <block quantity> <time>", "Specific number of blocks and mining time between of blocks"},
	{"send <amount> <address>", "        Send coins to a specific address"},
	{"custom_send <amount> <address>",
	"It will be asked to specify each utxo to select for this transaction and each utxo to be created for receiver and sender (change)"},
	{"info json", "                Export Blockchain data to JSON file"},
	{"info", "                        Display information about the blockchain"},
	{"info blockchain", "                Display information about the blockchain."},
	{"info_block", "                Display information about the block"},
	{"list_utxo", "                List unspent transactions"},
	{"save <path>/<file>", "        Save blockchain data to a file"},
	{"load <path>", "                Load blockchain data from a file"},
	{"ls", "                        List files in the current directory"},
	{"clear", "                        Clear the screen"},
	{"load <path>", "                Load blockchain data from a file"},
	{"↑ ↓ ← →", "                        Direction arrows."},
	{"help", "                        Display information about available commands."},
	{"exit", "                        Exit the program"}
};

	printf("Available commands:\n");
	for (i = 0; i < sizeof(commands) / sizeof(commands[0]); ++i)
	{
		printf("%s\t\t\t%s\n", commands[i].command, commands[i].description);
	}
}
