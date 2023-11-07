In this guide, we will provide examples of using the high-level API, 
with less emphasis on the low-level implementation. When working with XKCP, 
the low-level implementation is abstracted from the user.

# Hashing

## FIPS 202
Those are the NIST approved SHA-3 functions. They are based on the sponge construction, and the Keccak-p permutation.
The functions are:

### Hash functions
- `SHA3_224`
- `SHA3_256`
- `SHA3_384`
- `SHA3_512`

To use any of them in your C/C++ project, follow the steps below:
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
5. In your C/C++ main file, include the header `#include "FIPS202.a.headers/SimpleFIPS202.h"`
6. Use the functions in your code! In the following example, we will use `SHA3_256`:
    ```c
    // your input message
    const unsigned char *input = 
            (const unsigned char *) "The random message to hash";

    // the output that will contain the hash
    unsigned char output[32];

    int result = SHA3_256(output, input, (const char *) input));

    // returning 0 means success
    assert(result == 0);

    // printing the hash in hexadecimal format
    for (int i = 0; i < 32; i++) {
        printf("%02x", output[i]);
    }
    ```
7. Before executing the code, you have to compile the code with the static library `FIPS202.a`. 
When using `gcc`, this step might look like this:
    ```bash
    gcc main.c FIPS202_reference.a;
    ```
8. Run the executable:
    ```bash
    ./a.out
    ```

### Extendable output functions (XOFs)
- `SHAKE128`
- `SHAKE256`

[//]: # (TODO: add example for XOFs)