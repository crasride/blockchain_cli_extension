#include "transaction.h"

/**
* hash_inputs - llist action func to hash inputs
* @transaction_input: tx_in_t * struct
* @destination_buffer: pointer to address to write to
* @idx: index of node
* Return: 0 if success else 1
*/
int inputs(llist_node_t transaction_input, unsigned int idx,
				void *destination_buffer)
{
	if (!transaction_input || !destination_buffer)
		return (1); /* Error: invalid input pointers */

	/* Copy input transaction data to destination buffer */
	memcpy(*(uint8_t **)destination_buffer, transaction_input,
		SHA256_DIGEST_LENGTH * 3);

	/* Advance the pointer in the destination buffer for the next copy */
	*(uint8_t **)destination_buffer += SHA256_DIGEST_LENGTH * 3;
	return (0);

	(void)idx; /* Evita el warning de "unused parameter" */
}

/**
* hash_outputs - llist action func to hash outputs
* @transaction_output: tx_out_t * struct
* @destination_buffer: pointer to address to write to
* @idx: index of node
* Return: 0 if success else 1
*/
int outputs(llist_node_t transaction_output, unsigned int idx,
				void *destination_buffer)
{
	if (!transaction_output || !destination_buffer)
		return (1); /* Error: invalid input pointers */

	/* Copy the hash of the output transaction to the destination buffer */
	memcpy(*(uint8_t **)destination_buffer,
		((tx_out_t *)transaction_output)->hash, SHA256_DIGEST_LENGTH);

	/* Advance the pointer in the destination buffer for the next copy */
	*(uint8_t **)destination_buffer += SHA256_DIGEST_LENGTH;
	return (0);

	(void)idx; /* Evita el warning de "unused parameter" */
}

/**
* transaction_hash - computes hash of given transaction
* @transaction: pointer to tx to hash
* @hash_buf: buffer to save hash
* Return: ptr to buffer or NULL
*/
uint8_t *transaction_hash(transaction_t const *transaction,
						uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	ssize_t buffer_size;
	uint8_t *temporary_buffer, *current_position;

	/* Check if the transaction is valid */
	if (!transaction || !hash_buf)
		return (NULL);

	/* Calculate the size needed for the temporary buffer */
	buffer_size = SHA256_DIGEST_LENGTH * 3 * llist_size(transaction->inputs) +
				SHA256_DIGEST_LENGTH * llist_size(transaction->outputs);

	if (buffer_size <= 0)
		return (NULL); /* Error: invalid buffer size */

	/* Allocates memory for the temporary buffer */
	temporary_buffer = current_position = calloc(1, buffer_size);
	if (!temporary_buffer)
		return (NULL); /* Error: memory allocation failed */

	/* Copy input transaction data to temporary buffer */
	llist_for_each(transaction->inputs, inputs, &current_position);

	/* Copy output transaction hashes to temporary buffer */
	llist_for_each(transaction->outputs, outputs, &current_position);

	/* Calculates SHA-256 hash of the temporary buffer and saves hash_buf */
	if (!sha256((const int8_t *)temporary_buffer, buffer_size, hash_buf))
	{
		free(temporary_buffer);
		return (NULL); /* Error: hash calculation failed */
	}

	/* Free the temporary buffer memory */
	free(temporary_buffer);

	/* Returns the calculated hash */
	return (hash_buf);
}

