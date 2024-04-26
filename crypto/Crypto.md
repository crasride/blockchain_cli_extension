<h1 align="center">[EYNTK] Blockchain - Crypto</h1>

# Introduction
The purpose of this concept page is to put the cryptographic aspect in the context of a Blockchain.

There are different aspects of cryptography that we are going to see, and try to understand their puprose, and why they are used in the industry.

## SHA - Secure Hash Algorithm
If you are familiar with **Hash tables**, you remember that a [Hash function](https://en.wikipedia.org/wiki/Hash_function) “is any function that can be used to map data of arbitrary size to data of a fixed size.” A hash algorithm computes a digest of a given piece of data.

Hash algorithms are used to serve many purposes, they are used to store obfuscated passwords, to verify the authenticity of a file, etc.

The main advantage of a hash algorithm such as *SHA2* is that they are collision-resistant, which means that it is very hard to find data that will generate the same hash value. This is why we are going to need SHA2 in our Blockchain project.

As explained in the [Blockchain Introduction](), each block in the Blockchain stores a reference to the previous Block in the chain. This reference will be a hash of the previous block. This guarantees the authenticity of the chain. If a single bit of a block changes, its hash will no longer match the hash in the next Block. On top of that, since SHA2 is collision-resistant, it will be very difficult (if not practically impossible) for someone to generate a collision, and to match the “reference hash”.

The second main reason we use a hash is to save space. Imagine we have a Block which total size is 2MB. Its hash (if we use SHA256) will fit on 256 bits. So each Block, instead of storing the entire previous Block to guarantee its authenticity, will store a **digest** of it (a.k.a fingerprint), to save space.

Now, why **SHA256** over another? The reason is simple: the SHA algorithms family is published by the **NIST** **(National Institute of Standards and Technology)**, which makes it the standard in the Industry. SHA1 is now considered broken (meaning generating collisions is technically possible). SHA256 is part of the SHA2 family, which as of today, has not been broken.

The following video explains the hash algorithms:

[![SHA Video](https://img.youtube.com/vi/b4b8ktEV4Bg/0.jpg)](https://www.youtube.com/watch?v=b4b8ktEV4Bg)


## ECDSA - Elliptic Curve Digital Signature Algorithm
The second cryptography algorithm we are going to use during our Blockchain project is the **ECDSA**. ECDSA is an algorithm used for digital signatures, and uses Elliptic Curve Cryptography. **ECC** is an approach to public-key cryptography based on elliptic curves.

Digital signatures are a key part of any cryptocurrency: Any transaction must be signed by the sender’s private (secret) key to guarantee this person actually made the transaction.

Let’s break this down, and see how it works …

## ECC - Asymmetric vs. Symmetric cryptography
Public-key cryptography is another term for **asymmetric cryptography**. This term refers to a cryptographic system that uses pairs of keys. In such pair of keys, one is **public**, and one is **private**. A very known example of asymmetric algorithm is `RSA`.

The following video explains the difference between symmetric and asymmetric encryption, and describes how asymmetric encryption solves[the key exchange problem](https://en.wikipedia.org/wiki/Key_exchange#The_key_exchange_problem):


[![Asymmetric Encryption Video](https://img.youtube.com/vi/AQDCe585Lnc/0.jpg)](https://www.youtube.com/watch?v=AQDCe585Lnc)


## ECC - Explanation
The following video from **Computerphile** explains the trapdoor algorithm in Elliptic Curves, and how it is size efficient:

[![ECC Trapdoor Algorithm Video](https://img.youtube.com/vi/NF1pwjL9-DE/0.jpg)](https://www.youtube.com/watch?v=NF1pwjL9-DE)

Another video that dives more in the Maths behind Elliptic Curves, and compares them to RSA. You are not required to understand the maths behind Elliptic Curves, but it is always good to have an idea of how things work behind the curtain:

[![ECC Maths Video](https://img.youtube.com/vi/dCvB-mhkT0w/0.jpg)](https://www.youtube.com/watch?v=dCvB-mhkT0w)

## Digital Signature
We are going to use Elliptic Curves to generate digital signatures. The algorithm we are going to use is the **ECDSA**. The following video explains how a digital signature algorithm ensures data authenticity:


[![Digital Signature Video](https://img.youtube.com/vi/Aq3a-_O2NcI/0.jpg)](https://www.youtube.com/watch?v=Aq3a-_O2NcI&t=1s)

## Data structures

Here are the macros and the C structure we are going to use in our project to store a digital signature:

```
# include <openssl/sha.h>
# include <stdint.h>

# define EC_CURVE   NID_secp256k1

/* EC_KEY public key octet string length (using 256-bit curve) */
# define EC_PUB_LEN 65
/* Maximum signature octet string length (using 256-bit curve) */
# define SIG_MAX_LEN    72

# define PRI_FILENAME   "key.pem"
# define PUB_FILENAME   "key_pub.pem"

/**
 * struct sig_s - EC Signature structure
 *
 * @sig: Signature buffer. The whole space may not be used
 * @len: Actual signature size. Can't exceed SIG_MAX_LEN, therefore stored on a byte
 */
typedef struct sig_s
{
    /*
     * @sig must stay first, so we can directly use the structure as
     * an array of char
     */
    uint8_t     sig[SIG_MAX_LEN];
    uint8_t     len;
} sign_t;
```

It is important to include `openssl/sha.h`, as it defines the macro **SHA256_DIGEST_LENGTH** that we are going to use in this project.

**EC_CURVE** is the curve we are going to use for our public/private keys using ECDSA.

**EC_PUB_LEN** and **SIG_MAX_LEN** depends on the curve you use. We are using a 256-bit EC curve, so a public key length will be `256 / 8 * 2 + 1 -> 65`. The size of a signature is not fixed, but it has a maximum size of `72`. Therefore, we are going to store it in a buffer, and keep track of the actual signature length in a structure.

You can learn more about how these values are calculated [here](https://www.secg.org/sec1-v2.pdf)

## stdint.h
During this project, we are going to use the `stdint.h` typedefs to declare most of our basic data types. For example, instead of declaring an `int`, we will prefer to declare an `int32_t`. This is to maintain the same size depending on the system architecture our code will be executed on. the `stdint.h` defines the different types, so it will always be the same size, no matter the architecture.

This is going to be very important for compatibility of our Blockchain. If for example we want a Block’s timstamp to be stored on 32 bits when serialized or exchanged through a socket, it makes it very simple and reliable to use an `uint32_t` instead of an `unsigned int`, the size of the latter depending on the architecture.


