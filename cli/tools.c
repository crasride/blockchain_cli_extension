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
*check_string - checks if a string is a valid hex string
*@address: string to check
*@pub: public key
*@len: length of the string
*Return: 1 if valid, 0 if not
*/
int check_string(char *address, uint8_t *pub, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
	{
		if (isdigit(address[i]))
			continue;
		else if (address[i] >= 'a' && address[i] <= 'f')
			continue;
		else if (address[i] >= 'A' && address[i] <= 'F')
			address[i] = tolower(address[i]);
		else
		{
			free(pub);
			return (0);
		}
	}
	return (1);
}

/**
* string_to_pub - converts a EC_KEY public key in a lowercase or
* uppercase hex string to a byte array
*
* @address: public key of EC_PUB_LEN bytes, in hex
*
* Return: byte array on success, or NULL on failure
*/
uint8_t *string_to_pub(char *address)
{
	uint8_t *pub;
	unsigned int scan;
	size_t i, j, len;

	if (!address)
	{
		fprintf(stderr, "string_to_pub: NULL parameter\n");
		return (NULL);
	}
	pub = calloc(EC_PUB_LEN, sizeof(uint8_t));
	if (!pub)
	{
		fprintf(stderr, "string_to_pub: calloc failure\n");
		return (NULL);
	}
	len = strlen(address);
	if (len < (EC_PUB_LEN * 2))
	{
		free(pub);
		return (NULL);
	}
	if (!check_string(address, pub, len))
		return (NULL);

	for (i = 0, j = 0; i < EC_PUB_LEN; i++, j += 2)
	{
		if (sscanf(address + j, "%02x", &scan) == -1)
		{
			perror("string_to_pub: sscanf");
			free(pub);
			return (NULL);
		}
		pub[i] = scan;
	}

	return (pub);
}
