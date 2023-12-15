In this guide, we will provide examples of using the high-level API, with less emphasis on
the low-level implementation, since XKCP abstracts the low-level implementation from the user.

Before proceeding with the usage example, please make sure that you have built the XKCP library as described in the [README](../README.md).
and included it in your C/C++ project.

# Hashing and Extendable output functions (XOFs)

## FIPS 202

Those are the NIST approved SHA-3 functions. They are based on the `Sponge construction`, and the `Keccak-p[1600, 24]` permutation.
The functions are:

### Hash functions

A hash function is a function on binary data (i.e., bit strings) for which the length of the output is fixed. <br>
The input to a hash function is called the _message_, and the output is called the _digest_ or _hash value_.

NIST standardized the following hash functions in FIPS 202:

- `SHA3_224`
- `SHA3_256`
- `SHA3_384`
- `SHA3_512`

The suffix `224`, `256`, `384`, and `512` indicate the fixed length of the `digest` in bits.

### Extendable output functions (XOFs)

A XOF is a function on bit strings, also called _messages_, in which the output can be extended to any desired length. <br>

NIST standardized the following XOFs in FIPS 202, marking them as the first XOFs to be standardized by NIST.

- `SHAKE128`
- `SHAKE256`

The suffix `128` and `256` indicates the desired security level of the function.

### Usage

To use any of them in your C/C++ project, you have to first build the FIPS202 library, and include it in your project.
The following steps illustrate how to do that:

#### Example of using the `SHA3_256` hash function

<details open>
    <summary>Simple usage</summary>

```c
#include "SimpleFIPS202.h"

int main() {
   // your input message
   const unsigned char *input =
           (const unsigned char *) "The random message to hash";

   int outputByteLen = 32;
   unsigned char output[outputByteLen];

   int result = SHA3_256(output, input, strlen((const char *) input));

   // returning 0 means success
   assert(result == 0);

   // printing the hash in hexadecimal format
   for (int i = 0; i < outputByteLen; i++)
       printf("\\x%02x", output[i]);
   printf("\n");

   // ...
}
```

</details>

