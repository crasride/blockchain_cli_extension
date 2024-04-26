#ifndef PROVIDED_H
#define PROVIDED_H

void _print_hex_buffer(uint8_t const *buf, size_t len);

int _tx_in_print(tx_in_t const *in, unsigned int idx,
	char const *indent);

int _tx_out_print(tx_out_t const *out, unsigned int idx,
	char const *indent);

int _transaction_print_brief_loop(transaction_t const *transaction,
	unsigned int idx, char const *indent);

void _transaction_print_brief(transaction_t const *transaction);

int _transaction_print_loop(transaction_t const *transaction,
	unsigned int idx, char const *indent);

int _block_print_brief(block_t const *block, unsigned int index,
	char const *indent);

int _block_print(block_t const *block, unsigned int index,
	char const *indent);

void _blockchain_print(blockchain_t const *blockchain);

void _blockchain_print_brief(blockchain_t const *blockchain);

#endif
