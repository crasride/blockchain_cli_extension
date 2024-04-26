#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "../../../crypto/hblk_crypto.h"
#define C_RESET   "\x1b[0m"
#define C_RED     "\x1b[31m"

#include <llist.h>

/**
* struct transaction_s - Transaction structure
*
* @id:      Transaction ID. A hash of all the inputs and outputs.
*           Prevents further alteration of the transaction.
* @inputs:  List of `tx_in_t *`. Transaction inputs
* @outputs: List of `tx_out_t *`. Transaction outputs
*/
typedef struct transaction_s
{
	uint8_t     id[SHA256_DIGEST_LENGTH];
	llist_t     *inputs;
	llist_t     *outputs;
} transaction_t;

/**
* struct tx_out_s - Transaction output
*
* @amount: Amount received
* @pub:    Receiver's public address
* @hash:   Hash of @amount and @pub. Serves as output ID
*/
typedef struct tx_out_s
{
	uint32_t    amount;
	uint8_t     pub[EC_PUB_LEN];
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
* struct tx_in_s - Transaction input
*
* Description: A transaction input always refers to a previous
* transaction output. The only exception is for a Coinbase transaction, that
* adds new coins to ciruclation.
*
* @block_hash:  Hash of the Block containing the transaction @tx_id
* @tx_id:       ID of the transaction containing @tx_out_hash
* @tx_out_hash: Hash of the referenced transaction output
* @sig:         Signature. Prevents anyone from altering the content of the
*               transaction. The transaction input is signed by the receiver
*               of the referenced transaction output, using their private key
*/
typedef struct tx_in_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	uint8_t     tx_out_hash[SHA256_DIGEST_LENGTH];
	sign_t       sig;
} tx_in_t;

/**
* struct unspent_tx_out_s - Unspent transaction output
*
* Description: This structure helps identify transaction outputs that were not
* used in any transaction input yet, making them "available".
*
* @block_hash: Hash of the Block containing the transaction @tx_id
* @tx_id:      ID of the transaction containing @out
* @out:        Copy of the referenced transaction output
*/
typedef struct unspent_tx_out_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t    out;

} unspent_tx_out_t;

/* Protypes Functions Porject Blockchain - Transactions */

/* Functions Task 0*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

/* Functions Task 1*/
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);

/* Functions Task 2*/
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);

/* Functions Task 3*/
uint8_t *transaction_hash(transaction_t const *transaction,
							uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

/* Functions Task 4*/
sign_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
					EC_KEY const *sender, llist_t *all_unspent);

/* Functions Task 5*/
void print_utxo_message(void);
transaction_t *transaction_create(EC_KEY const *sender,
									EC_KEY const *receiver,
									uint32_t amount, llist_t *all_unspent,
									llist_t *tx_pool);

/* Functions Task 6*/
int transaction_is_valid(transaction_t const *transaction,
							llist_t *all_unspent);

/* Functions Task 7*/
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index);
#define COINBASE_AMOUNT 50

/* Functions Task 8*/
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);

/* Functions Task 9*/
void transaction_destroy(transaction_t *transaction);

/* Functions Task 14*/
int update_unspent(llist_t *transactions,
				uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent);

int check_unspent(llist_node_t node, unsigned int idx, void *args);
int used_in_pool(llist_t *tx_pool, unspent_tx_out_t *unspent);
int sign_transaction_inputs(llist_node_t node, unsigned int idx, void *args);
llist_t *send_amount(EC_KEY const *sender, EC_KEY const *receiver,
					int32_t amount, uint32_t total);
int select_unspent_in(llist_node_t node, unsigned int idx, void *args);

/* Transaction create custom */
transaction_t *transaction_create_custom(EC_KEY const *sender,
									EC_KEY const *receiver,
									uint32_t amount, llist_t *all_unspent,
									llist_t *tx_pool, llist_t *selected_unspent);

#endif /* _TRANSACTION_H_ */
