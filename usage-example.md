In this guide, we will provide examples of using the high-level API, with less emphasis on 
the low-level implementation, since XKCP abstracts the low-level implementation from the user.

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

[//]: # (TODO: verify the above statement)

### Extendable output functions (XOFs)
A XOF is a function on bit strings, also called _messages_, in which the output can be extended to any desired length. <br>

NIST standardized the following XOFs in FIPS 202, marking them as the first XOFs to be standardized by NIST.
- `SHAKE128`
- `SHAKE256`

The suffix `128` and `256` indicates the desired security level of the function.

### Usage

To use any of them in your C/C++ project, you have to first build the FIPS202 library, and include it in your project.
The following steps illustrate how to do that:

1. From `LowLevel.build`, choose your low level implementation, it has to be one for `Keccak-p[1600]`  
since the FIPS 202 functions are based on the `Keccak-p 1600`. In the following,
we will use the unoptimized reference implementation `K1600-ref-64bits`.
You can find more information about available low-level implementations in the `LowLevel.build` file.
2. In `HOWTO-customize.build`, add the following line:
```<target name="FIPS202.a" inherits="FIPS202 KeccakP-1600-reference" />```
Where `FIPS202` is the name of the high-level target that includes the FIPS 202 functions, 
more information about the high-level targets can be found in the `HighLevel.build` file.
3. From the root of the XKCP repository, run `make FIPS202.a`
4. In `bin` you will find the static library `FIPS202.a`, and the corresponding headers directory `FIPS202.a.headers`.
You can copy those to your project directory.
5. In your C/C++ main file, include the header `#include "FIPS202.a.headers/SimpleFIPS202.h"
6. You are ready to use the FIPS 202 functions in your code. See below for an example of using the `SHA3_256` and `SHAKE128` functions.
7. Before executing the code, you have to compile the code with the static library `FIPS202.a`.
   When using `gcc`, this step might look like this:
    ```bash
    gcc main.c FIPS202_reference.a;
    ```
8. Run the executable:
    ```bash
    ./a.out
    ```

#### Example of using the `SHA3_256` hash function
    
Add the following code to your C/C++ main file:

```c
// your input message
const unsigned char *input = 
        (const unsigned char *) "The random message to hash";

int OUTPUT_LENGTH = 32;
unsigned char output[OUTPUT_LENGTH];

int result = SHA3_256(output, input, strlen((const char *) input));

// returning 0 means success
assert(result == 0);

// printing the hash in hexadecimal format
for (int i = 0; i < OUTPUT_LENGTH; i++)
    printf("\\x%02x", output[i]);
 
printf("\n");
```

#### Example of using the `SHAKE128` XOF

Add the following code to your C/C++ main file:

```c
 // your input message
 const unsigned char *input = 
         (const unsigned char *) "The random message to hash";

// you can choose any output length
int OUTPUT_LENGTH = 64;

 unsigned char output[OUTPUT_LENGTH];

 int result = SHAKE128(output, OUTPUT_LENGTH, input, strlen((const char *) input));

 // returning 0 means success
 assert(result == 0);

 // printing the hash in hexadecimal format
 for (int i = 0; i < OUTPUT_LENGTH; i++)
    printf("\\x%02x", output[i]);
 
 printf("\n");
```

For more information on how to use the FIPS 202 functions, see the `SimpleFIPS202.h` header file.

## TurboSHAKE
`TurboSHAKE` is a family of fast and secure XOFs. These are just like the SHAKE functions of FIPS 202, but with the 
Keccak-p permutation reduced to 12 rounds (instead of 24), so about twice faster. 
They are based on the `Sponge construction`, and the `Keccak-p[1600, 12]` permutation.

[//]: # (TODO: add example)

## KangarooTwelve
`KangarooTwelve`, or `K12`, is a family of XOFs, based on the `TurboSHAKE128`, hence it also uses the `Keccak-p[1600, 12]` permutation.
On high-end platforms, it can exploit a high degree of parallelism, whether using multiple cores or the SIMD instruction set of modern processors.

[//]: # (TODO: add example)

# {{Other types of functions}}

...