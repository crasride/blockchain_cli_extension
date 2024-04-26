#include "blockchain.h"

/**
 * fread_transactions - read the transactions in blocks
 * @block: Pointer to the Blocks of Blockchain
 * @fr: File stream to read from
 */

void fread_transactions(block_t *block, FILE *fr)
{
	int32_t i, j, tx_size, tx_in, tx_out;
	transaction_t *t_node;
	tx_in_t *in_node;
	tx_out_t *out_node;

	fread(&tx_size, 4, 1, fr);
	if (tx_size == -1)
	{
		block->transactions = NULL;
		return;
	}
	block->transactions = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < tx_size; i++)
	{
		t_node = malloc(sizeof(transaction_t));
		t_node->inputs = llist_create(MT_SUPPORT_FALSE);
		t_node->outputs = llist_create(MT_SUPPORT_FALSE);
		fread(&t_node->id, 32, 1, fr);
		/* Read nb inputs & outputs */
		fread(&tx_in, 4, 1, fr);
		fread(&tx_out, 4, 1, fr);
		for (j = 0; j < tx_in; j++)
		{
			in_node = malloc(sizeof(tx_in_t));
			fread(in_node, 169, 1, fr);
			llist_add_node(t_node->inputs, in_node, ADD_NODE_REAR);
		}
		for (j = 0; j < tx_out; j++)
		{
			out_node = malloc(sizeof(tx_out_t));
			fread(out_node, 101, 1, fr);
			llist_add_node(t_node->outputs, out_node, ADD_NODE_REAR);
		}
		llist_add_node(block->transactions, t_node, ADD_NODE_REAR);
	}
}

/**
 * check_header - check the header validity format
 * @header: Pointer to header struct
 * Return: 1 if valid, 0 otherwise
 */

int check_header(block_header_t *header)
{
	if (memcmp(header->magic, HBLK_MAGIC, 4) ||
		(memcmp(header->version, HBLK_VERSION, 3)))
		return (0);
	return (1);
}

/**
 * blockchain_deserialize - Deserializes (load) Bchain
 * from a file provided in 'path'
 * @path: path to the file to load the serialized Bchain from
 *
 * Return: Pointer to deserialized Bchain on Success, NULL Failure
 */

blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *fr;
	blockchain_t *bchain;
	block_t *block;
	uint32_t i;
	block_header_t header;
	unspent_tx_out_t *utxo;

	fr = fopen(path, "rb");
	if (!fr || !path)
		return (NULL);

	fread(&header, sizeof(header), 1, fr);
	if (!check_header(&header) || &header.blocks == 0)
	{
		fclose(fr);
		return (NULL);
	}

	bchain = malloc(sizeof(blockchain_t));
	bchain->chain = llist_create(MT_SUPPORT_FALSE);
	bchain->unspent = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < header.blocks; i++)
	{
		block = malloc(sizeof(block_t));
		fread(block, 1, sizeof(block->info), fr);
		fread(&block->data.len, 1, 4, fr);
		memset(block->data.buffer, 0, 1024);
		fread(&block->data.buffer, 1, block->data.len, fr);
		fread(&block->hash, 32, 1, fr);
		fread_transactions(block, fr);
		llist_add_node(bchain->chain, block, ADD_NODE_REAR);
	}
	for (i = 0; i < header.unspent; i++)
	{
		utxo = malloc(sizeof(unspent_tx_out_t));
		fread(utxo, 165, 1, fr);
		llist_add_node(bchain->unspent, utxo, ADD_NODE_REAR);
	}
	fclose(fr);
	return (bchain);
}
