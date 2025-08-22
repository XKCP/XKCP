/*
Implementation by Gilles Van Assche and Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"
#include "config.h"
#ifdef XKCP_has_Sponge_Keccak
#include "KeccakHash.h"
#endif
#ifdef XKCP_has_KangarooTwelve
#include "KangarooTwelve.h"
#endif
#ifdef XKCP_has_SP800_185
#include "SP800-185.h"
#endif
#ifdef XKCP_has_TurboSHAKE
#include "TurboSHAKE.h"
#endif

int hexencode(const void* data_buf, size_t dataLength, char* result, size_t resultSize)
{
   const char hexchars[] = "0123456789abcdef";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t resultIndex = 0;
   size_t x;

    for(x=0; x<dataLength; x++) {
        if (resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
        result[resultIndex++] = hexchars[(data[x] / 16) % 16];
        if (resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
        result[resultIndex++] = hexchars[data[x] % 16];
   }
   if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
   result[resultIndex] = 0;
   return 0;   /* indicate success */
}

#define bufferSize 65536
#define algorithm_Keccak    0
#define algorithm_K12       1
#define algorithm_ParallelHash  2
#define algorithm_TurboSHAKE    3

typedef struct {
    unsigned int algorithm;
    unsigned int rate;
    unsigned int capacity;
    unsigned int hashbitlen;
    unsigned char delimitedSuffix;
    unsigned int blockByteLen;
} Specifications;

typedef union {
#ifdef XKCP_has_Sponge_Keccak
    Keccak_HashInstance keccak;
#endif
#ifdef XKCP_has_KangarooTwelve
    KangarooTwelve_Instance k12;
#endif
#ifdef XKCP_has_SP800_185
    ParallelHash_Instance ph;
#endif
#ifdef XKCP_has_TurboSHAKE
    TurboSHAKE_Instance ts;
#endif
} Instance;

int hashInitialize(Instance *instance, const Specifications *specs)
{
#ifdef XKCP_has_Sponge_Keccak
    if (specs->algorithm == algorithm_Keccak) {
        if (Keccak_HashInitialize(&instance->keccak, specs->rate, specs->capacity, specs->hashbitlen, specs->delimitedSuffix)) {
            printf("Incorrect Keccak parameters (%d, %d, %d, 0x%02x).\n", specs->rate, specs->capacity, specs->hashbitlen, specs->delimitedSuffix);
            return -1;
        }
        else
            return 0;
    }
    else
#endif
#ifdef XKCP_has_KangarooTwelve
    if (specs->algorithm == algorithm_K12) {
        if (KangarooTwelve_Initialize(&instance->k12, specs->capacity/2, specs->hashbitlen/8)) {
            printf("Incorrect KangarooTwelve parameters.\n");
            return -1;
        }
        else
            return 0;
    }
    else
#endif
#ifdef XKCP_has_SP800_185
    if (specs->algorithm == algorithm_ParallelHash) {
        if (specs->capacity == 256) {
            if (ParallelHash128_Initialize(&instance->ph, specs->blockByteLen, specs->hashbitlen, "", 0)) {
                printf("Incorrect ParallelHash128 parameters.\n");
                return -1;
            }
            else
                return 0;
        }
       else if (specs->capacity == 512) {
            if (ParallelHash256_Initialize(&instance->ph, specs->blockByteLen, specs->hashbitlen, "", 0)) {
                printf("Incorrect ParallelHash256 parameters.\n");
                return -1;
            }
            else
                return 0;
        }
        else {
            printf("Incorrect ParallelHash parameters.\n");
            return -1;
        }
    }
    else
#endif
#ifdef XKCP_has_TurboSHAKE
    if (specs->algorithm == algorithm_TurboSHAKE) {
        if (TurboSHAKE_Initialize(&instance->ts, specs->capacity)) {
            printf("Incorrect TurboSHAKE parameters.\n");
            return -1;
        }
        else
            return 0;
    }
    else
#endif
        return -1;
}

