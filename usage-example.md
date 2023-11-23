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
   ```
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
   ```
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
   ```

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
   ```
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
   ```
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
   ```
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
   ```

# {{Other types of functions}}

- [ ]  Kravatte and its modes
- [ ]  Xoofff and its modes
- [ ]  Xoodyak

...