#include "cli.h"


/**
* llist_get_node_data - Obtains the data stored in a node
*
* @node: Pointer to the node
*
* Return: Pointer to the data stored in the node
 */
void *llist_get_node_data(llist_node_t node)
{
	if (node == NULL)
	{
		printf("Node is NULL\n");
		return (NULL);
	}
	return ((block_t *)node);
}


/**
* _get_endianness - Get current endianness
*
* Return: 1 for little endian, 2 for big endian, 0 for unknown
*/
uint8_t _get_endianness(void)
{
	_endian_t order = { 1 };

	if (order.bytes[0] == 1)
		return (1); /* Little endian */
	if (order.bytes[0] == 0)
		return (2); /* Big endian */

	return (0); /* Should not happen */
}

/**
* sum_unspent - sum unspent transaction outputs
* @node: unspent transaction output
* @idx: index of the node
* @arg: pointer to the total
* Return: 0
*/
int sum_unspent(void *node, unsigned int idx, void *arg)
{
	unspent_tx_out_t *unspent_tx_out = node;
	uint32_t *total = arg;

	(void)idx;
	*total += unspent_tx_out->out.amount;
	return (0);
}

/**
* sum_unspent_wallet - sum unspent transaction outputs for a wallet
* @node: unspent transaction output
* @idx: index of the node
* @args: pointer to the total and the wallet
* Return: 0
*/
int sum_unspent_wallet(void *node, unsigned int idx, void *args)
{
	void **ptr = args;
	unspent_tx_out_t *unspent_tx_out = node;
	uint32_t *total = (uint32_t *)ptr[0];
	state_t *state = (state_t *)ptr[1];
	char *wallet_address, *utxo_address;
	uint8_t pub[EC_PUB_LEN] = {0};

	(void)idx;
	ec_to_pub(state->wallet, pub);
	wallet_address = bytes_to_hex(pub, EC_PUB_LEN);
	utxo_address = bytes_to_hex(unspent_tx_out->out.pub, EC_PUB_LEN);

	if (strcmp(wallet_address, utxo_address) == 0)
		*total += unspent_tx_out->out.amount;
	return (0);
}