int hashUpdate(Instance *instance, const Specifications *specs, const unsigned char *buffer, size_t size)
{
#ifdef XKCP_has_Sponge_Keccak
    if (specs->algorithm == algorithm_Keccak)
        return Keccak_HashUpdate(&instance->keccak, buffer, size*8);
    else
#endif
#ifdef XKCP_has_KangarooTwelve
    if (specs->algorithm == algorithm_K12)
        return KangarooTwelve_Update(&instance->k12, buffer, size);
    else
#endif
#ifdef XKCP_has_SP800_185
    if ((specs->algorithm == algorithm_ParallelHash) && (specs->capacity == 256))
        return ParallelHash128_Update(&instance->ph, buffer, size*8);
    else
    if ((specs->algorithm == algorithm_ParallelHash) && (specs->capacity == 512))
        return ParallelHash256_Update(&instance->ph, buffer, size*8);
    else
#endif
#ifdef XKCP_has_TurboSHAKE
    if (specs->algorithm == algorithm_TurboSHAKE)
        return TurboSHAKE_Absorb(&instance->ts, buffer, size);
    else
#endif
        return -1;
}

int hashFinal(Instance *instance, const Specifications *specs, unsigned char *buffer)
{
#ifdef XKCP_has_Sponge_Keccak
    if (specs->algorithm == algorithm_Keccak)
        return Keccak_HashFinal(&instance->keccak, buffer);
    else
#endif
#ifdef XKCP_has_KangarooTwelve
    if (specs->algorithm == algorithm_K12)
        return KangarooTwelve_Final(&instance->k12, buffer, "", 0);
    else
#endif
#ifdef XKCP_has_SP800_185
    if ((specs->algorithm == algorithm_ParallelHash) && (specs->capacity == 256))
        return ParallelHash128_Final(&instance->ph, buffer);
    else
    if ((specs->algorithm == algorithm_ParallelHash) && (specs->capacity == 512))
        return ParallelHash256_Final(&instance->ph, buffer);
    else
#endif
#ifdef XKCP_has_TurboSHAKE
    if (specs->algorithm == algorithm_TurboSHAKE) {
        int code = TurboSHAKE_AbsorbDomainSeparationByte(&instance->ts, specs->delimitedSuffix);
        code |= TurboSHAKE_Squeeze(&instance->ts, buffer, (specs->hashbitlen + 7)/8);
        return code;
    }
    else
#endif
        return -1;
}

int processFile(const char *fileName, const Specifications *specs, int base64)
{
    FILE *fp;
    Instance instance;
    size_t read;
    unsigned char buffer[bufferSize];
    char display[bufferSize*2+1];

    if (specs->hashbitlen > (bufferSize*8)) {
        printf("The requested digest length (%d bits) does not fit in the buffer.\n", specs->hashbitlen);
        return -1;
    }
    if (strcmp("-", fileName) == 0) {
        fp = stdin;
    }
    else {
        fp = fopen(fileName, "rb");
        if (fp == NULL) {
            printf("File '%s' could not be opened.\n", fileName);
            return -1;
        }
    }
    if (hashInitialize(&instance, specs)) {
        fclose(fp);
        return -1;
    }
    do {
        read = fread(buffer, 1, bufferSize, fp);
        if (read > 0)
            hashUpdate(&instance, specs, buffer, read);
    } while(read>0);
    if (fp != stdin) {
        fclose(fp);
    }
    hashFinal(&instance, specs, buffer);
    if (base64) {
        if (base64encode(buffer, (specs->hashbitlen+7)/8, display, bufferSize*2)) {
            printf("Error while converting to base64.\n");
            return -1;
        }
    }
    else {
        if (hexencode(buffer, (specs->hashbitlen+7)/8, display, bufferSize*2)) {
            printf("Error while converting to hex.\n");
            return -1;
        }
    }
    printf("%s  ", display);
    printf("%s\n", fileName);
    return 0;
}

