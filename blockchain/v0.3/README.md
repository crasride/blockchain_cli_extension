<h1 align="center">Blockchain - Transactions</h1>


### Concepts
For this project, we expect you to look at this concept:

[[EYNTK] Blockchain - Transactions](./Blockchain_Transactions.md)

### Resources
**Read or watch:**

* [How a Bitcoin transaction works, CNN](https://www.ccn.com/ex-bitcoin-skeptic-ron-paul-says-crypto-could-prevent-recession/)
* [UTXO vs Account-based transaction model](https://blockonomi.com/utxo-vs-account-based-transaction-models/)
* [Provided libraries and sources](https://github.com/hs-hq/holbertonschool-blockchain)

### Learning Objectives
At the end of this project, you are expected to be able to explain to anyone, without the help of Google:

### General
* How to build a transaction
* What is a transaction input
* What is a transaction output
* Why to sign a transaction input
* How to compute the balance of given wallet
* What is a coinbase transaction and why it is used
* How to ensure immutability of a transaction
* What is the difference between the UTXO and the Account-based transaction models

### Requirements
### General

* Allowed editors: `vi, vim, emacs`
* All your files will be compiled on Ubuntu `14.04 LTS`
* Your C programs and functions will be compiled with `gcc 4.8.4` using the flags `-Wall -Werror -Wextra, -pedantic -Wno-deprecated-declarations` **and the linker flags** `-lssl` and `-lcrypto`
* All your files should end with a new line
* A `README.md` file, at the root of the folder of the project, is mandatory
* Your code should use the `Betty` style. It will be checked using [betty-style.pl](https://github.com/hs-hq/Betty/blob/main/betty-style.pl) and [betty-doc.pl](https://github.com/hs-hq/Betty/blob/main/betty-doc.pl)
* Your library `libhblk_crypto.a`, located in the `crypto` folder will be built and linked during correction
* The prototypes of all your functions should be included in your header file called `blockchain/v0.3/blockchain.h`
* All your header files should be include guarded


### More Info

### Prerequisites

* Create the folder `blockchain/v0.3/` and copy the following files in it:

	* blockchain/v0.2/blockchain_create.c
	* blockchain/v0.2/blockchain_deserialize.c
	* blockchain/v0.2/blockchain_destroy.c
	* blockchain/v0.2/blockchain.h
	* cblockchain/v0.2/blockchain_serialize.c
	* blockchain/v0.2/block_create.c
	* blockchain/v0.2/block_destroy.c
	* blockchain/v0.2/block_hash.c
	* blockchain/v0.2/block_is_valid.c
* Modify your code so when a Blockchain is serialized, the version stored is `0.3`

### Additional information

* Unless specified otherwise, you are allowed to use the C standard library
* You are free to use any data structure that suits you as long as their purpose is well defined
* You are free to print any information **on stderr**, this stream will be discarded during correction. Since you are going to use these utility functions in your Blockchain project, you are free to manage errors as you wish.
* At the end of this project, the structure of your repository should look like this (you may have additional files):

```
holbertonschool-blockchain
├── blockchain
│   ├── v0.1/
│   ├── v0.2/
│   └── v0.3
│       ├── blockchain_create.c
│       ├── blockchain_deserialize.c
│       ├── blockchain_destroy.c
│       ├── blockchain_difficulty.c
│       ├── blockchain.h
│       ├── blockchain_serialize.c
│       ├── block_create.c
│       ├── block_destroy.c
│       ├── block_hash.c
│       ├── block_is_valid.c
│       ├── block_mine.c
│       ├── hash_matches_difficulty.c
│       ├── libhblk_blockchain_v0.3.a
│       ├── Makefile
│       ├── provided
│       │   ├── _blockchain_print.c
│       │   ├── _endianness.c
│       │   ├── endianness.h
│       │   ├── _genesis.c
│       │   ├── _print_hex_buffer.c
│       │   └── _transaction_print.c
│       ├── test
│       │   ├── blockchain_create-main.c
│       │   ├── blockchain_deserialize-main.c
│       │   ├── blockchain_destroy-main.c
│       │   ├── blockchain_difficulty-main.c
│       │   ├── blockchain_serialize-main.c
│       │   ├── block_create-main.c
│       │   ├── block_destroy-main.c
│       │   ├── block_hash-main.c
│       │   ├── block_is_valid-main.c
│       │   ├── block_mine-main.c
│       │   └── hash_matches_difficulty-main.c
│       └── transaction
│           ├── coinbase_create.c
│           ├── coinbase_is_valid.c
│           ├── test
│           │   ├── coinbase_create-main.c
│           │   ├── coinbase_is_valid-main.c
│           │   ├── transaction_create-main.c
│           │   ├── transaction_destroy-main.c
│           │   ├── transaction_hash-main.c
│           │   ├── transaction_is_valid-main.c
│           │   ├── tx_in_create-main.c
│           │   ├── tx_in_sign-main.c
│           │   ├── tx_out_create-main.c
│           │   ├── unspent_tx_out_create-main.c
│           │   └── update_unspent-main.c
│           ├── transaction_create.c
│           ├── transaction_destroy.c
│           ├── transaction.h
│           ├── transaction_hash.c
│           ├── transaction_is_valid.c
│           ├── tx_in_create.c
│           ├── tx_in_sign.c
│           ├── tx_out_create.c
│           ├── unspent_tx_out_create.c
│           └── update_unspent.c
├── crypto/
└── README.md
```

### Tasks

### [0. Create transaction output]()

Write a function that allocates and initializes a transaction output structure

* Prototype: `tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);`, where:
	* `amount` is the amount of the transaction
	* `pub` is the public key of the transaction receiver
* Your function must return a pointer to the created transaction output upon success, or `NULL` upon failure

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/tx_out_create-main.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "hblk_crypto.h"
#include "transaction.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _tx_out_print - Print a transaction output
 *
 * @out: Pointer to the transaction output to print
 */
static void _tx_out_print(tx_out_t const *out)
{
    if (!out)
        return;

    printf("Transaction output: {\n");

    printf("\tamount: %u,\n", out->amount);
    printf("\tpub: ");
    _print_hex_buffer(out->pub, sizeof(out->pub));
    printf(",\n");
    printf("\thash: ");
    _print_hex_buffer(out->hash, sizeof(out->hash));
    printf("\n");

    printf("}\n");
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    EC_KEY *receiver;
    uint8_t receiver_pub[EC_PUB_LEN];
    tx_out_t *out;

    receiver = ec_create();

    out = tx_out_create(972, ec_to_pub(receiver, receiver_pub));
    _tx_out_print(out);

    free(out);
    EC_KEY_free(receiver);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/tx_out_create-test transaction/tx_out_create.c provided/_print_hex_buffer.c transaction/test/tx_out_create-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/tx_out_create-test
Transaction output: {
    amount: 972,
    pub: 042835908c04f2b9a75153da5e4a4660222a5970cfdda92a1abb38c140a2d65f7ef6e55663284f8a5bbc7b6eb8e78f0b98cd2b2790df2b63c6551f7cb6a9fa9aeb,
    hash: 413ad94a0b12db7f99e4b8cdaf3af0b0d2560801fb8a49d59e8d4715b0123dd7
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### ### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/tx_out_create.c`

---
### [1. Create unspent transaction output]()

Write a function that allocates and initializes an unspent transaction output structure

* Prototype: `unspent_tx_out_t *unspent_tx_out_create(uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH], tx_out_t const *out);`, where:
	* `block_hash` contains the hash of the Block where the referenced transaction output is located
	* `tx_id` contains the hash of a transaction in the Block block_hash, where the referenced transaction output is located
	* `out` points to the referenced transaction output
* Your function must return a pointer to the created unspent transaction output upon success, or `NULL` upon failure

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/unspent_tx_out_create-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _unspent_tx_out_print - Print an unspent transaction output
 *
 * @unspent: Pointer to the unspent transaction output to print
 */
static void _unspent_tx_out_print(unspent_tx_out_t const *unspent)
{
    if (!unspent)
        return;

    printf("Unspent transaction output: {\n");

    printf("\tblock_hash: ");
    _print_hex_buffer(unspent->block_hash, sizeof(unspent->block_hash));
    printf(",\n");
    printf("\ttx_id: ");
    _print_hex_buffer(unspent->tx_id, sizeof(unspent->tx_id));
    printf(",\n");

    printf("\tout: {\n");

    printf("\t\tamount: %u,\n", unspent->out.amount);
    printf("\t\tpub: ");
    _print_hex_buffer(unspent->out.pub, sizeof(unspent->out.pub));
    printf(",\n");
    printf("\t\thash: ");
    _print_hex_buffer(unspent->out.hash, sizeof(unspent->out.hash));
    printf("\n");

    printf("\t}\n}\n");
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t transaction_id[SHA256_DIGEST_LENGTH];
    tx_out_t *out;
    uint8_t pub[EC_PUB_LEN];
    EC_KEY *receiver;
    unspent_tx_out_t *unspent;

    sha256((int8_t *)"Block", strlen("Block"), block_hash);
    sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
    receiver = ec_create();
    out = tx_out_create(972, ec_to_pub(receiver, pub));

    unspent = unspent_tx_out_create(block_hash, transaction_id, out);
    _unspent_tx_out_print(unspent);

    EC_KEY_free(receiver);
    free(out);
    free(unspent);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/unspent_tx_out_create-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c provided/_print_hex_buffer.c transaction/test/unspent_tx_out_create-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/unspent_tx_out_create-test
Unspent transaction output: {
    block_hash: 211d0bb8cf4f5b5202c2a9b7996e483898644aa24714b1e10edd80a54ba4b560,
    tx_id: eec26ddd9a408449f8e06c622c11d61b94341b04fae5eac8d755c477b8294624,
    out: {
        amount: 972,
        pub: 04ff3188241457604d7aa47c3c532cbbd0e38201348acb3bf898f0ff2100741d3e5d954ace88b78bd244aeaab3d5179e16d3119c776d614e4ed14afec658bcb169,
        hash: 8b59eea4bc0b844a149f71c5bfc476c8911b28cb2f52cf229e566ae27fc8ca6d
    }
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/unspent_tx_out_create.c`

---
### [2. Create transaction input]()

Write a function that allocates and initializes a transaction input structure

* Prototype: tx_in_t *tx_in_create(unspent_tx_out_t const *unspent);, where
	* unspent points to the unspent transaction output to be converted to a transaction input
* Your function must return a pointer to the created transaction input upon success, or NULL upon failure
* The created transaction input’s signature structure must be zeroed

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/tx_in_create-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _tx_in_print - Print a transaction input
 *
 * @in: Pointer to the transaction input to print
 */
static void _tx_in_print(tx_in_t const *in)
{
    if (!in)
        return;

    printf("Transaction input: {\n");

    printf("\tblock_hash: ");
    _print_hex_buffer(in->block_hash, sizeof(in->block_hash));
    printf(",\n");
    printf("\ttx_id: ");
    _print_hex_buffer(in->tx_id, sizeof(in->tx_id));
    printf(",\n");
    printf("\ttx_out_hash: ");
    _print_hex_buffer(in->tx_out_hash, sizeof(in->tx_out_hash));
    printf("\n");
    printf("\tsig: ");
    if (in->sig.len)
        _print_hex_buffer(in->sig.sig, in->sig.len);
    else
        printf("null");
    printf("\n");

    printf("}\n");
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t transaction_id[SHA256_DIGEST_LENGTH];
    tx_out_t *out;
    uint8_t pub[EC_PUB_LEN];
    EC_KEY *receiver;
    unspent_tx_out_t *unspent;
    tx_in_t *in;

    sha256((int8_t *)"Block", strlen("Block"), block_hash);
    sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
    receiver = ec_create();
    out = tx_out_create(972, ec_to_pub(receiver, pub));
    unspent = unspent_tx_out_create(block_hash, transaction_id, out);

    in = tx_in_create(unspent);
    _tx_in_print(in);

    EC_KEY_free(receiver);
    free(out);
    free(unspent);
    free(in);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/tx_in_create-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c provided/_print_hex_buffer.c transaction/test/tx_in_create-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/tx_in_create-test
Transaction input: {
    block_hash: 211d0bb8cf4f5b5202c2a9b7996e483898644aa24714b1e10edd80a54ba4b560,
    tx_id: eec26ddd9a408449f8e06c622c11d61b94341b04fae5eac8d755c477b8294624,
    tx_out_hash: 6bd6fbe4d27b5c6a520dcff19dc61a419be9b09000bc4af1d5d96920e0a9548c
    sig: null
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/tx_in_create.c`

---

### [3. Transaction ID]()

Write a function that computes the ID (hash) of a transaction

* Prototype: `uint8_t *transaction_hash(transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);`, where:
	* `transaction` points to the transaction to compute the hash of
	* `hash_buf` is a buffer in which to store the computed hash
* `transaction->id` must be left unchanged
* Your function must return a pointer to `hash_buf`
* The transaction ID must be a hash of a buffer containing the following information
	* Each input’s `block_hash` (32 bytes), `tx_id` (32 bytes) and `tx_out_hash` (32 bytes), followed by
	* Each output’s `hash` (32 bytes)
	* For example, the buffer for a transaction with 4 inputs and 2 outputs would be `448` bytes long `(32 * 3 * 4 + 32 * 2)`

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/transaction_hash-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t transaction_id[SHA256_DIGEST_LENGTH];
    tx_out_t *out, *out2;
    uint8_t pub[EC_PUB_LEN];
    EC_KEY *owner;
    llist_t *all_unspent;
    unspent_tx_out_t *unspent;
    tx_in_t *in;
    transaction_t transaction;

    /* Create a mock Block hash */
    sha256((int8_t *)"Block", strlen("Block"), block_hash);
    /*
     * Create a mock Transaction hash
     * This hash represents the transaction our mock output is from, and
     * our `owner` will receive the coins from
     */
    sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
    /*
     * In this example, `owner` will be the receiver of the transaction
     * output, and the sender of the transaction input we want to sign
     */
    owner = ec_create();
    /* Create a mock transaction output to give coins to our `owner` */
    out = tx_out_create(972, ec_to_pub(owner, pub));
    unspent = unspent_tx_out_create(block_hash, transaction_id, out);
    /*
     * The list of all unspent transaction outputs is normally located
     * in our Blockchain
     */
    all_unspent = llist_create(MT_SUPPORT_FALSE);
    llist_add_node(all_unspent, unspent, ADD_NODE_REAR);

    transaction.inputs = llist_create(MT_SUPPORT_FALSE);
    transaction.outputs = llist_create(MT_SUPPORT_FALSE);
    /* Create the transaction input from the unspent transaction output */
    in = tx_in_create(unspent);
    llist_add_node(transaction.inputs, in, ADD_NODE_REAR);
    pub[1] *= 2; /* Pretend it's a different address :) */
    out2 = tx_out_create(972, pub);
    llist_add_node(transaction.outputs, out2, ADD_NODE_REAR);

    /* Compute transaction id (hash) */
    transaction_hash(&transaction, transaction.id);
    _transaction_print(&transaction);

    /* Cleanup */
    EC_KEY_free(owner);
    free(out);
    llist_destroy(all_unspent, 1, free);
    llist_destroy(transaction.inputs, 1, free);
    llist_destroy(transaction.outputs, 1, free);
    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/transaction_hash-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c provided/_print_hex_buffer.c transaction/test/transaction_hash-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/transaction_hash-test
Transaction: {
    inputs [1]: [
        {
            block_hash: 211d0bb8cf4f5b5202c2a9b7996e483898644aa24714b1e10edd80a54ba4b560,
            tx_id: eec26ddd9a408449f8e06c622c11d61b94341b04fae5eac8d755c477b8294624,
            tx_out_hash: bc83422c65456bc7504066060d6ee8fd9e78d2905e38e7a9cd373b51cc6b0624,
            sig: null
        }
    ],
    outputs [1]: [
        {
            amount: 972,
            pub: 0498f9c5e9a3eb346dd85cd6db7c4f2c1649955a7a08ba3c143a5f9ea71c0fabfe41170f024d63c04433bb9f618af303a30c197f2d06c87a58351a8ede94427eb3,
            hash: ce033ddf7ad0649bbc374ac6007a7508b1b9ca9ede13cb24d38950aa13fc802e
        }
    ],
    id: 3ba548756992c2b553c9b6f93789796be1ba461c40b75febe43e09b9933c79ac
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/transaction_hash.c`

---
### [4. Sign transaction input]()

Write a function that signs a transaction input, given the transaction id it is from

* Prototype: `sign_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH], EC_KEY const *sender, llist_t *all_unspent);`, where:
	* `in` points to the transaction input structure to sign
	* `tx_id` contains the ID (hash) of the transaction the transaction input to sign is stored in
	* `sender` contains the private key of the receiver of the coins contained in the transaction output referenced by the transaction input
	* `all_unspent` is the list of all unspent transaction outputs to date
* Your function must verify the public key derived from the private key in `sender` matches the public key stored in the transaction output referenced by the transaction input to be signed
* Your function must sign the ID of the transaction `tx_id` the transaction input is stored in, using `sender`‘s private key
* Your function must return a pointer to the resulting signature structure upon success, or `NULL` upon failure

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/tx_in_sign-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t transaction_id[SHA256_DIGEST_LENGTH];
    tx_out_t *out, *out2;
    uint8_t pub[EC_PUB_LEN];
    EC_KEY *owner;
    llist_t *all_unspent;
    unspent_tx_out_t *unspent;
    tx_in_t *in;
    transaction_t transaction;

    /* Create a mock Block hash */
    sha256((int8_t *)"Block", strlen("Block"), block_hash);
    /*
     * Create a mock Transaction hash
     * This hash represents the transaction our mock output is from, and
     * our `owner` will receive the coins from
     */
    sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
    /*
     * In this example, `owner` will be the receiver of the transaction
     * output, and the sender of the transaction input we want to sign
     */
    owner = ec_create();
    /* Create a mock transaction output to give coins to our `owner` */
    out = tx_out_create(972, ec_to_pub(owner, pub));
    unspent = unspent_tx_out_create(block_hash, transaction_id, out);
    /*
     * The list of all unspent transaction outputs is normally located
     * in our Blockchain
     */
    all_unspent = llist_create(MT_SUPPORT_FALSE);
    llist_add_node(all_unspent, unspent, ADD_NODE_REAR);

    transaction.inputs = llist_create(MT_SUPPORT_FALSE);
    transaction.outputs = llist_create(MT_SUPPORT_FALSE);
    /* Create the transaction input from the unspent transaction output */
    in = tx_in_create(unspent);
    llist_add_node(transaction.inputs, in, ADD_NODE_REAR);
    pub[1] *= 2; /* Pretend it's a different address :) */
    out2 = tx_out_create(972, pub);
    llist_add_node(transaction.outputs, out2, ADD_NODE_REAR);

    /* Compute transaction id (hash) */
    transaction_hash(&transaction, transaction.id);
    /* Sign transaction input with transaction hash */
    tx_in_sign(in, transaction.id, owner, all_unspent);
    _transaction_print(&transaction);
    if (ec_verify(owner, transaction.id, sizeof(transaction.id), &in->sig))
        printf("Signature verified\n");
    else
        fprintf(stderr, "Signature invalid\n");

    /* Cleanup */
    EC_KEY_free(owner);
    free(out);
    llist_destroy(all_unspent, 1, free);
    free(in);
    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/tx_in_sign-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c provided/_print_hex_buffer.c transaction/test/tx_in_sign-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/tx_in_sign-test
Transaction: {
    inputs [1]: [
        {
            block_hash: 211d0bb8cf4f5b5202c2a9b7996e483898644aa24714b1e10edd80a54ba4b560,
            tx_id: eec26ddd9a408449f8e06c622c11d61b94341b04fae5eac8d755c477b8294624,
            tx_out_hash: 8b2f5895c5d6cba2d3933de04a6d7ffbb23d35d6200591b0b1c359ff8b04caa2,
            sig: 3045022100ec4b71d2a8bc2d1eecf581b8823b2ca364d6f282d0a2f28ac7ffb63403db0b6c022065f33996bfc12cff91f65e3bffa3a545ff3528e83b4d5ea5ab72a169000d52c1
        }
    ],
    outputs [1]: [
        {
            amount: 972,
            pub: 04fe703c123e9a57d267aed2b5a0a95d6d89d5a41166b05b47ac10df85fbfe05fda8bcb4cb57cef16050ba508852f2b84ea7ffa90ee5253d1bc2204a888d12dc9a,
            hash: 335b6724b77c11914da3e5442edda12d5580ffc72ce89d36c891c836c91ce386
        }
    ],
    id: 82db677e5e8d92956b4677b7f10ae261588d8cc6269bbdeef4166c7e49806398
}
Signature verified
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/tx_in_sign.c`

---

### [5. Create transaction]()

Write a function that creates a transaction

* Prototype: `transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent);`, where:
	* `sender` contains the private key of the transaction sender
	* `receiver` contains the public key of the transaction receiver
	* `amount` is the amount to send
	* `all_unspent` is the list of all the unspent outputs to date
* Your function must return a pointer to the created transaction upon success, or `NULL` upon failure
* The following steps must be implemented to create a transaction:
	* Select a set of unspent transaction outputs from `all_unspent`, which public keys match `sender`‘s private key
	* Create transaction inputs from these selected unspent transaction outputs
	* The function must fail if `sender` does not possess enough coins (total amount of selected unspent outputs lower than amount)
	* Create a transaction output, sending `amount` coins to `receiver`’s public key
	* If the total `amount` of the selected unspent outputs is greater than amount, create a second transaction output, sending the leftover back to `sender`
	* Compute transaction ID (hash)
	* Sign transaction inputs using the previously computed transaction ID

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/transaction_create-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "transaction.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t transaction_id[SHA256_DIGEST_LENGTH];
    tx_out_t *out;
    uint8_t pub[EC_PUB_LEN];
    EC_KEY *sender, *receiver;
    llist_t *all_unspent;
    unspent_tx_out_t *unspent;
    transaction_t *transaction;

    /* Create a mock Block hash */
    sha256((int8_t *)"Block", strlen("Block"), block_hash);
    /*
     * Create a mock Transaction hash
     * This hash represents the transaction our mock output is from, and
     * our `sender` will receive the coins from
     */
    sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
    /*
     * In this example, `sender` will be the receiver of the transaction
     * output, and the sender of the next transaction
     */
    sender = ec_create();
    receiver = ec_create();
    /* Create a mock transaction output to give coins to our `sender` */
    out = tx_out_create(972, ec_to_pub(sender, pub));
    unspent = unspent_tx_out_create(block_hash, transaction_id, out);
    /*
     * The list of all unspent transaction outputs is normally located
     * in our Blockchain
     */
    all_unspent = llist_create(MT_SUPPORT_FALSE);
    llist_add_node(all_unspent, unspent, ADD_NODE_REAR);

    /* Create transaction */
    transaction = transaction_create(sender, receiver, 500, all_unspent);
    _transaction_print(transaction);

    /* Cleanup */
    EC_KEY_free(sender);
    EC_KEY_free(receiver);
    free(out);
    llist_destroy(all_unspent, 1, free);
    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/transaction_create-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c provided/_print_hex_buffer.c provided/_transaction_print.c transaction/test/transaction_create-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/transaction_create-test
Transaction: {
    inputs [1]: [
        {
            block_hash: 211d0bb8cf4f5b5202c2a9b7996e483898644aa24714b1e10edd80a54ba4b560,
            tx_id: eec26ddd9a408449f8e06c622c11d61b94341b04fae5eac8d755c477b8294624,
            tx_out_hash: fcc16f27b43f8bf6c8166cf36cb69223d6bf97109e3654ad18dada037d60c44b,
            sig: 3044022049c2ce974968edc5c2e9db78869bcf05ab8699d4c1caf510662ed39a5833787502206a1484fac91b689c5aeaa4fad7dc3bc4b25fe57b5a234b7e0f1d809041137129
        }
    ],
    outputs [2]: [
        {
            amount: 500,
            pub: 045840a4f95940502e31f80d2e295fd0f82b2dd88f4085af06796ff844fde784e34e9453045aeb6cc9c5d41ca4f18dcc7dd0d78d79a1593feb1a2d1ab5b8f3a6a0,
            hash: ecb6eedd94f94b5fd6ea39f6516b7ccd3e571e401c35bc018e57bd00288a57cf
        }
        {
            amount: 472,
            pub: 040ab1c0c12feaf0c7471ae63b0b054dabf938b34c188afc0c0a5d1efb22a12387e916f34cff659a814e450a8f59cb94d2dbaf9e44d2c94da078dc95eb5c5f5b1b,
            hash: 3a2af0ac1143cb2a300417286ac958add0f174dd29961d7f185aeaff5d8a1300
        }
    ],
    id: bb96ada514c1885f5230e14d113e4ad285205cfeef182beefb21a504f2b56165
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```
### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/transaction_create.c`

---

### [6. Transaction validation]()

Write a function that checks whether a transaction is valid

* Prototype: `int transaction_is_valid(transaction_t const *transaction, llist_t *all_unspent);`, where:
	* `transaction` points to the transaction to verify
	* `all_unspent` is the list of all unspent transaction outputs to date
* Your function must return `1` if the transaction is valid, and `0` otherwise
* The transaction must verify the following:
	* The computed hash of the transaction must match the hash stored in it
	* Each input in the transaction must refer to an unspent output in `all_unspent`
	* Each input’s signature must be verified using the public key stored in the referenced unspent output
	* The total amount of inputs must match the total amount of outputs

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/transaction_is_valid-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t transaction_id[SHA256_DIGEST_LENGTH];
    tx_out_t *out, *out2;
    uint8_t pub[EC_PUB_LEN];
    EC_KEY *owner;
    llist_t *all_unspent;
    unspent_tx_out_t *unspent;
    tx_in_t *in;
    transaction_t transaction;

    /* Create a mock Block hash */
    sha256((int8_t *)"Block", strlen("Block"), block_hash);
    /*
     * Create a mock Transaction hash
     * This hash represents the transaction our mock output is from, and
     * our `owner` will receive the coins from
     */
    sha256((int8_t *)"Transaction", strlen("Transaction"), transaction_id);
    /*
     * In this example, `owner` will be the receiver of the transaction
     * output, and the sender of the transaction input we want to sign
     */
    owner = ec_create();
    /* Create a mock transaction output to give coins to our `owner` */
    out = tx_out_create(972, ec_to_pub(owner, pub));
    unspent = unspent_tx_out_create(block_hash, transaction_id, out);
    /*
     * The list of all unspent transaction outputs is normally located
     * in our Blockchain
     */
    all_unspent = llist_create(MT_SUPPORT_FALSE);
    llist_add_node(all_unspent, unspent, ADD_NODE_REAR);

    transaction.inputs = llist_create(MT_SUPPORT_FALSE);
    transaction.outputs = llist_create(MT_SUPPORT_FALSE);
    /* Create the transaction input from the unspent transaction output */
    in = tx_in_create(unspent);
    llist_add_node(transaction.inputs, in, ADD_NODE_REAR);
    pub[1] *= 2; /* Pretend it's a different address :) */
    out2 = tx_out_create(972, pub);
    llist_add_node(transaction.outputs, out2, ADD_NODE_REAR);

    /* Compute transaction id (hash) */
    transaction_hash(&transaction, transaction.id);
    /* Sign transaction input with transaction hash */
    tx_in_sign(in, transaction.id, owner, all_unspent);
    _transaction_print(&transaction);
    if (transaction_is_valid(&transaction, all_unspent))
        printf("Transaction valid\n");
    else
        fprintf(stderr, "Transaction invalid\n");

    /* Cleanup */
    EC_KEY_free(owner);
    free(out);
    llist_destroy(all_unspent, 1, free);
    free(in);
    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/transaction_is_valid-test transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_is_valid.c provided/_print_hex_buffer.c transaction/test/transaction_is_valid-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/transaction_is_valid-test
Transaction: {
    inputs [1]: [
        {
            block_hash: 211d0bb8cf4f5b5202c2a9b7996e483898644aa24714b1e10edd80a54ba4b560,
            tx_id: eec26ddd9a408449f8e06c622c11d61b94341b04fae5eac8d755c477b8294624,
            tx_out_hash: df39411b787103f992d2889ebb63c70d847681fe67a7a28841b9d42c9061ed6c,
            sig: 3046022100b5fe4cb20a751f0959d58cd4da1205f53789d7ccf394c7de97de86f73308f6df022100feb3ca6936ae3a5ebda4316d93e0512c1f8dae86d66f3cc4fb41270a6f836391
        }
    ],
    outputs [1]: [
        {
            amount: 972,
            pub: 04445adf82edc27b9bb86f0abf943bc9e1e2028376576f557e0915da9f4c68556318c088ccb6c0e736226fd336a03ecaa73cd2c3e1910498b51bc46bae65bf378d,
            hash: 3384afb12e49135b1e596d5b52e79e5de1d20689a329d2a90b32efafd3b2077b
        }
    ],
    id: 143355a0605fd1f4cb77d46d9fc7b817458f4b92f0ad4356df69fca16c87050d
}
Transaction valid
alex@~/holbertonschool-blockchain/blockchain/v0.3$

```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/transaction_is_valid.c`

---

### [7. Coinbase transaction]()

Write a function that creates a coinbase transaction

* Prototype: `transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index);`, where:
	* `receiver` contains the public key of the miner, who will receive the coinbase coins
	* `block_index` is the index of the Block the coinbase transaction will belong to
* The resulting transaction must contain a single transaction input and a single transaction output
* The transaction input should be zeroed, and not signed. To ensure this transaction input is unique, the block index will be copied in the first 4 bytes of the input’s `tx_out_hash`
* The amount in the transaction output should be exactly `COINBASE_AMOUNT`
* Your function must return a pointer to the created transaction upon success, or `NULL` upon failure

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/coinbase_create-main.c
#include <stdlib.h>

#include "transaction.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    transaction_t *coinbase;
    EC_KEY *owner;

    owner = ec_create();

    coinbase = coinbase_create(owner, 1);
    _transaction_print(coinbase);
    coinbase = coinbase_create(owner, 2);
    _transaction_print(coinbase);

    /* Cleanup */
    EC_KEY_free(owner);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/coinbase_create-test transaction/tx_out_create.c transaction/transaction_hash.c transaction/coinbase_create.c provided/_print_hex_buffer.c transaction/test/coinbase_create-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/coinbase_create-test
Transaction: {
    inputs [1]: [
        {
            block_hash: 0000000000000000000000000000000000000000000000000000000000000000,
            tx_id: 0000000000000000000000000000000000000000000000000000000000000000,
            tx_out_hash: 0100000000000000000000000000000000000000000000000000000000000000,
            sig: null
        }
    ],
    outputs [1]: [
        {
            amount: 50,
            pub: 04782f43d85a0c0f9eeca936386fa889032bc345432d2f6f3c5dd1bb5d64658870d4d6ba184d6325824a158c3194428118c00b544f998851f3df0800071bfa4611,
            hash: c6b689b36506bca5770eefd88b0234643282479a8a4e8061668a99887a61792d
        }
    ],
    id: 95fc8dd1589db1ff1f2165008ef31f9b17aa4b537c31fea78833a4ad5030b75c
}
Transaction: {
    inputs [1]: [
        {
            block_hash: 0000000000000000000000000000000000000000000000000000000000000000,
            tx_id: 0000000000000000000000000000000000000000000000000000000000000000,
            tx_out_hash: 0200000000000000000000000000000000000000000000000000000000000000,
            sig: null
        }
    ],
    outputs [1]: [
        {
            amount: 50,
            pub: 04782f43d85a0c0f9eeca936386fa889032bc345432d2f6f3c5dd1bb5d64658870d4d6ba184d6325824a158c3194428118c00b544f998851f3df0800071bfa4611,
            hash: c6b689b36506bca5770eefd88b0234643282479a8a4e8061668a99887a61792d
        }
    ],
    id: ffefbd318c6400a5f81b154bbfc5f01ee14d958ffd533bdd2426248f738a4f08
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/coinbase_create.c`

---

### [8. Coinbase transaction validation]()

Write a function that checks whether a coinbase transaction is valid

* Prototype: `int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index);`, where:
	* `coinbase` points to the coinbase transaction to verify
	* `block_index` is the index of the Block the coinbase transaction will belong to
* Your function must return `1` if the coinbase transaction is valid, and `0` otherwise
* The coinbase transaction must verify the following:
	* The computed hash of the transaction must match the hash stored in it
	* The transaction must contain exactly 1 input
	* The transaction must contain exactly 1 output
	* The transaction input’s `tx_out_hash` first 4 bytes must match the `block_index`
	* The transaction input’s `block_hash`, `tx_id`, and signature must be zeroed
	* The transaction output amount must be exactly `COINBASE_AMOUNT`

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/coinbase_is_valid-main.c
#include <stdlib.h>

#include "transaction.h"

void _transaction_print(transaction_t const *transaction);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    transaction_t *coinbase;
    EC_KEY *owner;

    owner = ec_create();

    coinbase = coinbase_create(owner, 1);
    _transaction_print(coinbase);

    if (coinbase_is_valid(coinbase, 1))
        printf("Coinbase transaction valid\n");
    else
        fprintf(stderr, "Coinbase transaction invalid\n");

    /* Cleanup */
    EC_KEY_free(owner);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/coinbase_is_valid-test transaction/tx_out_create.c transaction/transaction_hash.c transaction/coinbase_create.c transaction/coinbase_is_valid.c provided/_print_hex_buffer.c transaction/test/coinbase_is_valid-main.c provided/_transaction_print.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/coinbase_is_valid-test
Transaction: {
    inputs [1]: [
        {
            block_hash: 0000000000000000000000000000000000000000000000000000000000000000,
            tx_id: 0000000000000000000000000000000000000000000000000000000000000000,
            tx_out_hash: 0100000000000000000000000000000000000000000000000000000000000000,
            sig: null
        }
    ],
    outputs [1]: [
        {
            amount: 50,
            pub: 04b87de03521902fd914826c520729546bc7ac2db09b82480b848ef67bac4a7e0206494fffa76556e4980dbd5caeb589d1d39817b09b22c46117a5e442becc5f68,
            hash: c1810cf2c58a4cb0696ab83a7fcdf47e685c86cf4468d19dddb2d73c800cd908
        }
    ],
    id: 3a5c2ad91394a81a705277c6010da92866e4f93d4696ba39b9ea521058fc2f68
}
Coinbase transaction valid
alex@~/holbertonschool-blockchain/blockchain/v0.3$

```
### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/coinbase_is_valid.c`

---

### [9. Delete transaction]()

Write a function that deallocates a transaction structure

* Prototype: `void transaction_destroy(transaction_t *transaction);`, where:
	* `transaction` points to the transaction to delete
* Your function must free the content of the transaction and the transaction itself

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/transaction_destroy-main.c
#include <stdlib.h>

#include "transaction.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    transaction_t *coinbase;
    EC_KEY *owner;

    owner = ec_create();
    coinbase = coinbase_create(owner, 1);
    transaction_destroy(coinbase);

    /* Cleanup */
    EC_KEY_free(owner);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/transaction_destroy-test transaction/tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/coinbase_create.c transaction/transaction_destroy.c transaction/test/transaction_destroy-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ valgrind ./transaction/transaction_destroy-test
==26810== Memcheck, a memory error detector
==26810== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==26810== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==26810== Command: ./transaction/transaction_destroy-test
==26810==
==26810==
==26810== HEAP SUMMARY:
==26810==     in use at exit: 0 bytes in 0 blocks
==26810==   total heap usage: 274 allocs, 274 frees, 15,541 bytes allocated
==26810==
==26810== All heap blocks were freed -- no leaks are possible
==26810==
==26810== For counts of detected and suppressed errors, rerun with: -v
==26810== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/transaction_destroy.c`

---

### [10. Update Blockchain and Block creation/deletion]()

* Update the function `block_t *block_create(block_t const *prev, int8_t const data, uint32_t data_len);`.
	* Your function must now initializes the Block’s transaction list to an empty linked list.
*	Update the function `void block_destroy(block_t *block);`.
	* Your function must now destroy the Block’s transaction list.
### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/block_create.c, blockchain/v0.3/block_destroy.c`

---

### [11. Create/delete list of unspent]()

Update the functions `blockchain_create` and `blockchain_destroy` to respectively create and delete the linked list `unspent` of unspent transaction outputs.

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/blockchain_create.c, blockchain/v0.3/blockchain_destroy.c`

---

### [12. Update: Hash Block]()

Update the function block_hash to include the list of transactions in a Block’s hash.

* Prototype: `uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);`, where
	* `block` points to the Block to be hashed
* The resulting hash must be stored in `hash_buf`
* `block->hash` must be left unchanged.
* Your function must return a pointer to `hash_buf`
* You might need to use dynamic allocation for this task

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat test/block_hash-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *block;
    EC_KEY *owner;

    owner = ec_create();
    blockchain = blockchain_create();
    block = llist_get_head(blockchain->chain);

    block = block_create(block, (int8_t *)"Holberton", 9);
    llist_add_node(block->transactions,
        coinbase_create(owner, block->info.index),
        ADD_NODE_REAR);
    llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    _blockchain_print_brief(blockchain);

    block_hash(block, block->hash);
    _blockchain_print_brief(blockchain);

    blockchain_destroy(blockchain);
    EC_KEY_free(owner);
    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o block_hash-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c transaction/tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/coinbase_create.c transaction/transaction_destroy.c provided/_genesis.c provided/_print_hex_buffer.c provided/_blockchain_print.c provided/_transaction_print.c provided/_transaction_print_brief.c test/block_hash-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread

alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./block_hash-test
Blockchain: {
    chain [2]: [
        Block: {
            info: { 0, 0, 1537578000, 0, 0000000000000000000000000000000000000000000000000000000000000000 },
            data: { "Holberton School", 16 },
            transactions [-1]: [
            ]
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: { 1, 0, 1541634598, 0, c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 },
            data: { "Holberton", 9 },
            transactions [1]: [
                Transaction: {
                    amount: 50 from 1 inputs,
                    receiver: 040ef58ed06792647e355bcfc8e28e4aedf745dbc56a21c89ac7122211ab30c486f2709c66981e715ff0e7e0be2b55b3ee33aa229565e3721776154826197abcb1
                    id: f991378d54e91e8cbbf871a513d14005a9277c80a8137a87161b4855bc98afcf
                }
            ]
            hash: 0000000000000000000000000000000000000000000000000000000000000000
        }
    ]
}
Blockchain: {
    chain [2]: [
        Block: {
            info: { 0, 0, 1537578000, 0, 0000000000000000000000000000000000000000000000000000000000000000 },
            data: { "Holberton School", 16 },
            transactions [-1]: [
            ]
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: { 1, 0, 1541634598, 0, c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 },
            data: { "Holberton", 9 },
            transactions [1]: [
                Transaction: {
                    amount: 50 from 1 inputs,
                    receiver: 040ef58ed06792647e355bcfc8e28e4aedf745dbc56a21c89ac7122211ab30c486f2709c66981e715ff0e7e0be2b55b3ee33aa229565e3721776154826197abcb1
                    id: f991378d54e91e8cbbf871a513d14005a9277c80a8137a87161b4855bc98afcf
                }
            ]
            hash: 63b6637e4f7a9dd3b9353612879813063b18932fcc5a23b87cba65bd437f0bbe
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/block_hash.c`

---

### [13. Update: Block validation]()

Update the function block_is_valid to check that each Block’s list of transaction is valid

* Prototype: `int block_is_valid(block_t const *block, block_t const *prev_block, llist_t *all_unspent);`, where:
	* block
	* prev_block
	* all_unspent
* A block must contain **at least** one transaction (at least the coinbase transaction)
* The first transaction in a Block **must** be a valid coinbase transaction

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat test/block_is_valid-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);
void _print_hex_buffer(uint8_t const *buf, size_t len);

static block_t *_add_block(blockchain_t *blockchain, block_t const *prev,
    char const *data, EC_KEY *miner)
{
    block_t *block;
    transaction_t *coinbase;

    block = block_create(prev, (int8_t *)data, (uint32_t)strlen(data));
    block->info.difficulty = 20;

    coinbase = coinbase_create(miner, block->info.index);
    llist_add_node(block->transactions, coinbase, ADD_NODE_FRONT);

    block_mine(block);

    if (block_is_valid(block, prev, blockchain->unspent) == 0)
    {
        printf("Block mined: [%u] ", block->info.difficulty);
        _print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
        printf("\n");
        llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    }
    else
    {
        fprintf(stderr, "Invalid Block with index: %u\n",
            block->info.index);
    }

    return (block);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
        blockchain_t *blockchain;
    block_t *block;
    EC_KEY *miner;

    miner = ec_create();
    blockchain = blockchain_create();

    block = llist_get_head(blockchain->chain);
    block = _add_block(blockchain, block, "Holberton", miner);
    block = _add_block(blockchain, block, "School", miner);

    blockchain_destroy(blockchain);
    EC_KEY_free(miner);
    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o block_is_valid-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c blockchain_difficulty.c block_mine.c transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/transaction_is_valid.c transaction/coinbase_create.c transaction/coinbase_is_valid.c transaction/transaction_destroy.c provided/_genesis.c provided/_print_hex_buffer.c test/block_is_valid-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./block_is_valid-test
Block mined: [20] 000005bf0290a662435dd601106aeae812479afdb54beea471fec3506ec4ee8c
Block mined: [20] 000001080685b6e2884c4c839834d745c744875c5eeba67e1dc099af1af615e8
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/block_is_valid.c`

---

### [14. Update all unspent]()

Write a function that updates the list of all unspent transaction outputs, given a list of processed transactions

* Prototype: `llist_t *update_unspent(llist_t *transactions, uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent);`, where:
	* `transactions` is the list of validated transactions
	* `block_hash` Hash of the validated Block that contains the transaction list transactions
	* `all_unspent` is the current list of unspent transaction outputs
* Your function must create and return a new list of unspent transaction outputs
* All transaction inputs from each transaction in `transactions` should not be included in the returned list of unspent transaction outputs
* All transaction outputs from each transaction in `transactions` should be appended in the returned list of unspent transaction outputs
* The list `all_unspent` must be deleted upon success

```c
alex@~/holbertonschool-blockchain/blockchain/v0.3$ cat transaction/test/update_unspent-main.c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "blockchain.h"

void _blockchain_print_brief(blockchain_t const *blockchain);
void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * _print_unspent - Print an unspent transaction output
 *
 * @unspent: Pointer to the unspent transaction output to print
 * @idx:     Unused
 * @indent:  Indentation
 *
 * Return: 0
 */
static int _print_unspent(unspent_tx_out_t const *unspent, unsigned int idx,
    char const *indent)
{
    printf("%s{\n", indent);

    printf("%s\tblock_hash: ", indent);
    _print_hex_buffer(unspent->block_hash, SHA256_DIGEST_LENGTH);
    printf("\n");

    printf("%s\ttx_id: ", indent);
    _print_hex_buffer(unspent->tx_id, SHA256_DIGEST_LENGTH);
    printf("\n");

    printf("%s\tamount: %u\n", indent, unspent->out.amount);

    printf("%s\tpub: ", indent);
    _print_hex_buffer(unspent->out.pub, EC_PUB_LEN);
    printf("\n");

    printf("%s}\n", indent);

    (void)idx;
    return (0);
}

/**
 * _print_all_unspent - Print a list of unspent transaction outputs
 *
 * @unspent: List of unspent transaction outputs to print
 */
static void _print_all_unspent(llist_t *unspent)
{
    printf("Unspent transaction outputs [%u]: [\n", llist_size(unspent));

    llist_for_each(unspent, (node_func_t)_print_unspent, "\t");

    printf("]\n");
}

/**
 * _add_block - Add a block to a blockchain
 *
 * @blockchain: Pointer to the Blockchain to add the Block to
 * @prev:       Pointer to the previous Block in the chain
 * @data:       Data buffer to be put in the Block
 * @miner:      EC key of the miner (receiver of the coinbase transaction)
 *
 * Return: A pointer to the created Block
 */
static block_t *_add_block(blockchain_t *blockchain, block_t const *prev,
    char const *data, EC_KEY *miner)
{
    block_t *block;
    transaction_t *coinbase;

    block = block_create(prev, (int8_t *)data, (uint32_t)strlen(data));
    block->info.difficulty = 20;

    coinbase = coinbase_create(miner, block->info.index);
    llist_add_node(block->transactions, coinbase, ADD_NODE_FRONT);

    block_mine(block);

    if (block_is_valid(block, prev, blockchain->unspent) == 0)
    {
        printf("Block mined: [%u] ", block->info.difficulty);
        _print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
        printf("\n");

        /* Update all unspent */
        blockchain->unspent = update_unspent(block->transactions,
            block->hash, blockchain->unspent);

        llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
    }
    else
    {
        fprintf(stderr, "Invalid Block with index: %u\n",
            block->info.index);
    }

    return (block);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    blockchain_t *blockchain;
    block_t *block;
    EC_KEY *miner;

    miner = ec_create();
    blockchain = blockchain_create();

    block = llist_get_head(blockchain->chain);
    block = _add_block(blockchain, block, "Holberton", miner);
    _print_all_unspent(blockchain->unspent);
    block = _add_block(blockchain, block, "School", miner);
    _print_all_unspent(blockchain->unspent);

    _blockchain_print_brief(blockchain);

    blockchain_destroy(blockchain);
    EC_KEY_free(miner);

    return (EXIT_SUCCESS);
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$ gcc -Wall -Wextra -Werror -pedantic -g3 -I. -Itransaction/ -Iprovided/ -I../../crypto -o transaction/update_unspent-test blockchain_create.c block_create.c block_destroy.c blockchain_destroy.c block_hash.c block_is_valid.c hash_matches_difficulty.c blockchain_difficulty.c block_mine.c transaction/tx_out_create.c transaction/unspent_tx_out_create.c transaction/tx_in_create.c transaction/transaction_hash.c transaction/tx_in_sign.c transaction/transaction_create.c transaction/transaction_is_valid.c transaction/coinbase_create.c transaction/coinbase_is_valid.c transaction/transaction_destroy.c transaction/update_unspent.c provided/_genesis.c provided/_print_hex_buffer.c provided/_blockchain_print.c provided/_transaction_print.c provided/_transaction_print_brief.c transaction/test/update_unspent-main.c -L../../crypto -lhblk_crypto -lllist -lssl -lcrypto -pthread
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ./transaction/update_unspent-test
Block mined: [20] 00000a70b74dc303a9d9127d928afdd702cf79f8bfcd6388ada9f6a2738fa0d5
Unspent transaction outputs [1]: [
    {
        block_hash: 00000a70b74dc303a9d9127d928afdd702cf79f8bfcd6388ada9f6a2738fa0d5
        tx_id: add25b33e032d37d9ab20c1e8d98bcdafb7f02ef2d3a77b3c314486684addd2e
        amount: 50
        pub: 045d18d1d8460371a0714c4c35293db8bbd289acdbb79888d69546602ba33ac1ffb16ecba62bbdf48610b0debd3bf104df147da2b37fa7ab83584930675404906f
    }
]
Block mined: [20] 000002e52b2f4a1e230a9f044870a673419ce0c4c52788ed5d93de3da869b29b
Unspent transaction outputs [2]: [
    {
        block_hash: 00000a70b74dc303a9d9127d928afdd702cf79f8bfcd6388ada9f6a2738fa0d5
        tx_id: add25b33e032d37d9ab20c1e8d98bcdafb7f02ef2d3a77b3c314486684addd2e
        amount: 50
        pub: 045d18d1d8460371a0714c4c35293db8bbd289acdbb79888d69546602ba33ac1ffb16ecba62bbdf48610b0debd3bf104df147da2b37fa7ab83584930675404906f
    }
    {
        block_hash: 000002e52b2f4a1e230a9f044870a673419ce0c4c52788ed5d93de3da869b29b
        tx_id: 5c08ced6d72ae27186017352a86adbb44d938e985706f8035c723175dc4a937d
        amount: 50
        pub: 045d18d1d8460371a0714c4c35293db8bbd289acdbb79888d69546602ba33ac1ffb16ecba62bbdf48610b0debd3bf104df147da2b37fa7ab83584930675404906f
    }
]
Blockchain: {
    chain [3]: [
        Block: {
            info: { 0, 0, 1537578000, 0, 0000000000000000000000000000000000000000000000000000000000000000 },
            data: { "Holberton School", 16 },
            transactions [-1]: [
            ]
            hash: c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803
        }
        Block: {
            info: { 1, 20, 1542620387, 1564848, c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 },
            data: { "Holberton", 9 },
            transactions [1]: [
                Transaction: {
                    amount: 50 from 0 inputs,
                    receiver: 045d18d1d8460371a0714c4c35293db8bbd289acdbb79888d69546602ba33ac1ffb16ecba62bbdf48610b0debd3bf104df147da2b37fa7ab83584930675404906f
                    id: add25b33e032d37d9ab20c1e8d98bcdafb7f02ef2d3a77b3c314486684addd2e
                }
            ]
            hash: 00000a70b74dc303a9d9127d928afdd702cf79f8bfcd6388ada9f6a2738fa0d5
        }
        Block: {
            info: { 2, 20, 1542620388, 1263699, 00000a70b74dc303a9d9127d928afdd702cf79f8bfcd6388ada9f6a2738fa0d5 },
            data: { "School", 6 },
            transactions [1]: [
                Transaction: {
                    amount: 50 from 0 inputs,
                    receiver: 045d18d1d8460371a0714c4c35293db8bbd289acdbb79888d69546602ba33ac1ffb16ecba62bbdf48610b0debd3bf104df147da2b37fa7ab83584930675404906f
                    id: 5c08ced6d72ae27186017352a86adbb44d938e985706f8035c723175dc4a937d
                }
            ]
            hash: 000002e52b2f4a1e230a9f044870a673419ce0c4c52788ed5d93de3da869b29b
        }
    ]
}
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/transaction/update_unspent.c`

---

### [15. Update: Blockchain serialization & deserialization]()

Update the functions `blockchain_serialize` and `blockchain_deserialize`, to serialize each Block’s transactions list.

**blockchain_serialize**

* Prototype: `int blockchain_serialize(blockchain_t const *blockchain, char const *path);`, where:
	* `blockchain` points to the Blockchain to be serialized,
	* `path` contains the path to a file to serialize the Blockchain into
* If `path` points to an existing file, it must be overwritten
* Your function must return `0` upon success, or `-1` upon failure

**blockchain_deserialize**

* Prototype: `blockchain_t *blockchain_deserialize(char const *path);`, where:
	* `path` contains the path to a file to deserialize the Blockchain from
* Your function must return the deserialized Blockchain upon success, or NULL upon failure

**Format**

The resulting file should contain exactly the following:

**File Header**

The following table describes the file header

| **Offset** |  **Size(bytes)** | **Field**         | **Purpose**                                                                   |
|--------|--------------|---------------|---------------------------------------------------------------------------|
| 0x00   | 4            | hblk_magic    | Identifies the file as a valid serialized Blockchain format               |
|        |              |               | `HBLK (48 42 4c 4b)` in ASCII; these four bytes constitute the magic number |
| 0x04   | 3            | hblk_version  | Identifies the version at which the Blockchain has been serialized        |
|        |              |               | The format is `X.Y`, where both `X` and `Y` are ASCII characters between 0 and 9 |
| 0x07   | 1            | hblk_endian   | This byte is set to either `1` or `2` to signify little or big endianness, respectively |
|        |              |               | This affects interpretation of multi-byte fields                         |
| 0x08   | 4            | hblk_blocks   | Number of blocks in the Blockchain                                       |
|        |              |               | Endianness dependent                                                      |
| 0x0C   | 4            | hblk_unspent  | Number of unspent transaction outputs in the Blockchain                   |
|        |              |               | Endianness dependent                                                      |
| 0x10   | ?            | blocks        | List of Blocks                                                            |
| 0x10 + ? | 165 * hblk_unspent | unspent | List of unspent transaction outputs                                       |

**Block**

The following table describes how a block is serialized. The Blocks are serialized contiguously, the first one starting at offset `0x10`

| **Offset**          | **Size (bytes)** | **Field**           | **Purpose**                                                   |
|-----------------|--------------|-----------------|-----------------------------------------------------------|
| 0x00            | 4            | index           | Block index in the Blockchain                             |
|                 |              |                 | Endianness dependent                                      |
| 0x04            | 4            | difficulty      | Block difficulty                                          |
|                 |              |                 | Endianness dependent                                      |
| 0x08            | 8            | timestamp       | Time at which the block was created (UNIX timestamp)      |
|                 |              |                 | Endianness dependent                                      |
| 0x10            | 8            | nonce           | Salt value used to alter the Block hash                   |
|                 |              |                 | Endianness dependent                                      |
| 0x18            | 32           | prev_hash       | Hash of the previous Block in the Blockchain              |
| 0x38            | 4            | `data_len`        | Block data size (in bytes)                                |
|                 |              |                 | Endianness dependent                                      |
| 0x3C            | `data_len`     | data            | Block data                                                |
| 0x3C + `data_len` | 32           | hash            | Block hash                                                |
| 0x5C + `data_len` | 4            | nb_transactions | Number of transactions in the Block                       |
|                 |              |                 | Endianness dependent                                      |
|                 |              |                 | -1 is for NULL (e.g. Genesis Block), 0 is for empty list   |
| 0x60 + data_len | ?            | transactions    | List of transactions                                      |


**Transaction**

The following table describes how a transaction is serialized. The transactions of a Block are serialized contiguously, the first one starting at offset 0x60 + data_len

| **Offset**                   | **Size (bytes)** | **Field**        | **Purpose**                                                    |
|--------------------------|--------------|--------------|------------------------------------------------------------|
| 0x00                     | 32           | id           | Transaction ID (hash)                                      |
| 0x20                     | 4            | nb_inputs    | Number of transaction inputs in the Transaction           |
|                          |              |              | Endianness dependent                                       |
| 0x24                     | 4            | nb_outputs   | Number of transaction outputs in the Transaction          |
|                          |              |              | Endianness dependent                                       |
| 0x28                     | 169 * `nb_inputs` | inputs    | List of transaction inputs                                 |
| 0x28 + 169 * `nb_inputs`   | 101 * `nb_outputs` | outputs  | List of transaction outputs                                |


**Transaction input**

The following table describes how a transaction input is serialized. The transaction inputs of a Transaction are serialized contiguously, the first one starting at offset 0x28. Each transaction input is stored on exactly 169 bytes.

| **Offset** | **Size (bytes)** | **Field**       | **Purpose**                                                                  |
|--------|--------------|-------------|--------------------------------------------------------------------------|
| 0x00   | 32           | block_hash  | Hash of the Block in which the consumed transaction output is located    |
| 0x20   | 32           | tx_id       | ID (hash) of the transaction in which the consumed transaction output is located |
| 0x40   | 32           | tx_out_hash | Hash of the consumed transaction output                                  |
| 0x60   | 72           | sig.sig     | Transaction input signature buffer                                      |
| 0xA8   | 1            | sig.len     | Transaction input signature length                                      |


**Transaction output**

The following table describes how a transaction output is serialized. The transaction outputs of a Transaction are serialized contiguously, the first one starting after the last transaction input. Each transaction output is stored on exactly 101 bytes.

| **Offset** | **Size (bytes)** | **Field**   | **Purpose**                                     |
|--------|--------------|---------|---------------------------------------------|
| 0x00   | 4            | amount  | Transaction output amount                   |
|        |              |         | Endianness dependent                        |
| 0x04   | 65           | pub     | Receiver’s public key                       |
| 0x45   | 32           | hash    | Transaction output hash                     |


**Unspent transaction output**

The following table describes how an unspent transaction output is serialized. The unspent transaction outputs are serialized contiguously, the first one starting right after the last Block serialized. Each serialized unspent transaction output is stored on exactly 165 bytes.

| **Offset** | **Size (bytes)** | **Field**         | **Purpose**                                                              |
|--------|--------------|---------------|----------------------------------------------------------------------|
| 0x00   | 32           | block_hash    | Hash of the Block the referenced transaction output belongs to       |
| 0x20   | 32           | tx_id         | ID of the Transaction the referenced transaction output belongs to   |
| 0x40   | 4            | out.amount    | Amount of the referenced transaction output                          |
|        |              |               | Endianness dependent                                                 |
| 0x44   | 65           | out.pub       | Receiver’s public key of the referenced transaction output           |
| 0x85   | 32           | out.hash      | Hash of the referenced transaction output                            |


### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/blockchain_serialize.c, blockchain/v0.3/blockchain_deserialize.c`

---

### [16. Blockchain library]()

Write a Makefile that compiles all the previous functions and archives them into a static library for future use.

The library must be called `libhblk_blockchain.a`, and your Makefile must define a rule for this file.

```bash
alex@~/holbertonschool-blockchain/blockchain/v0.3$ make libhblk_blockchain.a
[...]
alex@~/holbertonschool-blockchain/blockchain/v0.3$ ar -t libhblk_blockchain.a
blockchain_create.o
block_create.o
block_destroy.o
blockchain_destroy.o
block_hash.o
blockchain_serialize.o
blockchain_deserialize.o
block_is_valid.o
hash_matches_difficulty.o
blockchain_difficulty.o
block_mine.o
tx_out_create.o
unspent_tx_out_create.o
tx_in_create.o
transaction_hash.o
tx_in_sign.o
transaction_create.o
transaction_is_valid.o
coinbase_create.o
coinbase_is_valid.o
transaction_destroy.o
update_unspent.o
alex@~/holbertonschool-blockchain/blockchain/v0.3$ nm --defined-only libhblk_blockchain.a | grep T | cut -d ' ' -f3
blockchain_create
block_create
block_destroy
blockchain_destroy
block_hash
block_hash_nonce
blockchain_serialize
blockchain_deserialize
block_is_valid
hash_matches_difficulty
blockchain_difficulty
block_mine
tx_out_create
unspent_tx_out_create
tx_in_create
transaction_hash
tx_in_sign
transaction_create
transaction_is_valid
coinbase_create
coinbase_is_valid
transaction_destroy
update_unspent
alex@~/holbertonschool-blockchain/blockchain/v0.3$
```

### Repo:

* GitHub repository: `holbertonschool-blockchain`
* File: `blockchain/v0.3/Makefile`
