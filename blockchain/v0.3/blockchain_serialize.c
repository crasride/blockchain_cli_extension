#include "blockchain.h"


/**
* write_transactions - serialize the transactions to the file descriptor "fd"
* @transactions: Points to Linked list of transactions
* @fd: file descriptor
* Return: Nothing
*               Transactions are serialized as a table:
* Offset Size (bytes)                        Field           Purpose
* 0x28   169 * nb\_inputs                    inputs   List transaction  inputs
* 0x28 + 169 * nb\_inputs 101 * nb\_outputs  outputs  List transaction  outputs
*
*/
void write_transactions(llist_t *transactions, int fd)
{
	int tr_index; /* Transaction index */
	int io_index; /* Input-output iterate transaction*/
	int tx_in;/* Number of tx inputs */
	int tx_out;/* Number of tx outputs */
	transaction_t *t_node;/* Transaction */
	tx_in_t *current_tr_input;/* Transaction input data */
	tx_out_t *current_tr_output;/* Transaction output data */

	/* Loop through all transactions */
	for (tr_index = 0; tr_index < llist_size(transactions); tr_index++)
	{
		/* Get the transaction */
		t_node = llist_get_node_at(transactions, tr_index);
		/* Write the transaction to the file */
		tx_in = llist_size(t_node->inputs);
		/* Number of tx outputs */
		tx_out = llist_size(t_node->outputs);
		/* Write the transaction to the file */
		write(fd, &t_node->id, SHA256_DIGEST_LENGTH);
		/* Number of tx inputs */
		write(fd, &tx_in, 4);
		/* Number of tx outputs */
		write(fd, &tx_out, 4);
		/* Now list of tx_inputs => 169 times number of inputs */
		for (io_index = 0; io_index < tx_in; io_index++)
		{
			/* Get the input */
			current_tr_input = llist_get_node_at(t_node->inputs, io_index);
			/* Write the input to the file */
			write(fd, current_tr_input, 169);
		}
		/* Same for list of outputs => 101 times number of outputs */
		for (io_index = 0; io_index < tx_out; io_index++)
		{
			/* Get the output */
			current_tr_output = llist_get_node_at(t_node->outputs, io_index);
			/* Write the output to the file */
			write(fd, current_tr_output, 101);
		}
	}
}

/**
* init_block_header - initializes a block_header struct
* @header: Pointer to block_header struct
*/
void init_block_header(block_header_t *header)
{
	/* Get the endianness */
	uint8_t _get_endianness(void);
	/* Check if the header is NULL */
	if (!header)
		return;
	/* Initialize the block header */
	memcpy(header->magic, "HBLK", 4);
	/* Set the blockchain version */
	memcpy(header->version, "0.3", 3);
	/* Set the endianness */
	header->endian = _get_endianness();
	/* Set the number of blocks */
	header->blocks = 0;
	/* Set the number of unspent outputs */
	header->unspent = 0;
}

/**
* blockchain_serialize - serializes a Blockchain into a file
* @blockchain: Points to the blockchain to serialize
* @path: Path to the file in which to serialize the blockchain
* Return: 0 on Success, -1 upon Failure
*            File Header the following table describes the file header
* Offset    Size (bytes)         Field           Purpose
* 0x10 + ?  165 * hblk_unspent   unspent...List of unspent transaction outputs
*/
int blockchain_serialize(blockchain_t const *blockchain,
						char const *path)
{
	int fd = -1;
	uint32_t i;
	block_header_t header;
	block_t *block;
	unspent_tx_out_t *unspent_node;
	int tx_size = 0;
	/* Poen the file and check if the blockchain is NULL */
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd < 0 || !blockchain || !path)
		return (-1);
	/* Initialize the block header */
	init_block_header(&header);
	header.blocks = llist_size(blockchain->chain);
	header.unspent = llist_size(blockchain->unspent);
	write(fd, &header, sizeof(header));

	for (i = 0; i < header.blocks; i++)
	{
		block = llist_get_node_at(blockchain->chain, i);
		write(fd, block, sizeof(block->info));
		write(fd, &block->data.len, sizeof(block->data.len));
		write(fd, block->data.buffer, block->data.len);
		write(fd, &block->hash, SHA256_DIGEST_LENGTH);
		/* If we are on genesis block */
		if (block->info.index == 0)
			tx_size = -1;
		else
			tx_size = llist_size(block->transactions);
		write(fd, &tx_size, 4);
		write_transactions(block->transactions, fd);
	}
	/* Write the unspent transactions */
	for (i = 0; i < header.unspent; i++)
	{
		unspent_node = llist_get_node_at(blockchain->unspent, i);
		write(fd, unspent_node, 165);
	}
	close(fd);
	return (0);
}