int processString(const char *input, const Specifications *specs, int base64)
{
    Instance instance;
    unsigned char buffer[bufferSize];
    char display[bufferSize*2+1];

    if (specs->hashbitlen > (bufferSize*8)) {
        printf("The requested digest length (%d bits) does not fit in the buffer.\n", specs->hashbitlen);
        return -1;
    }
    if (hashInitialize(&instance, specs)) {
        return -1;
    }
    hashUpdate(&instance, specs, input, strlen(input));
    hashFinal(&instance, specs, buffer);
    if (base64) {
        if (base64encode(buffer, (specs->hashbitlen+7)/8, display, bufferSize*2)) {
            printf("Error while converting to base64.\n");
            return -1;
        }
    }
    else {
        if (hexencode(buffer, (specs->hashbitlen+7)/8, display, bufferSize*2)) {
            printf("Error while converting to hex.\n");
            return -1;
        }
    }
    printf("%s  ", display);
    printf("\"%s\"\n", input);
    return 0;
}

int processZeroes(size_t zeroes, const Specifications *specs, int base64)
{
    Instance instance;
    unsigned char buffer[bufferSize];
    char display[bufferSize*2+1];
    size_t left, length;

    if (specs->hashbitlen > (bufferSize*8)) {
        printf("The requested digest length (%d bits) does not fit in the buffer.\n", specs->hashbitlen);
        return -1;
    }
    if (hashInitialize(&instance, specs)) {
        return -1;
    }
    memset(buffer, 0, bufferSize);
    left = zeroes;
    while(left > 0) {
        length = (left <= bufferSize) ? left : bufferSize;
        hashUpdate(&instance, specs, buffer, length);
        left -= length;
    }
    hashFinal(&instance, specs, buffer);
    if (base64) {
        if (base64encode(buffer, (specs->hashbitlen+7)/8, display, bufferSize*2)) {
            printf("Error while converting to base64.\n");
            return -1;
        }
    }
    else {
        if (hexencode(buffer, (specs->hashbitlen+7)/8, display, bufferSize*2)) {
            printf("Error while converting to hex.\n");
            return -1;
        }
    }
    printf("%s  ", display);
    printf("`00`^%zu\n", zeroes);
    return 0;
}

void printHelp()
{
    printf("Usage: KeccakSum [options] [file names] [options] [file names] ...\n\n");
    printf("  --help or -h                To display this page\n");
    printf("\nInput options:\n");
    printf("  <file name>                 File to hash\n");
    printf("  -                           Standard input to hash\n");
    printf("  --string <string>           String to hash\n");
    printf("  --zeroes <integer>          All-zero string with the given number of bytes\n");
    printf("                              (for benchmarking purposes)\n");
    printf("\nOutput options:\n");
    printf("  --base64                    Print in base64\n");
    printf("  --hex                       Print in hexadecimal\n");
    printf("  --outputbits <integer>      Number of output bits\n");
    printf("\nHashes and XOFs (roughly from fastest to slowest):\n");
#ifdef XKCP_has_KangarooTwelve
    printf("  --kangarootwelve or --k12   KT128\n");
    printf("  --kt128                     KT128\n");
    printf("  --kt256                     KT256\n");
#endif
#ifdef XKCP_has_SP800_185
    printf("  --ph128                     ParallelHash128\n");
    printf("  --ph256                     ParallelHash256\n");
    printf("   -B <block size in bytes>   ParallelHash's B parameter\n");
    printf("                              (default is 8192 bytes)\n");
#endif
#ifdef XKCP_has_TurboSHAKE
    printf("  --turboshake128 or --ts128  TurboSHAKE128\n");
    printf("  --turboshake256 or --ts256  TurboSHAKE256\n");
#endif
#ifdef XKCP_has_Sponge_Keccak
    printf("  --shake128                  SHAKE128\n");
    printf("  --shake256                  SHAKE256\n");
    printf("  --sha3-224                  SHA3-224\n");
    printf("  --sha3-256                  SHA3-256\n");
    printf("  --sha3-384                  SHA3-384\n");
    printf("  --sha3-512                  SHA3-512\n");
    printf("  --no-suffix                 Use 0x01 as delimited suffix (pure Keccak)\n");
    printf("  --ethereum                  Equivalent to --sha3-256 --no-suffix\n");
#endif
    printf("\n");
    printf("The options are processed in order.\n");
#ifdef XKCP_has_Sponge_Keccak
    printf("By default, it uses SHAKE128 and base64 display.\n");
#elifdef XKCP_has_KangarooTwelve
    printf("By default, it uses KT128 and base64 display.\n");
#else
#error "No default algorithm defined."
#endif
    printf("With no file names and no strings, it reads the standard input.\n");
}

