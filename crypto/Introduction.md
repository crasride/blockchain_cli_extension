<h1 align="center">[EYNTK] Blockchain - Introduction</h1>


# Everything You Need To Know to start the Blockchain track

Welcome to the `Blockchain` track ! \o/

The goal of this series of projects is to build a basic `Blockchain/Cryptocurrency` entirely **in C, and from scratch!** It’s going to be a lot of work, and we’re going to go through this step by step.

## Where do we start?
Before even breaking down the data structure that we are going to use to build our Blockchain, we need to understand what a Blockchain is, and why it is considered to be “unbreakable”.

<p align="center">
<img width="" height="" src="./media/2_files/1.jpg">
</p>


## Blockchain - the big picture

From a really high-level perspective, a blockchain is simply a list of blocks, in which each block contains data, and a reference to the previous one in the list.

The data contained in a block can be anything (in the case of a cryptocurrency, the data stored in a block is usually a list of transactions, but we’ll save this topic for later …).

Now what makes the Blockchain so secure? Well, first, let’s put it on the table right now, a **Blockchain is not unbreakable.** It is **really** hard to hack or corrupt though, and we will see how.

## Why is a blockchain so secure?

Let’s put oursleves in the shoes of a hacker, willing to corrupt a blockchain, by altering its content.

<p align="left">
<img width="300" height="250" src="./media/2_files/100.gif">
</p>

What we want to do is simple: take a block and change the data stored in it.

Here comes the first layer of security. The data of each block in a blockchain is hashed using a cryptographic hash algorithm (e.g. SHA), and this hash is stored in the block. This guaranties the authenticity of a block. If any bit in the block changes, its new hash will be drasticly different from the one stored in the block, which makes the block invalid.

Alright, easy, once we altered the data in the Block, we can recompute its hash, and store the new one at the place of the old hash, so nobody will know the block was changed!

Hmmmm … nope. As said earlier, each block stores a reference to the previous block in the chain. this reference is the hash of the previous block. Meaning that if a block is altererd, its new hash and the reference to this block in the next block will no longer match.

A couple lines of code, and we can re-hash every block in the chain, to make it valid again, right?

Well, here comes the most “unbreakable” part of a blockchain: it’s distributed. Anyone can claim a copy of a blockchain, and when someone wants to add a new block, at least 50% of the network has to agree. Now I think you get it, if you alter your copy of the blockchain, it will no longer match with anyone’s blockchain on the network, so you’re screwed … Unless you possess more than 50% of the computing power of the network, which in our case, for our little project, is doable, but for BitCoin for example, it is just unimaginable.

Again, this is a very high-level perspective of how a Blockchain guaranties data authenticity. More aspects of the Blockchain guaranty its security, and we will see them later :)

As a reminder, a blockchain **doesn’t guaranty data security**, it’s the complete opposite. A blockchain is meant to be distributed/public. It guaranties data authenticity, meaning that the data stored in a Blockchain is in theory immutable, and thus authentic. So you don’t want to store credentials in there, but instead, certifications, contracts, etc.

### So … where do we start?

As described earlier, a Blockchain is called `immutable`. The information, cannot be altered or removed. What guaranties this immutability comes from cryptography. All the information in a Blockchain is `hashed`.

That’s where we will start with our series of project.

### From scratch … almost.

The first part of this series of project is to get familiar with the cryptographic aspect of a Blockchain. We are going to use the `OpenSSL` library, that will provide us with the different cryptographic algorithms needed to build our Blockchain.

The `OpenSSL` is **one** implementation of the SSL protocol. It is probably the most widely used implementation in today’s softwares, and its API is really low level. That sounds perfect for us! :)

The `OpenSSL` API, I have to admit, is not the best documented I’ve ever seen. Working with it will require a lot of manual pages reading and experimentation. The following link will take you to the complete documentation of the API. You are going to use it a lot, so I suggest you just leave it open …

[The OpenSSL manual pages](https://www.openssl.org/docs/manmaster/man3/index.html)

Good luck, and have fun with this series of projects ! \o/

