#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_



#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <llist.h>
#include <openssl/sha.h> /* SHA256_DIGEST_LENGTH */
#include <time.h>
/*task 5*/
#include <fcntl.h> /* open */
#include "./provided/endianness.h"
#define HBLK_MAGIC "HBLK"
#define HBLK_VERSION "0.3"

#define GENESIS_HASH "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
#define SHA256_DIGEST_LENGTH 32

#include "./transaction/transaction.h"
#include "../../crypto/hblk_crypto.h"
#include "./provided/endianness.h"

/* Constants */
#define BLOCKCHAIN_DATA_MAX 1024

/**
* struct block_data_s - Block data
*
* @buffer: Data buffer
* @len:    Data size (in bytes)
*/
typedef struct block_data_s
{
	/*
	* @buffer must stay first, so we can directly use the structure as
	* an array of char
	*/
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

/**
* struct block_info_s - Block info structure
*
* @index:      Index of the Block in the Blockchain
* @difficulty: Difficulty of proof of work (hash leading zero bits)
* @timestamp:  Time the Block was created at (UNIX timestamp)
* @nonce:      Salt value used to alter the Block hash
* @prev_hash:  Hash of the previous Block in the Blockchain
*/
typedef struct block_info_s
{
	/*
	* The order of the elements in this structure should remain the same.
	* It was designed so every element of this structure is aligned and
	* doesn't require padding from the compiler.
	* Therefore, it is possible to use the structure as an array of char,
	* on any architecture.
	*/
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
* struct block_s - Block structure
*
* @info: Block info
* @data: Block data
* @transactions: List of transactions
* @hash: 256-bit digest of the Block, to ensure authenticity
*/
typedef struct block_s
{
	block_info_t    info; /* This must stay first */
	block_data_t    data; /* This must stay second */
	llist_t *transactions;
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
* struct blockchain_s - Blockchain structure
*
* @chain: Linked list of pointers to block_t
* @unspent: Linked list of unspent transaction outputs
*/
typedef struct blockchain_s
{
	llist_t     *chain;
	llist_t     *unspent;
} blockchain_t;


/* Functions tasks */
blockchain_t *blockchain_create(void);

/* Functions Task1 */
block_t *block_create(block_t const *prev, int8_t const *data,
						uint32_t data_len);

/* Functions Task2 */
void block_destroy(block_t *block);

/* Functions Task3 */
void blockchain_destroy(blockchain_t *blockchain);

/* Functions Task4 */
uint8_t *block_hash(block_t const *block,
										uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

/* Functions Task5 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
/* Functions identifier type endian */
uint8_t _get_endianness(void);

/* Functions Task6 */
#define SWAPENDIAN32(x) (\
	(((x) & 0xff000000) >> 24) | \
	(((x) & 0x00ff0000) >> 8) | \
	(((x) & 0x0000ff00) << 8) | \
	(((x) & 0x000000ff) << 24) \
)
blockchain_t *blockchain_deserialize(char const *path);
llist_t *blocks(int fd, uint32_t size, uint8_t endianness);
void cleanup_blockchain_data(blockchain_t *chain, int fd);
void cleanup_block_data(block_t *block, llist_t *list);

/* Functions Task7 */
#define GENESIS_BLOCK { \
	{ /* info */ \
		0 /* index */, \
		0, /* difficulty */ \
		1537578000, /* timestamp */ \
		0, /* nonce */ \
		{0} /* prev_hash */ \
	}, \
	{ /* data */ \
		"Holberton School", /* buffer */ \
		16 /* len */ \
	}, \
	NULL, /* transactions */ \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03" \
}

int block_is_valid(block_t const *block, block_t const *prev_block,
														llist_t *all_unspent);

/*Proyect Blockchain - Transactions*/
/* Functions Task0 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
														uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

# define BLOCK_GENERATION_INTERVAL 1
# define DIFFICULTY_ADJUSTMENT_INTERVAL 5

/**
 * struct block_header_s - struct to hold and init header
 * @magic: magic bytes identififcation
 * @version: version bytes
 * @endian: endianness of file
 * @blocks: Number of blocks in the Bchain
 * @unspent: Number of unspent tx outputs in the Bchain
 */

typedef struct block_header_s
{
	unsigned char magic[4];
	unsigned char version[3];
	unsigned char endian;
	uint32_t blocks; /* 4 bytes 32 bits */
	uint32_t unspent;
} block_header_t;

/* __attribute__((warn_unused_result)); */
#endif /* _BLOCKCHAIN_H_ */
