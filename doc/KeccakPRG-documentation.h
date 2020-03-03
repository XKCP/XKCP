/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Keccak, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by the designers, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

/** General information
  *
  * The following functions implement a pseudo-random bit generator based on Keccak.
  * More specifically, they instantiate the SpongePRG mode, published in our SAC 2011 paper,
  * with Keccak. (https://keccak.team/files/SpongeDuplex.pdf)
  *
  * For the 128-bit security strength, we recommend SpongePRG on top of Keccak-f[1600]
  * with a capacity of 254 bits. If a smaller footprint is required, we recommend
  * SpongePRG on top of Keccak-f[800] again with a capacity of 254 bits.
  *
  * The following type and functions are not actually implemented. Their
  * documentation is generic, with the prefix Prefix replaced by
  * - KeccakWidth200 for a SpongePRG object based on Keccak-f[200]
  * - KeccakWidth400 for a SpongePRG object based on Keccak-f[400]
  * - KeccakWidth800 for a SpongePRG object based on Keccak-f[800]
  * - KeccakWidth1600 for a SpongePRG object based on Keccak-f[1600]
  *
  * In all these functions, the rate and capacity must sum to the width of the
  * chosen permutation. For instance, to use the SpongePRG object with
  * Keccak[r=1346, c=254], one must use the KeccakWidth1600_SpongePRG* functions.
  *
  * The Prefix_SpongePRG_Instance contains the SpongePRG instance attributes for use
  * with the Prefix_SpongePRG* functions.
  * It gathers the state processed by the permutation as well as the rate,
  * the position of input/output bytes in the state in case of partial
  * input or output.
  */

#ifdef DontReallyInclude_DocumentationOnly
/**
  * Structure that contains the SpongePRG instance for use with the
  * Prefix_SpongePRG* functions.
  * It gathers the state processed by the permutation as well as
  * the rate.
  */
typedef struct Prefix_SpongePRG_InstanceStruct {
    /** The underlying duplex construction. */
    Prefix_DuplexInstance duplex;
} Prefix_SpongePRG_Instance;

/**
  * Function to initialize a SpongePRG object SpongePRG[Keccak-f[r+c], pad10*1, r, ρ].
  * The user specifies the security strength via the capacity c, while the block size ρ
  * and the rate r are computed as follows:
  *     - The rate is set to r=b-r, with b the width of the chosen
  *         permutation selected via the Prefix.
  *     - The block size ρ is set to 8*floor((r-2)/8) bits.
  * For instance, to initialize SpongePRG on top of Keccak-f[1600] with c=254 bits,
  * one should call KeccakWidth1600_SpongePRG_Initialize(&instance, 254) and
  * the block size is ρ=1344 bits or 168 bytes.
  * Similarly, to initialize SpongePRG on top of Keccak-f[800] with c=254 bits,
  * one should call KeccakWidth800_SpongePRG_Initialize(&instance, 254) and
  * the block size is ρ=544 bits or 68 bytes.
  * @param  instance        Pointer to the SpongePRG instance to be initialized.
  * @param  capacity        Value of the capacity c (in bits).
  * @pre    0 ≤ @a capacity ≤ b-10, and otherwise the value of the capacity is unrestricted.
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_SpongePRG_Initialize(Prefix_SpongePRG_Instance *instance, unsigned int capacity);

/**
  * Function to feed the generator with an input seed, which will influence
  * all further outputs.
  * @param  instance        Pointer to the SpongePRG instance initialized
  *                         by Prefix_SpongePRG_Initialize().
  * @param  input           Pointer to the bytes to queue.
  * @param  inputByteLen    The number of input bytes provided in @a input.
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_SpongePRG_Feed(Prefix_SpongePRG_Instance *instance, const unsigned char *input, unsigned int inputByteLen);

/**
  * Function to fetch output pseudo-random bytes based on all previously fed seeds.
  * Pseudo-random output should not be fetched before seeds with sufficient
  * entropy has been fed.
  * @param  instance        Pointer to the SpongePRG instance initialized
  *                         by Prefix_SpongePRG_Initialize().
  * @param  output          Pointer to the buffer where to store the output.
  * @param  outputByteLen   The number of output bytes desired.
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_SpongePRG_Fetch(Prefix_SpongePRG_Instance *instance, unsigned char *out, unsigned int outByteLen);

/**
  * Function to ensure irreversibility.
  * This function requires that ρ≥c. If so, its purpose is to guarantee that even
  * if the value of the state is leaked, an adversary cannot compute outputs
  * prior to calling this function.
  * @param  instance        Pointer to the SpongePRG instance initialized
  *                         by Prefix_SpongePRG_Initialize().
  * @pre The instance must satisfy ρ≥c, ortherwise the function returns an error.
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_SpongePRG_Forget(Prefix_SpongePRG_Instance *instance);
#endif