<details open>
    <summary>Advanced: Chunked input</summary>
    Sometimes, the input of your function is too long to be stored in memory and passed to the function at once, 
    think of a big file for example. In such cases, you can feed the input as chunks to the hash function, and at the end, 
    get the output at once or in chunks as well (We'll show an example of that later, with the SHAKE128 XOF).
    
   ```c
    #include "KeccakHash.h"

    int main() {
        const int inputChunksCount = 4;

        const unsigned char *input[inputChunksCount] = {
            (const unsigned char *) "Hello, ",
            (const unsigned char *) "this is ",
            (const unsigned char *) "my custom ",
            (const unsigned char *) "message!"
        };

        Keccak_HashInstance hi;
        HashReturn result;

        // initialize the hash instance
        result = Keccak_HashInitialize_SHA3_256(&hi);
        assert(result == KECCAK_SUCCESS);

        for (int i = 0; i < inputChunksCount; i++) {
            // feed the input in chunks
            Keccak_HashUpdate(&hi, input[i], strlen((const char *) input[i]) * 8);
            assert(result == KECCAK_SUCCESS);
        }

        int outputByteLen = 32;
        unsigned char output[outputByteLen];

        // get the output
        result = Keccak_HashFinal(&hi, output);
        assert(result == KECCAK_SUCCESS);

        // printing the hash in hexadecimal format
        for (int i = 0; i < outputByteLen; i++)
            printf("\\x%02x", output[i]);
        printf("\n");

        // ...
    }

````
</details>

#### Example of using the `SHAKE128` XOF

<details open>
<summary>Simple usage</summary>

```c
 #include "SimpleFIPS202.h"

 int main() {
     // your input message
     const unsigned char *input = (const unsigned char *) "The random message to hash";

     // you can choose any output length
     int outputByteLen = 64;
     unsigned char output[outputByteLen];

     int result = SHAKE128(output, outputByteLen, input, strlen((const char *) input));
     // returning 0 means success
     assert(result == 0);

     // printing the hash in hexadecimal format
     for (int i = 0; i < outputByteLen; i++)
        printf("\\x%02x", output[i]);
     printf("\n");

     // ...
 }
````

</details>

<details open>
    <summary>Advanced: Chunked output</summary>
    Since a XOF function has an arbitrary output length, you might want to read the output in chunks.
    
   ```c
    #include "KeccakHash.h"

    int main() {
        // your input message
        const unsigned char *input = (const unsigned char *) "The random message to hash";

        Keccak_HashInstance hi;
        HashReturn result;

        // initialize the hash instance
        result = Keccak_HashInitialize_SHAKE128(&hi);
        assert(result == KECCAK_SUCCESS);

        // feed the input
        Keccak_HashUpdate(&hi, input, strlen((const char *) input) * 8);
        assert(result == KECCAK_SUCCESS);

        // call `Keccak_HashFinal` to mark the end of the input
        result = Keccak_HashFinal(&hi, NULL);
        assert(result == KECCAK_SUCCESS);

        // choose the output chunk length
        const int outputChunkByteLen = 16;
        unsigned char chunk[outputChunkByteLen];

        // choose the number of output chunks
        const int outputChunksCount = 4;

        // initialize the full output
        const int fullOutputByteLen = outputChunkByteLen * outputChunksCount;
        unsigned char output[fullOutputByteLen];

        for (int i = 0; i < outputChunksCount; i++) {
            result = Keccak_HashSqueeze(&hi, chunk, outputChunkByteLen * 8);
            assert(result == KECCAK_SUCCESS);

            // incrementally build the output, like writing to a file.
            // for simplicity, we use `memcpy` in this example:
            memcpy(output + (i * outputChunkByteLen), chunk, outputChunkByteLen);
        }

        // printing the output chunk in hexadecimal format
        for (int i = 0; i < fullOutputByteLen; i++)
            printf("\\x%02x", output[i]);
        printf("\n");

        // ...
    }

````

</details>

For more information on how to use the FIPS 202 functions, see the `SimpleFIPS202.h` and `KeccakHash.h` headers.

## TurboSHAKE
`TurboSHAKE` is a family of fast and secure XOFs. These are just like the SHAKE functions of FIPS 202, but with the
Keccak-p permutation reduced to 12 rounds (instead of 24), so about twice faster.
They are based on the `Sponge construction`, and the `Keccak-p[1600, 12]` permutation.

There are 2 main functions in this family:
- TurboSHAKE128
- TurboSHAKE256

The suffix `128` and `256` indicates the desired security level of the function.

We will give usage examples of the `TurboSHAKE128` function, but the same applies to the `TurboSHAKE256` function.

<details open>
<summary>Simple usage</summary>

```c
 #include "TurboSHAKE.h"

 int main() {
     // your input message
     const unsigned char *input = (const unsigned char *) "The random message to hash";

     // you can choose any output length
     int outputByteLen = 512;
     unsigned char output[outputByteLen];

     // choose a domain separation in the range `[0x01, 0x02, .. , 0x7F]`
     unsigned char domain = 0x1F;

     int result = TurboSHAKE(256, input, strlen((const char *) input), domain, output, outputByteLen);
     assert(result == 0);  // returning 0 means success

     // printing the hash in hexadecimal format
     for (int i = 0; i < outputByteLen; i++)
        printf("\\x%02x", output[i]);
     printf("\n");

     // ...
 }
````

</details>

<details open>
    <summary>Advanced: Chunked output</summary>
    Since a XOF function has an arbitrary output length, you might want to read the output in chunks.
    
   ```c
    #include "TurboSHAKE.h"

    int main() {
        // your input message
        const unsigned char *input = (const unsigned char *) "The random message to hash";

        TurboSHAKE_Instance tsi;

        // initialize the turboSHAKE instance
        int result = TurboSHAKE_Initialize(&tsi, 256);
        assert(result == 0);

        // feed the input
        result = TurboSHAKE_Absorb(&tsi, input, strlen((const char *) input));
        assert(result == 0);

        // choose a domain separation in the range `[0x01, 0x02, .. , 0x7F]`
        unsigned char domain = 0x1F;
        result = TurboSHAKE_AbsorbDomainSeparationByte(&tsi, domain);
        assert(result == 0);

        // choose the output chunk length
        const int outputChunkByteLen = 16;
        unsigned char chunk[outputChunkByteLen];

        // choose the number of output chunks
        const int outputChunksCount = 4;

        // initialize the full output
        const int fullOutputByteLen = outputChunkByteLen * outputChunksCount;
        unsigned char output[fullOutputByteLen];

        for (int i = 0; i < outputChunksCount; i++) {
            result = TurboSHAKE_Squeeze(&tsi, output, outputChunkByteLen);
            assert(result == 0);

            // incrementally build the output, like writing to a file.
            // for simplicity, we use `memcpy` in this example:
            memcpy(output + (i * outputChunkByteLen), chunk, outputChunkByteLen);
        }

        // printing the output chunk in hexadecimal format
        for (int i = 0; i < fullOutputByteLen; i++)
            printf("\\x%02x", output[i]);
        printf("\n");

        // ...
    }

````
</details>

## KangarooTwelve
`KangarooTwelve`, or `K12`, is a family of XOFs, based on the `TurboSHAKE128`, hence it also uses the `Keccak-p[1600, 12]` permutation.
On high-end platforms, it can exploit a high degree of parallelism, whether using multiple cores or the SIMD instruction set of modern processors.

We will give 2 examples of using the `KangarooTwelve` function, one for the simple usage, with single input single output, then a more advanced example, with chunked input/output.

<details open>
<summary>Simple usage</summary>

```c
 #include "KangarooTwelve.h"

 int main() {
     const unsigned char *input = (const unsigned char *) "The random message to hash";

     const int outputByteLen = 64;
     unsigned char output[outputByteLen];

     int result = KangarooTwelve(input, strlen((const char *) input), output, outputByteLen, NULL, 0);
     assert(result == 0);  // returning 0 means success

     // printing the hash in hexadecimal format
     for (int i = 0; i < outputByteLen; i++)
        printf("\\x%02x", output[i]);
     printf("\n");

     // ...
 }
````

</details>

<details open>
    <summary>Advanced: Chunked input/output</summary>
    We will feed the input in chunks, and get the output in chunks as well.
    
   ```c
    #include "KangarooTwelve.h"

    int main() {
        const int inputChunksCount = 4;

        const unsigned char *input[inputChunksCount] = {
            (const unsigned char *) "The ",
            (const unsigned char *) "random ",
            (const unsigned char *) "message ",
            (const unsigned char *) "to hash"
        };

        KangarooTwelve_Instance kti;

        int result = KangarooTwelve_Initialize(&kti, 0);
        assert(result == 0);

        for (int i = 0; i < inputChunksCount; i++) {
            result = KangarooTwelve_Update(&kti, input[i], strlen((const char *) input[i]));
            assert(result == 0);
        }

        result = KangarooTwelve_Final(&kti, NULL, NULL, 0);
        assert(result == 0);

        const int outputChunkByteLen = 16;
        unsigned char chunk[outputChunkByteLen];

        const int outputChunksCount = 4;

        const int fullOutputByteLen = outputChunkByteLen * outputChunksCount;
        unsigned char output[fullOutputByteLen];

        for (int i = 0; i < outputChunksCount; i++) {
            result = KangarooTwelve_Squeeze(&kti, chunk, outputChunkByteLen);
            assert(result == 0);

            memcpy(output + (i * outputChunkByteLen), chunk, outputChunkByteLen);
        }

        // printing the output chunk in hexadecimal format
        for (int i = 0; i < fullOutputByteLen; i++)
            printf("\\x%02x", output[i]);
        printf("\n");

        // ...
    }

````
</details>

# General-purpose deck functions

A deck function allows us to feed arbitrary long input chunks and get arbitrary long output chunks interchangeably,
while offering incremental properties on the input and output, helping in speeding up the computation.

## Kravatte

[//]: # (TODO: give some description about Kravatte)

### Basic usages

<details open>
<summary>Single input single output</summary>

```c
 #include "Kravatte.h"

 int main() {
     // choose your key
     const unsigned char *key = (const unsigned char *) "alksjdfo2300a9sdflkjasdfdq343ag2";

     const unsigned char *input = (const unsigned char *) "a random input message";

     Kravatte_Instance ki;
     int result;

     // initialize the Kravatte instance
     result = Kravatte_MaskDerivation(&ki, key, strlen((const char *) key) * 8);
     assert(result == 0);

     int outputByteLen = 32;
     unsigned char output[outputByteLen];

     // one call to feed the input and get the output
     result = Kravatte(&ki, input, strlen((const char *) input) * 8, output, outputByteLen * 8, 0);
     assert(result == 0);

     // printing the hash in hexadecimal format
     for (int i = 0; i < outputByteLen; i++)
        printf("\\x%02x", output[i]);
     printf("\n");

     // ...
 }
````

</details>

<details open>
    <summary>Advanced: Multiple input single output</summary>
    We will feed the input in chunks, and get the output at once.

```c
 #include "Kravatte.h"

 int main() {
     const unsigned char *key = (const unsigned char *) "alksjdfo2300a9sdflkjasdfdq343ag2";

     const int inputChunksCount = 4;
     const unsigned char *input[inputChunksCount] = {
         (const unsigned char *) "a ",
         (const unsigned char *) "random ",
         (const unsigned char *) "input ",
         (const unsigned char *) "message"
     };

     const unsigned char *expectedOutput = (const unsigned char *)
             "\xf9\x98\x1b\x67\x97\xbf\xf5\x45\xf5\xd1\xae\x6d\x33\x14\xb0\x99"
             "\xcd\x9c\x5a\xcb\x52\xa4\x56\x69\xf7\xea\x52\xc8\x30\xf6\xb3\x7e";

     Kravatte_Instance ki;
     int result;

     result = Kravatte_MaskDerivation(&ki, key,
     strlen((const char *) key) * 8);
     assert(result == 0);

     // feeding the input in chunks
     for (int i = 0; i < inputChunksCount; i++) {
         int flags = (i == inputChunksCount - 1) ? KRAVATTE_FLAG_LAST_PART : 0;
         result = Kra(&ki, input[i], strlen((const char *) input[i]) * 8, flags);
         assert(result == 0);
     }

     int outputByteLen = 32;
     unsigned char output[outputByteLen];

     // getting the output at once
     result = Vatte(&ki, output, outputByteLen * 8, KRAVATTE_FLAG_LAST_PART);
     assert(result == 0);

     // printing the output chunk in hexadecimal format
     for (int i = 0; i < outputByteLen; i++)
         printf("\\x%02x", output[i]);
     printf("\n");

     // ...
 }

```

</details>

<details open>
    <summary>Advanced: Single input multiple output</summary>
    We will feed the input at once, and get the output in chunks.

```c
 #include "Kravatte.h"

 int main() {
     const unsigned char *key = (const unsigned char *) "alksjdfo2300a9sdflkjasdfdq343ag2";
     const unsigned char *input = (const unsigned char *) "a random input message";

     Kravatte_Instance ki;
     int result;

     result = Kravatte_MaskDerivation(&ki, key,
     strlen((const char *) key) * 8);
     assert(result == 0);

     // feeding the input at once
     result = Kra(&ki, input, strlen((const char *) input) * 8, KRAVATTE_FLAG_LAST_PART);
     assert(result == 0);


     const int outputChunksCount = 2;
     const int outputChunkByteLen = 16;
     unsigned char chunkOutput[outputChunkByteLen];

     // initialize the full output
     const int fullOutputByteLen = outputChunkByteLen * outputChunksCount;
     unsigned char output[fullOutputByteLen];

     // getting the output in chunks
     for (int i = 0; i < outputChunksCount; i++) {
         int flags = (i == outputChunksCount - 1) ? KRAVATTE_FLAG_LAST_PART : 0;
         result = Vatte(&ki, chunkOutput, outputChunkByteLen * 8, flags);
         assert(result == 0);
         // incrementally build the output, like writing to a file.
         // for simplicity, we use `memcpy` in this example:
         memcpy(output + (i * outputChunkByteLen), chunkOutput, outputChunkByteLen);
     }

     // printing the output chunk in hexadecimal format
     for (int i = 0; i < fullOutputByteLen; i++)
         printf("\\x%02x", output[i]);
     printf("\n");

     // ...
 }
```

</details>
    
Those examples can be adapted to support multiple input multiple output as well, by using a sequence of `Kra` and `Vatte` calls.

### SANE mode (Authenticated Encryption)

SANE is an authenticated encryption scheme supporting sessions, the tag for the message n authenticates
the full history of the session up to that point, i.e. the messages 1, 2, ..., n. <br>

<details open>
   <summary>Authenticated Encryption: conversation example</summary>

```c
#include "KravatteModes.h"

struct Message
{
    unsigned char *data;
    unsigned char *metadata;
} Message;

struct Message messages[] = {
    {(unsigned char *)"Hello, how is it going?", (unsigned char *)"time: 2020-12-12 12:12:12"},
    {(unsigned char *)"Can we meet at 12:30?", (unsigned char *)"time: 2020-12-12 12:12:13"},
    {(unsigned char *)"I want to talk about the project", (unsigned char *)"time: 2020-12-12 12:12:14"}};


int main() {
    // ksiEnc is the sender, while ksiDec is the receiver
    Kravatte_SANE_Instance ksiEnc;
    Kravatte_SANE_Instance ksiDec;

    // choose any key
    const int keyBitLen = 256;
    BitSequence key[keyBitLen] = "alksjdfo2300a9sdflkjasdfdq343ag2";

    // choose the nonce. it must be the same for both ksiEnc and ksiDec
    const int nonceBitLen = 128;
    BitSequence nonce[nonceBitLen] = "alksjdfo2300a9sd";

    BitSequence tagEnc[Kravatte_SANE_TagLength];
    BitSequence tagDec[Kravatte_SANE_TagLength];

    int result;

    // initialize the instance for both sender and receiver
    // make sure to use the same key/nonce pair for both,
    // otherwise, `tagEnc` and `tagDec` won't match.

    result = Kravatte_SANE_Initialize(&ksiEnc, key, keyBitLen, nonce, nonceBitLen, tagEnc);
    assert(result == 0);

    result = Kravatte_SANE_Initialize(&ksiDec, key, keyBitLen, nonce, nonceBitLen, tagDec);
    assert(result == 0);

    assert(memcmp(tagEnc, tagDec, Kravatte_SANE_TagLength) == 0);

    BitSequence *ciphertexts[3];
    BitSequence *tags[3];

    // Encrypt the messages by the sender
    for (int i = 0; i < 3; i++)
    {
        struct Message message = messages[i];

        int messageBitLen = strlen((const char *)message.data) * 8;
        ciphertexts[i] = malloc(sizeof(BitSequence) * (messageBitLen / 8));

        tags[i] = malloc(sizeof(BitSequence) * Kravatte_SANE_TagLength);

        int metadataBitLen = strlen((const char *)message.metadata) * 8;

        // encrypt the message and store the ciphertext and the tag in `ciphertexts[i]`
        // and `tag[i]` for later use by the decryptor. In real application, we will
        // be sending ciphertext and the tag over the wire towards the decryptor.

        result = Kravatte_SANE_Wrap(
            &ksiEnc,
            message.data, ciphertexts[i], messageBitLen,
            message.metadata, metadataBitLen,
            tags[i]);

        assert(result == 0);
    }

    // CAUTION: uncomment the line below (tempering with the ciphertexts)
    // for the decryption to fail (i.e. for Kravatte_SANE_Unwrap to return 1)
    // ciphertexts[0][0] ^= 1;

    // Decrypt the messages by the receiver
    for (int i = 0; i < 3; i++)
    {
        struct Message message = messages[i];

        int messageBitLen = strlen((const char *)message.data) * 8;
        BitSequence *plaintext = malloc(sizeof(BitSequence) * (messageBitLen / 8));

        int metadataBitLen = strlen((const char *)message.metadata) * 8;

        result = Kravatte_SANE_Unwrap(
            &ksiDec,
            ciphertexts[i], plaintext, messageBitLen,
            message.metadata, metadataBitLen,
            tags[i]);

        assert(result == 0); // no corruption detected, tag is valid

        assert(memcmp(plaintext, message.data, messageBitLen / 8) == 0);
    }

    // don't forget to free the memory :)
    for (int i = 0; i < 3; i++)
    {
        free(ciphertexts[i]);
        free(tags[i]);
    }
    // ...
}
```

</details>

### SANSE mode (Authenticated Encryption)

Like SANE, SANE is an authenticated encryption scheme supporting sessions, the tag for the message n authenticates
the full history of the session up to that point, i.e. the messages 1, 2, ..., n. <br>

However, SANSE is a nonce-misuse resistant version of SANE, and doesn't require the user to provide a nonce.
Instead, it uses a nonce internally, and the user only needs to provide a key.

For better security, a user can include a nonce in the metadata of the first message.

<!-- TODO: clarify more about nonce misuse resistant of SANSE? -->

<details open>
   <summary>Authenticated Encryption: conversation example</summary>

```c
#include "KravatteModes.h"

struct Message
{
    unsigned char *data;
    unsigned char *metadata;
} Message;

struct Message messages[] = {
    {(unsigned char *)"Hello, how is it going?", (unsigned char *)"time: 2020-12-12 12:12:12"},
    {(unsigned char *)"Can we meet at 12:30?", (unsigned char *)"time: 2020-12-12 12:12:13"},
    {(unsigned char *)"I want to talk about the project", (unsigned char *)"time: 2020-12-12 12:12:14"}};


int main() {
    // ksiEnc is the sender, while ksiDec is the receiver
    Kravatte_SANSE_Instance ksiEnc;
    Kravatte_SANSE_Instance ksiDec;

    // choose any key
    const int keyBitLen = 256;
    BitSequence key[keyBitLen] = "alksjdfo2300a9sdflkjasdfdq343ag2";

    BitSequence tagEnc[Kravatte_SANSE_TagLength];
    BitSequence tagDec[Kravatte_SANSE_TagLength];

    int result;

    // initialize the instance for both sender and receiver with the same key

    result = Kravatte_SANSE_Initialize(&ksiEnc, key, keyBitLen);
    assert(result == 0);

    result = Kravatte_SANSE_Initialize(&ksiDec, key, keyBitLen);
    assert(result == 0);

    BitSequence *ciphertexts[3];
    BitSequence *tags[3];

    // Encrypt the messages by the sender
    for (int i = 0; i < 3; i++)
    {
        struct Message message = messages[i];

        int messageBitLen = strlen((const char *)message.data) * 8;
        ciphertexts[i] = malloc(sizeof(BitSequence) * (messageBitLen / 8));

        tags[i] = malloc(sizeof(BitSequence) * Kravatte_SANSE_TagLength);

        int metadataBitLen = strlen((const char *)message.metadata) * 8;

        // encrypt the message and store the ciphertext and the tag in `ciphertexts[i]`
        // and `tag[i]` for later use by the decryptor. In real application, we will
        // be sending ciphertext and the tag over the wire towards the decryptor.

        result = Kravatte_SANSE_Wrap(
            &ksiEnc,
            message.data, ciphertexts[i], messageBitLen,
            message.metadata, metadataBitLen,
            tags[i]);

        assert(result == 0);
    }

    // CAUTION: uncomment the line below (tempering with the ciphertexts)
    // for the decryption to fail (i.e. for Kravatte_SANSE_Unwrap to return 1)
    // ciphertexts[0][0] ^= 1;

    // Decrypt the messages by the receiver
    for (int i = 0; i < 3; i++)
    {
        struct Message message = messages[i];

        int messageBitLen = strlen((const char *)message.data) * 8;
        BitSequence *plaintext = malloc(sizeof(BitSequence) * (messageBitLen / 8));

        int metadataBitLen = strlen((const char *)message.metadata) * 8;

        result = Kravatte_SANSE_Unwrap(
            &ksiDec,
            ciphertexts[i], plaintext, messageBitLen,
            message.metadata, metadataBitLen,
            tags[i]);

        assert(result == 0); // no corruption detected, tag is valid

        assert(memcmp(plaintext, message.data, messageBitLen / 8) == 0);
    }

    // don't forget to free the memory :)
    for (int i = 0; i < 3; i++)
    {
        free(ciphertexts[i]);
        free(tags[i]);
    }

    // ...
}
```

</details>

### WBC mode (Wide Block Cipher)

WBC is a wide block cipher mode, Built on top of the Farfalle construction and the Feistel network.
It can be used to encrypt a message of any length, and produce a ciphertext of the same length.

<details open>
   <summary>Simple encryption/decryption example</summary>

```c
#include "KravatteModes.h"

int main() {
    Kravatte_Instance kwiEnc;

    // choose any key
    const int keyBitLen = 256;
    BitSequence key[keyBitLen] = "alksjdfo2300a9sdflkjasdfdq343ag2";

    int result;

    // initialize the WBC instance
    result = Kravatte_WBC_Initialize(&kwiEnc, key, keyBitLen);
    assert(result == 0);

    const BitSequence *plaintext = (const BitSequence *)"The random message to encrypt";
    int messageBitLen = strlen((const char *)plaintext) * 8;

    BitSequence *ciphertext = malloc(sizeof(BitSequence) * (messageBitLen / 8));

    // choose a unique tweak - it must be the same for the encryption and decryption
    const int tweakBitLen = 128;
    BitSequence tweak[tweakBitLen] = "alksjdfo2300a9sd";

    result = Kravatte_WBC_Encipher(&kwiEnc, plaintext, ciphertext, messageBitLen, tweak, tweakBitLen);
    assert(result == 0);

    BitSequence *decrypted = malloc(sizeof(BitSequence) * (messageBitLen / 8));

    // if the tweak is not the same as the one used for encryption, the decryption will fail
    result = Kravatte_WBC_Decipher(&kwiEnc, ciphertext, decrypted, messageBitLen, tweak, tweakBitLen);
    assert(result == 0);

    assert(memcmp(plaintext, decrypted, messageBitLen / 8) == 0);

    // ...
}
```

</details>

### WBC-AE mode (Wide Block Cipher Authenticated Encryption)

WBC-AE is an Authenticated Encryption mode, Built on top of the WBC mode.

<details open>
   <summary>Simple encryption/decryption example with authentication</summary>

```c
#include "KravatteModes.h"

int main() {
    Kravatte_Instance kwiInstance;

    // choose any key
    const int keyBitLen = 256;
    BitSequence key[keyBitLen] = "alksjdfo2300a9sdflkjasdfdq343ag2";

    int result;

    // initialize the WBC-AE instance
    result = Kravatte_WBCAE_Initialize(&kwiInstance, key, keyBitLen);
    assert(result == 0);

    char plaintext[] = "The random message to encrypt";
    int messageBitLen = strlen((const char *)plaintext) * 8;

    // WBC_AE adds additional `Kravatte_WBCAE_t` bits to the message as tag
    // so we need to allocate buffers with size `messageBitLen + Kravatte_WBCAE_t`
    int extendedBitLen = messageBitLen + Kravatte_WBCAE_t;

    BitSequence *plaintext_buffer = malloc((extendedBitLen) / 8);
    memcpy(plaintext_buffer, plaintext, messageBitLen / 8);

    BitSequence *ciphertext = malloc((extendedBitLen) / 8);

    const int metadataBitLen = 200;
    BitSequence metadata[metadataBitLen] = "time: 2020-12-12 12:12:12";

    // encrypt the message
    result = Kravatte_WBCAE_Encipher(&kwiInstance, plaintext_buffer, ciphertext, messageBitLen, metadata, metadataBitLen);
    assert(result == 0);

    // CAUTION: uncomment the line below (tempering with the ciphertexts)
    // for the decryption to fail (i.e. for Kravatte_WBCAE_Decipher to return 1)
    // ciphertext_extended[0] ^= 1;

    unsigned char *decrypted = malloc((extendedBitLen) / 8);

    result = Kravatte_WBCAE_Decipher(&kwiInstance, ciphertext, decrypted, messageBitLen, metadata, metadataBitLen);
    assert(result == 0);

    assert(memcmp(plaintext, decrypted, messageBitLen / 8) == 0);

    // don't forget to free the memory :)
    free(ciphertext);
    free(decrypted);

    // ...
}
```

</details>

...

## Xoofff

Xoofff is similar to Kravatte, it's also built on top of the Farfalle construction, however,
it uses the Xoodoo permutation as primitive instead of the Keccak-p used in Kravatte, which makes it more suitable for constrained environments.

Xoofff has an API similar to Kravatte, so the examples of Kravatte can be adapted to use Xoofff.
In the following, we will give the differences between the APIs of Kravatte and Xoofff.

One can use the below conversion tables to adapt the examples of Kravatte to use Xoofff.

### Basic usages

| Feature                   | Kravatte                | Xoofff                |
| ------------------------- | ----------------------- | --------------------- |
| Header                    | "Kravatte.h"            | "Xoofff.h"            |
| Instance Type             | Kravatte_Instance       | Xoofff_Instance       |
| Initialization            | Kravatte_MaskDerivation | Xoofff_MaskDerivation |
| One call Hashing Function | Kravatte                | Xoofff                |
| Input feeding Function    | Kra                     | Xoofff_Compress       |
| Output Retrieval Function | Vatte                   | Xoofff_Expand         |
| Flag for Last Part        | KRAVATTE_FLAG_LAST_PART | Xoofff_FlagLastPart   |

### SANE mode (Authenticated Encryption)

| Feature             | Kravatte SANE            | Xoofff SANE           |
| ------------------- | ------------------------ | --------------------- |
| Header              | "KravatteModes.h"        | "XoofffModes.h"       |
| Instance Type       | Kravatte_SANE_Instance   | XoofffSANE_Instance   |
| Initialization      | Kravatte_SANE_Initialize | XoofffSANE_Initialize |
| Wrapping Function   | Kravatte_SANE_Wrap       | XoofffSANE_Wrap       |
| Unwrapping Function | Kravatte_SANE_Unwrap     | XoofffSANE_Unwrap     |
| Tag Length          | Kravatte_SANE_TagLength  | XoofffSANE_TagLength  |

### SANSE mode (Authenticated Encryption)

| Feature             | Kravatte SANSE            | Xoofff SANSE           |
| ------------------- | ------------------------- | ---------------------- |
| Header              | "KravatteModes.h"         | "XoofffModes.h"        |
| Instance Type       | Kravatte_SANSE_Instance   | XoofffSANSE_Instance   |
| Initialization      | Kravatte_SANSE_Initialize | XoofffSANSE_Initialize |
| Wrapping Function   | Kravatte_SANSE_Wrap       | XoofffSANSE_Wrap       |
| Unwrapping Function | Kravatte_SANSE_Unwrap     | XoofffSANSE_Unwrap     |
| Tag Length          | Kravatte_SANSE_TagLength  | XoofffSANSE_TagLength  |

### WBC

| Feature           | Kravatte WBC            | Xoofff WBC           |
| ----------------- | ----------------------- | -------------------- |
| Header            | "KravatteModes.h"       | "XoofffModes.h"      |
| Instance Type     | Kravatte_Instance       | Xoofff_Instance      |
| Initialization    | Kravatte_WBC_Initialize | XoofffWBC_Initialize |
| Encipher Function | Kravatte_WBC_Encipher   | XoofffWBC_Encipher   |
| Decipher Function | Kravatte_WBC_Decipher   | XoofffWBC_Decipher   |

### WBC-AE

| Feature           | Kravatte WBC-AE           | Xoofff WBC-AE          |
| ----------------- | ------------------------- | ---------------------- |
| Header            | "KravatteModes.h"         | "XoofffModes.h"        |
| Instance Type     | Kravatte_Instance         | Xoofff_Instance        |
| Initialization    | Kravatte_WBCAE_Initialize | XoofffWBCAE_Initialize |
| Encipher Function | Kravatte_WBCAE_Encipher   | XoofffWBCAE_Encipher   |
| Decipher Function | Kravatte_WBCAE_Decipher   | XoofffWBCAE_Decipher   |

## Xoodyak

```c
#include "libXKCP.a.headers/Xoodyak.h"

struct Message
{
    unsigned char *data;
    unsigned char *metadata;
} Message;

struct Message messages[] = {
    {(unsigned char *)"Hello, how is it going?", (unsigned char *)"time: 2020-12-12 12:12:12"},
    {(unsigned char *)"Can we meet at 12:30?", (unsigned char *)"time: 2020-12-12 12:12:13"},
    {(unsigned char *)"I want to talk about the project", (unsigned char *)"time: 2020-12-12 12:12:14"},
    {(unsigned char *)"", (unsigned char *)"time: 2020-12-12 12-12:16"},
    {(unsigned char*)"I am going to be late", (unsigned char *)""},
    {(unsigned char *)"", (unsigned char *)""}
    };

void testXoodyakHashedModeSingleInputSingleOuput()
{
    unsigned char *expected_output = "\x0F\x84\x57\x4F\xA4\x63\xA4\x72\x11\x46\xB3\xC1\xE0\xE0\x46\xF0"
                                     "\x22\x37\xBB\xBD\x57\xB0\x52\x01\xC0\x9C\xF2\x70\x7A\x45\x15\x27";

    Xoodyak_Instance instance;

    Xoodyak_Initialize(&instance, NULL, 0, NULL, 0, NULL, 0);

    Xoodyak_Absorb(&instance, messages[0].data, strlen((char *)messages[0].data));

    const int outputByteLen = 32;
    unsigned char output[outputByteLen];

    Xoodyak_Squeeze(&instance, output, outputByteLen);

    assert(memcmp(output, expected_output, outputByteLen) == 0);
}

void testXoodyakHashedModeMultipleInputSingleOutput()
{
    unsigned char *expected_output = "\x60\x3D\x6D\x0E\xC4\x57\xAD\x3B\xCA\xD4\xE2\x7C\xFE\x19\xF8\x6F"
                                     "\xEA\x46\xF6\x48\x54\x48\xA5\xB5\xB2\xF1\xAD\xEE\x17\x88\x4D\x5C";

    Xoodyak_Instance instance;

    Xoodyak_Initialize(&instance, NULL, 0, NULL, 0, NULL, 0);

    Xoodyak_Absorb(&instance, messages[0].data, strlen((char *)messages[0].data));
    Xoodyak_Absorb(&instance, messages[1].data, strlen((char *)messages[1].data));
    Xoodyak_Absorb(&instance, messages[2].data, strlen((char *)messages[2].data));

    const int outputByteLen = 32;
    unsigned char output[outputByteLen];

    Xoodyak_Squeeze(&instance, output, outputByteLen);

    assert(memcmp(output, expected_output, outputByteLen) == 0);
}

void testXoodyakHashedModeSingleInputMultipleOutput()
{
    unsigned char *expected_outputs[2] = {
        // the first chunk should be the same as the first 16 bytes of the output of
        // `testXoodyakHashedModeSingleInputSingleOuput` test - this is due to the Cyclist properties
        (unsigned char *)"\x0F\x84\x57\x4F\xA4\x63\xA4\x72\x11\x46\xB3\xC1\xE0\xE0\x46\xF0",
        // the second chunk will be different
        (unsigned char *)"\x8F\x79\x9D\x78\x94\x19\x1F\x00\xF9\x5E\x92\x99\x1D\x25\x42\xF6"};

    Xoodyak_Instance instance;

    Xoodyak_Initialize(&instance, NULL, 0, NULL, 0, NULL, 0);

    Xoodyak_Absorb(&instance, messages[0].data, strlen((char *)messages[0].data));

    const int outputByteLen = 16;
    unsigned char output[outputByteLen];

    for (int i = 0; i < 2; i++)
    {
        Xoodyak_Squeeze(&instance, output, outputByteLen);
        assert(memcmp(output, expected_outputs[i], outputByteLen) == 0);
    }
}

void testXoodyakHashedModeMultipleInputMultipleOutput()
{
    unsigned char *expected_outputs[2] = {
        // the first chunk should be the same as the first 16 bytes of the output of
        // `testXoodyakHashedModeMultipleInputSingleOutput` test - this is due to the Cyclist properties
        (unsigned char *)"\x60\x3D\x6D\x0E\xC4\x57\xAD\x3B\xCA\xD4\xE2\x7C\xFE\x19\xF8\x6F",
        // the second chunk will be different
        (unsigned char *)"\xF4\x13\x99\x57\xBD\x9F\x6B\xF0\x66\xE0\xCE\x50\xEF\x09\x8F\xD7"};

    Xoodyak_Instance instance;

    Xoodyak_Initialize(&instance, NULL, 0, NULL, 0, NULL, 0);

    Xoodyak_Absorb(&instance, messages[0].data, strlen((char *)messages[0].data));
    Xoodyak_Absorb(&instance, messages[1].data, strlen((char *)messages[1].data));
    Xoodyak_Absorb(&instance, messages[2].data, strlen((char *)messages[2].data));

    const int outputByteLen = 16;
    unsigned char output[outputByteLen];

    for (int i = 0; i < 2; i++)
    {
        Xoodyak_Squeeze(&instance, output, outputByteLen);
        assert(memcmp(output, expected_outputs[i], outputByteLen) == 0);
    }
}

void testXoodyakHashedModes()
{
    testXoodyakHashedModeSingleInputSingleOuput();
    testXoodyakHashedModeMultipleInputSingleOutput();
    testXoodyakHashedModeSingleInputMultipleOutput();
    testXoodyakHashedModeMultipleInputMultipleOutput();
}

void testXoodyakKeyedSimpleEncryptDecrypt() {
    Xoodyak_Instance encInstance;
    Xoodyak_Instance decInstance;

    unsigned char key[16] = "o2jso2j!~l;aksj-";

    Xoodyak_Initialize(&encInstance, key, 16, NULL, 0, NULL, 0);
    Xoodyak_Initialize(&decInstance, key, 16, NULL, 0, NULL, 0);

    unsigned char nonce[16] = "#dojd983&72-21!@";

    // they must absorb the same nonce
    Xoodyak_Absorb(&encInstance, nonce, 16);
    Xoodyak_Absorb(&decInstance, nonce, 16);

    const int messageByteLen = strlen((char *)messages[0].data);

    unsigned char encrypted[messageByteLen];
    Xoodyak_Encrypt(&encInstance, messages[0].data, encrypted, messageByteLen);

    unsigned char decrypted[messageByteLen];
    Xoodyak_Decrypt(&decInstance, encrypted, decrypted, messageByteLen);

    assert(memcmp(messages[0].data, decrypted, messageByteLen) == 0);
}

void testXoodyakKeyedAuthenticatedEncryption() {
    Xoodyak_Instance encInstance;
    Xoodyak_Instance decInstance;

    unsigned char key[16] = "o2jso2j!~l;aksj-";

    Xoodyak_Initialize(&encInstance, key, 16, NULL, 0, NULL, 0);
    Xoodyak_Initialize(&decInstance, key, 16, NULL, 0, NULL, 0);

    unsigned char nonce[16] = "#dojd983&72-21!@";

    // they must absorb the same nonce
    Xoodyak_Absorb(&encInstance, nonce, 16);
    Xoodyak_Absorb(&decInstance, nonce, 16);

    const int messageByteLen = strlen((char *)messages[0].data);
    const int tagByteLen = 16;

    // sender side:
    
    unsigned char encrypted[messageByteLen];
    Xoodyak_Absorb(&encInstance, messages[0].metadata, messageByteLen);
    Xoodyak_Encrypt(&encInstance, messages[0].data, encrypted, messageByteLen);

    unsigned char tag[tagByteLen];
    Xoodyak_Squeeze(&encInstance, tag, tagByteLen);

    // on the wire:
    // CAUTION: temper with the encrypted message (flip a bit) by uncommenting
    // the following line and the tags will not match -> Authentication insured.
    // encrypted[0] ^= 1;

    // receiver side:

    unsigned char decrypted[messageByteLen];
    Xoodyak_Absorb(&decInstance, messages[0].metadata, messageByteLen);
    Xoodyak_Decrypt(&decInstance, encrypted, decrypted, messageByteLen);

    unsigned char expectedTag[tagByteLen];
    Xoodyak_Squeeze(&decInstance, expectedTag, tagByteLen);

    assert(memcmp(messages[0].data, decrypted, messageByteLen) == 0);
    assert(memcmp(tag, expectedTag, tagByteLen) == 0);
}

void testXoodyakKeyedSessionAuthenticatedEncryption() {
    Xoodyak_Instance aliceInstance;
    Xoodyak_Instance bobInstance;

    unsigned char key[16] = "o2jso2j!~l;aksj-";

    Xoodyak_Initialize(&aliceInstance, key, 16, NULL, 0, NULL, 0);
    Xoodyak_Initialize(&bobInstance, key, 16, NULL, 0, NULL, 0);

    unsigned char nonce[16] = "#dojd983&72-21!@";

    // they must absorb the same nonce
    Xoodyak_Absorb(&aliceInstance, nonce, 16);
    Xoodyak_Absorb(&bobInstance, nonce, 16);

    const int tagByteLen = 16;
    const int messageCount = 6;

    unsigned char *encrypted_messages[messageCount];
    unsigned char *tags[messageCount];

    // Alice sends all the messages to Bob:

    for (int i = 0; i < messageCount; i++) {
        const int messageByteLen = strlen((char *)messages[i].data);

        tags[i] = malloc(tagByteLen);

        if (messages[i].metadata != NULL) {
            Xoodyak_Absorb(&aliceInstance, messages[i].metadata, strlen((char *)messages[i].metadata));
        }

        if (messageByteLen) {
            encrypted_messages[i] = malloc(messageByteLen);
            Xoodyak_Encrypt(&aliceInstance, messages[i].data, encrypted_messages[i], messageByteLen);
        } else {
            encrypted_messages[i] = NULL;
        }

        Xoodyak_Squeeze(&aliceInstance, tags[i], tagByteLen);
    }

    // CAUTION: temper with the encrypted message (flip a bit) by uncommenting
    // the following line and the tags will not match -> Authentication insured.
    // encrypted_messages[0][0] ^= 1;

    // Bob receives the messages and ensure that they're authenticated:

    for (int i = 0; i < messageCount; i++) {
        const int messageByteLen = strlen((char *)messages[i].data);

        if (messages[i].metadata != NULL) {
            Xoodyak_Absorb(&bobInstance, messages[i].metadata, strlen((char *)messages[i].metadata));
        }

        if (messageByteLen) {
            unsigned char decrypted[messageByteLen];
            Xoodyak_Decrypt(&bobInstance, encrypted_messages[i], decrypted, messageByteLen);
            assert(memcmp(messages[i].data, decrypted, messageByteLen) == 0);
        } else {
            assert(encrypted_messages[i] == NULL);
        }

        unsigned char expectedTag[tagByteLen];
        Xoodyak_Squeeze(&bobInstance, expectedTag, tagByteLen);
        assert(memcmp(tags[i], expectedTag, tagByteLen) == 0);
    }


    // Bob sends a confirmation message to Alice:

    unsigned char confirmationMessage[32] = "I have received all the messages";
    
    unsigned char confirmationEncrypted[32];
    Xoodyak_Encrypt(&bobInstance, confirmationMessage, confirmationEncrypted, 32);

    unsigned char confirmationTag[tagByteLen];
    Xoodyak_Squeeze(&bobInstance, confirmationTag, tagByteLen);

    // CAUTION: temper with the encrypted message (flip a bit) by uncommenting
    // the following line and the tags will not match -> Authentication insured.
    // confirmationEncrypted[0] ^= 1;

    // Alice receives the confirmation message and ensure that it's authenticated:

    unsigned char confirmationDecrypted[32];
    Xoodyak_Decrypt(&aliceInstance, confirmationEncrypted, confirmationDecrypted, 32);

    unsigned char expectedConfirmationTag[tagByteLen];
    Xoodyak_Squeeze(&aliceInstance, expectedConfirmationTag, tagByteLen);

    assert(memcmp(confirmationMessage, confirmationDecrypted, 32) == 0);
    assert(memcmp(confirmationTag, expectedConfirmationTag, tagByteLen) == 0);

    for (int i = 0; i < messageCount; i++) {
        free(encrypted_messages[i]);
        free(tags[i]);
    }
}

void testXoodyakKeyedSessionAuthenticatedEncryptionWithRollingSubKeys() {
    Xoodyak_Instance aliceInstanceSession1;
    Xoodyak_Instance bobInstanceSession1;

    unsigned char aliceKey[16] = "o2jso2j!~l;aksj-";
    unsigned char bobKey[16] = "o2jso2j!~l;aksj-";

    // choose the same key for both instances
    assert(memcmp(aliceKey, bobKey, 16) == 0);

    Xoodyak_Initialize(&aliceInstanceSession1, aliceKey, 16, NULL, 0, NULL, 0);
    Xoodyak_Initialize(&bobInstanceSession1, bobKey, 16, NULL, 0, NULL, 0);

    unsigned char nonceSession1[16] = "#dojd983&72-21!@";

    // they must absorb the same nonce
    Xoodyak_Absorb(&aliceInstanceSession1, nonceSession1, 16);
    Xoodyak_Absorb(&bobInstanceSession1, nonceSession1, 16);

    // now after we have initialized the instances of session 1,
    // we can derive the subkeys for session 2 and stored it for later.
    Xoodyak_SqueezeKey(&aliceInstanceSession1, aliceKey, 16);
    Xoodyak_SqueezeKey(&bobInstanceSession1, bobKey, 16);

    // the new derived keys must be the same
    assert(memcmp(aliceKey, bobKey, 16) == 0);

    // start of session 1 (note that we will not detail the usage of Xoodyak here, 
    // for a detailed example, see `testXoodyakKeyedSessionAuthenticatedEncryption`)

    const int tagByteLen = 16;

    // alice sends a message to bob
    unsigned char aliceToBobMessage[24] = "Hello Bob, this is Alice";

    unsigned char encryptedAliceToBobMessage[24];
    Xoodyak_Encrypt(&aliceInstanceSession1, aliceToBobMessage, encryptedAliceToBobMessage, 24);

    unsigned char aliceToBobMessageTag[tagByteLen];
    Xoodyak_Squeeze(&aliceInstanceSession1, aliceToBobMessageTag, tagByteLen);

    // bob receives the message and decrypts it

    unsigned char decryptedAliceToBobMessage[24];
    Xoodyak_Decrypt(&bobInstanceSession1, encryptedAliceToBobMessage, decryptedAliceToBobMessage, 24);

    unsigned char expectedAliceToBobMessageTag[tagByteLen];
    Xoodyak_Squeeze(&bobInstanceSession1, expectedAliceToBobMessageTag, tagByteLen);

    assert(memcmp(aliceToBobMessage, decryptedAliceToBobMessage, 24) == 0);
    assert(memcmp(aliceToBobMessageTag, expectedAliceToBobMessageTag, tagByteLen) == 0);
    

    // start of session 2

    Xoodyak_Instance aliceInstanceSession2;
    Xoodyak_Instance bobInstanceSession2;

    Xoodyak_Initialize(&aliceInstanceSession2, aliceKey, 16, NULL, 0, NULL, 0);
    Xoodyak_Initialize(&bobInstanceSession2, bobKey, 16, NULL, 0, NULL, 0);

    unsigned char nonceSession2[16] = "#dojd923572-21!@";

    // they must absorb the same nonce
    Xoodyak_Absorb(&aliceInstanceSession2, nonceSession2, 16);
    Xoodyak_Absorb(&bobInstanceSession2, nonceSession2, 16);


    // bob sends a message to alice

    unsigned char bobToAliceMessage[24] = "Hello Alice, this is Bob";

    unsigned char encryptedBobToAliceMessage[24];
    Xoodyak_Encrypt(&bobInstanceSession2, bobToAliceMessage, encryptedBobToAliceMessage, 24);

    unsigned char bobToAliceMessage1Tag[tagByteLen];
    Xoodyak_Squeeze(&bobInstanceSession2, bobToAliceMessage1Tag, tagByteLen);

    // alice receives the message and decrypts it

    unsigned char decryptedBobToAliceMessage[24];
    Xoodyak_Decrypt(&aliceInstanceSession2, encryptedBobToAliceMessage, decryptedBobToAliceMessage, 24);

    unsigned char expectedBobToAliceMessageTag[tagByteLen];
    Xoodyak_Squeeze(&aliceInstanceSession2, expectedBobToAliceMessageTag, tagByteLen);

    assert(memcmp(bobToAliceMessage, decryptedBobToAliceMessage, 24) == 0);
    assert(memcmp(bobToAliceMessage1Tag, expectedBobToAliceMessageTag, tagByteLen) == 0);
}

void testXoodyakKeyedAuthenticatedEncryptionWithRatchet() {
    Xoodyak_Instance encInstance;
    Xoodyak_Instance decInstance;

    unsigned char key[16] = "o2jso2j!~l;aksj-";

    Xoodyak_Initialize(&encInstance, key, 16, NULL, 0, NULL, 0);
    Xoodyak_Initialize(&decInstance, key, 16, NULL, 0, NULL, 0);

    unsigned char nonce[16] = "#dojd983&72-21!@";

    // they must absorb the same nonce
    Xoodyak_Absorb(&encInstance, nonce, 16);
    Xoodyak_Absorb(&decInstance, nonce, 16);

    const int messageByteLen = strlen((char *)messages[0].data);
    const int tagByteLen = 16;

    // sender side:
    
    unsigned char encrypted[messageByteLen];
    Xoodyak_Absorb(&encInstance, messages[0].metadata, messageByteLen);
    Xoodyak_Encrypt(&encInstance, messages[0].data, encrypted, messageByteLen);

    // Ratchet can be called here (before squeezing the tag) and in 
    //  that case, it only requires on extra call to the permutation f.
    //  In this example, it's called before squeezing the tag.
    Xoodyak_Ratchet(&encInstance);

    unsigned char tag[tagByteLen];
    Xoodyak_Squeeze(&encInstance, tag, tagByteLen);

    // Ratchet can be called here (after squeezing the tag) and in
    //  that case, its processing can be done asynchronously, while
    //  the message is transmitted, and waits for the next message.

    // on the wire:
    // CAUTION: temper with the encrypted message (flip a bit) by uncommenting
    // the following line and the tags will not match -> Authentication insured.
    // encrypted[0] ^= 1;

    // receiver side:

    unsigned char decrypted[messageByteLen];
    Xoodyak_Absorb(&decInstance, messages[0].metadata, messageByteLen);
    Xoodyak_Decrypt(&decInstance, encrypted, decrypted, messageByteLen);

    // same as the sender, the receiver has to call Ratchet before squeezing the tag.
    Xoodyak_Ratchet(&decInstance);

    unsigned char expectedTag[tagByteLen];
    Xoodyak_Squeeze(&decInstance, expectedTag, tagByteLen);

    assert(memcmp(messages[0].data, decrypted, messageByteLen) == 0);
    assert(memcmp(tag, expectedTag, tagByteLen) == 0);
}

void testXoodyakKeyedModes() {
    testXoodyakKeyedSimpleEncryptDecrypt();
    testXoodyakKeyedAuthenticatedEncryption();
    testXoodyakKeyedSessionAuthenticatedEncryption();
    testXoodyakKeyedSessionAuthenticatedEncryptionWithRollingSubKeys();
    testXoodyakKeyedAuthenticatedEncryptionWithRatchet();
}

void testXoodyakCombinedMode() {
    // here we will assume that Alice and Bob have both their own public keys,
    // and they have done the Diffie-Hellman key exchange to obtain a shared secret
    // key. We will use Xoodyak in hash mode to first absorb these keys, and a nonce,
    // and derive a session key as the secret key to use for this session.

    // Alice and Bob have their own public keys
    unsigned char alicePublicKey[32] = "alice's public key";
    unsigned char bobPublicKey[32] = "bob's public key";

    // Alice and Bob have done the Diffie-Hellman key exchange to obtain a shared secret key
    unsigned char sharedSecretKey[32] = "shared secret key";

    // Alice and Bob have agreed on a nonce
    unsigned char nonce[16] = "nonce";

    // Now we get the session key by hashing the public keys, the shared secret key and the nonce
    Xoodyak_Instance sessionKeyInstance;
    Xoodyak_Initialize(&sessionKeyInstance, NULL, 0, NULL, 0, NULL, 0);
    // absorb the protocol id - could be anything.
    Xoodyak_Absorb(&sessionKeyInstance, "Xoodyak_Combined_Mode", 21);
    // absorb the keys
    Xoodyak_Absorb(&sessionKeyInstance, alicePublicKey, 32);
    Xoodyak_Absorb(&sessionKeyInstance, bobPublicKey, 32);
    Xoodyak_Absorb(&sessionKeyInstance, sharedSecretKey, 32);
    // absorb a nonce - note that if the keys are ephemeral, no need for a nonce
    Xoodyak_Absorb(&sessionKeyInstance, nonce, 16);
    // squeeze the session key
    unsigned char sessionKey[32];
    Xoodyak_Squeeze(&sessionKeyInstance, sessionKey, 32);

    // Now we can use the session key to encrypt and decrypt messages
    //  (note that in this example, we will not authenticate the messages
    //  simplicity - look into the above examples for authenticated encryption)
    Xoodyak_Instance encInstance;
    Xoodyak_Instance decInstance;

    Xoodyak_Initialize(&encInstance, sessionKey, 32, NULL, 0, NULL, 0);
    Xoodyak_Initialize(&decInstance, sessionKey, 32, NULL, 0, NULL, 0);

    // Alice sends a message to Bob
    unsigned char message[32] = "Hello Bob, this is Alice";

    // Alice encrypts the message
    unsigned char encrypted[32];
    Xoodyak_Encrypt(&encInstance, message, encrypted, 32);

    // Bob decrypts the message
    unsigned char decrypted[32];
    Xoodyak_Decrypt(&decInstance, encrypted, decrypted, 32);

    assert(memcmp(message, decrypted, 32) == 0);

    // Bob sends a message to Alice
    unsigned char message2[32] = "Hello Alice, this is Bob";

    // Bob encrypts the message
    unsigned char encrypted2[32];
    Xoodyak_Encrypt(&decInstance, message2, encrypted2, 32);

    // Alice decrypts the message
    unsigned char decrypted2[32];
    Xoodyak_Decrypt(&encInstance, encrypted2, decrypted2, 32);

    assert(memcmp(message2, decrypted2, 32) == 0);
}

int main()
{
    printf("Tests started..\n");

    testXoodyakHashedModes();
    testXoodyakKeyedModes();
    testXoodyakCombinedMode();

    printf("All tests passed\n");

    return 0;
}

```