int process(int argc, char* argv[])
{
    Specifications specs;
    int base64 = 1;
    int i, r;
    int was_filename_or_string = 0;
#ifdef XKCP_has_Sponge_Keccak
    specs.algorithm = algorithm_Keccak;
    specs.rate = 1344;
    specs.capacity = 256;
    specs.delimitedSuffix = 0x1F;
#elifdef XKCP_has_KangarooTwelve
    specs.algorithm = algorithm_K12;
    specs.capacity = 256;
#else
#error "No default algorithm defined."
#endif
    specs.hashbitlen = (base64 ? 264 : 256);

    for(i=1; i<argc; i++) {
        int outputbits;
        if (strcmp("--base64", argv[i]) == 0)
            base64 = 1;
        else if (strcmp("--hex", argv[i]) == 0)
            base64 = 0;
        else if ((strcmp("--outputbits", argv[i]) == 0) || (strcmp("-n", argv[i]) == 0)) {
            if ((i+1) >= argc) {
                printf("Error: missing argument for --outputbits\n");
                return -1;
            }
            outputbits = 0;
            if (sscanf(argv[i+1], "%d", &outputbits) && (outputbits > 0) && ((outputbits % 8) == 0)) {
                specs.hashbitlen = outputbits;
                i++;
            }
            else {
                printf("Error: argument for --outputbits option must be a positive integer multiple of 8\n");
                return -1;
            }
        }
#ifdef XKCP_has_Sponge_Keccak
        else if (strcmp("--shake128", argv[i]) == 0) {
            specs.rate = 1344;
            specs.capacity = 256;
            specs.hashbitlen = (base64 ? 264 : 256);
            specs.delimitedSuffix = 0x1F;
        }
        else if (strcmp("--shake256", argv[i]) == 0) {
            specs.rate = 1088;
            specs.capacity = 512;
            specs.hashbitlen = (base64 ? 528 : 512);
            specs.delimitedSuffix = 0x1F;
        }
#endif
#ifdef XKCP_has_TurboSHAKE
        else if ((strcmp("--turboshake128", argv[i]) == 0) || (strcmp("--ts128", argv[i]) == 0)) {
            specs.algorithm = algorithm_TurboSHAKE;
            specs.capacity = 256;
            specs.hashbitlen = (base64 ? 264 : 256);
            specs.delimitedSuffix = 0x1F;
        }
        else if ((strcmp("--turboshake256", argv[i]) == 0) || (strcmp("--ts256", argv[i]) == 0)) {
            specs.algorithm = algorithm_TurboSHAKE;
            specs.capacity = 512;
            specs.hashbitlen = (base64 ? 528 : 512);
            specs.delimitedSuffix = 0x1F;
        }
#endif
#ifdef XKCP_has_Sponge_Keccak
        else if (strcmp("--sha3-224", argv[i]) == 0) {
            specs.rate = 1152;
            specs.capacity = 448;
            specs.hashbitlen = 224;
            specs.delimitedSuffix = 0x06;
        }
        else if (strcmp("--sha3-256", argv[i]) == 0) {
            specs.rate = 1088;
            specs.capacity = 512;
            specs.hashbitlen = 256;
            specs.delimitedSuffix = 0x06;
        }
        else if (strcmp("--sha3-384", argv[i]) == 0) {
            specs.rate = 832;
            specs.capacity = 768;
            specs.hashbitlen = 384;
            specs.delimitedSuffix = 0x06;
        }
        else if (strcmp("--sha3-512", argv[i]) == 0) {
            specs.rate = 576;
            specs.capacity = 1024;
            specs.hashbitlen = 512;
            specs.delimitedSuffix = 0x06;
        }
        else if (strcmp("--no-suffix", argv[i]) == 0) {
            specs.delimitedSuffix = 0x01;
        }
        else if (strcmp("--ethereum", argv[i]) == 0) {
            specs.rate = 1088;
            specs.capacity = 512;
            specs.hashbitlen = 256;
            specs.delimitedSuffix = 0x01;
        }
#endif
#ifdef XKCP_has_KangarooTwelve
        else if ((strcmp("--kangarootwelve", argv[i]) == 0) || (strcmp("--k12", argv[i]) == 0) || (strcmp("--kt128", argv[i]) == 0)) {
            specs.algorithm = algorithm_K12;
            specs.capacity = 256;
            specs.hashbitlen = (base64 ? 264 : 256);
        }
        else if (strcmp("--kt256", argv[i]) == 0) {
            specs.algorithm = algorithm_K12;
            specs.capacity = 512;
            specs.hashbitlen = (base64 ? 528 : 512);
        }
#endif
#ifdef XKCP_has_SP800_185
        else if (strcmp("--ph128", argv[i]) == 0) {
            specs.algorithm = algorithm_ParallelHash;
            specs.capacity = 256;
            specs.hashbitlen = (base64 ? 264 : 256);
            specs.blockByteLen = 8192;
        }
        else if (strcmp("--ph256", argv[i]) == 0) {
            specs.algorithm = algorithm_ParallelHash;
            specs.capacity = 512;
            specs.hashbitlen = (base64 ? 528 : 512);
            specs.blockByteLen = 8192;
        }
        else if (strcmp("-B", argv[i]) == 0) {
            int blockByteLen;
            if ((i+1) >= argc) {
                printf("Error: missing argument for -B\n");
                return -1;
            }
            blockByteLen = 0;
            if (sscanf(argv[i+1], "%d", &blockByteLen) && (blockByteLen > 0)) {
                specs.blockByteLen = blockByteLen;
                i++;
            }
            else {
                printf("Error: argument for -B option must be a positive power of 2\n");
                return -1;
            }
        }
#endif
        else if (strcmp("--string", argv[i]) == 0) {
            if ((i+1) >= argc) {
                printf("Error: missing argument for --string\n");
                return -1;
            }
            i++;
            was_filename_or_string = 1;
            processString(argv[i], &specs, base64);
        }
        else if (strcmp("--help", argv[i]) == 0 || strcmp("-h", argv[i]) == 0) {
            printHelp();
            return 0;
        }
        else if (strcmp("--zeroes", argv[i]) == 0) {
            if ((i+1) >= argc) {
                printf("Error: missing argument for --zeroes\n");
                return -1;
            }
            size_t zeroes = 0;
            if (sscanf(argv[i+1], "%zu", &zeroes)) {
                i++;
            }
            else {
                printf("Error: incorrect argument for --zeroes\n");
                return -1;
            }
            was_filename_or_string = 1;
            processZeroes(zeroes, &specs, base64);
        }
        else if (strcmp("--help", argv[i]) == 0 || strcmp("-h", argv[i]) == 0) {
            printHelp();
            return 0;
        }
        else {
            if (strlen(argv[i]) > 2) {
                if ((argv[i][0] == '-') && (argv[i][1] == '-')) {
                    printf("Unrecognized option '%s'\n", argv[i]);
                    return -1;
                }
            }
            was_filename_or_string = 1;
            r = processFile(argv[i], &specs, base64);
            if (r)
                return r;
        }
    }
    if (was_filename_or_string == 0) {
        r = processFile("-", &specs, base64);
        if (r)
            return r;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    return process(argc, argv);
}
