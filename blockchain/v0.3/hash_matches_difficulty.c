#include "blockchain.h"

/**
* hash_matches_difficulty - Checks whether a given hash matches a given
* difficulty
* @hash: The hash to check
* @difficulty: The minimum difficulty the hash should match
*
* Return: 1 if the difficulty is respected, or 0 otherwise
*/
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
															uint32_t difficulty)
{
	/* Calculate the number of full bytes */
	uint32_t full_bytes = difficulty / 8;
	/* Calculate the number of remaining bits */
	uint8_t remaining_bits = difficulty % 8;
	/* Initialize the index */
	uint32_t i = 0;

	/* Check full bytes */
	for (i = 0; i < full_bytes; i++)
	{
		/* Check if the current byte is different from 0 */
		if (hash[i] != 0)
			return (0);
	}
	/* Check remaining bits */
	if (remaining_bits != 0 && (hash[full_bytes] >> (8 - remaining_bits)) != 0)
		return (0);

	return (1);
}


