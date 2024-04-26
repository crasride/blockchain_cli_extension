<h1 align="center"> Blockchain - CLI </h1>

### Learning Objectives

### General
* How to interact with the Blockchain library you previously created
* How to maintain a transaction pool

### Requirements
### General
* Allowed editors: `vi, vim, emacs`
* All your files will be compiled on **Ubuntu 14.04 LTS**
* Your C programs and functions will be compiled with `gcc 4.8.4` using the flags `-Wall -Werror -Wextra, -pedantic` and the linker flags `-lssl` and `-lcrypto`
* Your library `libhblk_crypto.a`, located in the `crypto` folder will be built and linked during correction
* Your library `libhblk_blockchain.a`, located in the `blockchain/v0.3` folder will be built and linked during correction
* All your files should end with a new line
* A **README.md** file, at the root of the folder of the project, is mandatory
* Your code should use the `Betty` style. It will be checked using [betty-style.pl](https://github.com/hs-hq/Betty/blob/main/betty-style.pl) and [betty-doc.pl](https://github.com/hs-hq/Betty/blob/main/betty-doc.pl)
* The prototypes of all your functions should be included in your header file called `blockchain/v0.2/blockchain.h`
* All your header files should be include guarded

### Folder & Compilation
The sources of your CLI must be placed in the folder `cli`, at the root of your repository. This folder should contain a `Makefile`, that creates the program `cli`.
You are free to use any library call of your choice.
You are free to design and arrange your code the way you like, as long as it is Betty-compliant.

### Program
* The `CLI` is a really simple command interpreter, that will allow a user to interact with your Blockchain API
* The goal is to implement a simple command line interface, in C
* You are free to design your CLI the way you like
* You are free to output the information in the format of your choice, as long as the requirements are fulfilled


### Tasks
### [0. CLI - Create/Load Wallet]()

At startup, your CLI must either create a wallet (EC key pair), or load it from a folder.

Implement the command `wallet_load`

* Usage: `wallet_load <path>,` where:
	* `<path>` is the path to the folder from which to load the key pair (string)
* Description: Load wallet (EC key pair)

### Prerequisites

* The program compile when running `make`
* Launch the CLI. At least a prompt is displayed, allowing you to enter a command.
* When the CLI is launched, a wallet is either loaded or created


### Send command

* Try and type the command `wallet_load <path>.`
	* The wallet should be loaded

---

### [1. CLI - Save Wallet]()

Implement the command `wallet_save`

* Usage: `wallet_save <path>`, where:
	* `<path>` is the path to the folder in which to save the key pair (string)
* Description: Save wallet (EC key pair)


### Send command
* Try and type the command `wallet_save <path>`

	* the current wallet must be serialized at the given path
	* If the folder already exists, it must be overridden
	* Keys must be stored in the PEM format

---
### [2. CLI - send]()

Implement the command `send`

* Usage: `send <amount> <address>`, where:
	* `<amount>`is the number of coins to send (Integer)
	* `<address>`is the EC public key of the receiver
* Description: Send coins
	* Create a new transaction
	* Verify the transaction validity
	* Add transaction to a local list (transaction pool)
	* Do not update list of unspent

### Prerequisites

* The program compile when running `make`
* Launch the CLI. At least a prompt is displayed, allowing you to enter a command.

### Send command

* Try and type the command `send 10 04d2335bcb8e3a1013ff18648b55a939dcec35ea6cb8a497089d73d5a79b069f74255d4442032b61cf328d0da7fcf8822380f7876b09ffc2002f5957a619ceb0c4`

	* A transaction should be successfully created (1 point)
	* the transaction must be valid and the inputs must be signed (1 point)
	* The transaction must be added to a local transaction pool (1 point)

---

### [3. CLI - mine]()

Implement the command `mine`

* Usage: `mine`
* Description: Mine a block
	* Create a new Block using the Blockchain API
	* **IF** transactions are available in the local transaction pool, include the transactions **1 by 1** in the Block
		* Verify the transaction using the list of unspent outputs.
		* If the transaction is not valid, do not include it in the Block, and delete it
		* Update the list of unspent outputs after a transaction is processed
	* Set the difficulty of the Block using the difficulty adjustment method
	* Inject a coinbase transaction **as the first** transaction in the Block
	* Mine the Block (proof of work)
	* Verify Block validity
	* Add the Block to the Blockchain


### Mine command

* Try and type the command mine (without having any transaction in the local transaction pool)

	* A Block containing only a coinbase transaction should be mined and added to the Blockchain (2 points)

* Add a single transaction in the local transaction pool using the `send` command. Try and type the command `mine`

	* A Block containing a coinbase transaction AND the transaction in the pool should be mined and added to the Blockchain (2 points)
	
	* The transaction pool should now be empty (1 point)

* Add several transactions in the local transaction pool using the `send` command. Try and type the command `mine`

	* A Block containing a coinbase transaction AND the transactions in the pool should be mined and added to the Blockchain (3 points)
	* The transaction pool should now be empty (1 point

---

### [4. CLI - info]()

Implement the command `info`

* Usage: `info`
* Description: Display information about the Blockchain, at least the following:
	* Display the number of Blocks in the Blockchain
	* Display the number of unspent transaction output
	* Display the number of pending transactions in the local transaction pool


### Info command

* Try and type the command `info`


	* The number of Blocks in the Blockchain, or the content of the Blockchain is displayed (1 point)
	* The number of unspent transaction outputs, or the list of unspent transaction outputs is displayed (1 point)
	* The number of transactions, or the list of transactions in the local transaction pool is displayed (1 point)


* Add transactions to the local transaction pool using the `send` command. Try and type the command `info`

	* The number of Blocks in the Blockchain, or the content of the Blockchain is displayed (1 point)
	* The number of unspent transaction outputs, or the list of unspent transaction outputs is displayed (1 point)
	* The number of transactions, or the list of transactions in the local transaction pool is displayed (1 point)


* Mine one or several Blocks using the `mine` command. Add transactions to the local transaction pool using the `send` command. Try and type the command `info`

	* The number of Blocks in the Blockchain, or the content of the Blockchain is displayed (1 point)
	* The number of unspent transaction outputs, or the list of unspent transaction outputs is displayed (1 point)
	* The number of transactions, or the list of transactions in the local transaction pool is displayed (1 point)

---

### [5. CLI - load]()

Implement the command `load`

* Usage: `load <path>`
* Description: Load a Blockchain from a file
	* Override the local Blockchain


### Load command

* Try and type the command `load <path>` with `<path>` pointing to a valid serialized Blockchain. Then run the command `info`

	* The Blockchain should be correctly loaded (3 points)


* Try and type the command `load <path>` with `<path>` pointing to a invalid serialized Blockchain (file does not exist, or invalid file format)

	* An error message should be displayed, and no Blockchain should be loaded (1 point)

---

### [6. CLI - save]()

implement the command `save`

* Usage: `save <path>`
* Description: Save the local Blockchain into a file
	* If the file exists, override it

### Save command

* After mining one or several Blocks using the `send` and the `mine` commands, try and run `save ./save.hblk`

	* The local Blockchain must be correctly saved in the file `save.hblk` (3 points)
