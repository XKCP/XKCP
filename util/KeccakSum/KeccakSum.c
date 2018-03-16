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
#include "KeccakHash.h"
#include "KangarooTwelve.h"
#include "SP800-185.h"

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

typedef struct {
    unsigned int algorithm;
    unsigned int rate;
    unsigned int capacity;
    unsigned int hashbitlen;
    unsigned char delimitedSuffix;
} Specifications;

typedef union {
    Keccak_HashInstance keccak;
    KangarooTwelve_Instance k12;
} Instance;

int hashInitialize(Instance *instance, const Specifications *specs)
{
    if (specs->algorithm == algorithm_Keccak) {
        if (Keccak_HashInitialize(&instance->keccak, specs->rate, specs->capacity, specs->hashbitlen, specs->delimitedSuffix)) {
            printf("Incorrect Keccak parameters (%d, %d, %d, 0x%02x).\n", specs->rate, specs->capacity, specs->hashbitlen, specs->delimitedSuffix);
            return -1;
        }
        else
            return 0;
    }
    else if (specs->algorithm == algorithm_K12) {
        if (KangarooTwelve_Initialize(&instance->k12, specs->hashbitlen/8)) {
            printf("Incorrect KangarooTwelve parameters.\n");
            return -1;
        }
        else
            return 0;
    }
    else
        return -1;
}

int hashUpdate(Instance *instance, const Specifications *specs, const unsigned char *buffer, size_t size)
{
    if (specs->algorithm == algorithm_Keccak)
        Keccak_HashUpdate(&instance->keccak, buffer, size*8);
    else if (specs->algorithm == algorithm_K12)
        KangarooTwelve_Update(&instance->k12, buffer, size);
    return 0;
}

int hashFinal(Instance *instance, const Specifications *specs, unsigned char *buffer)
{
    if (specs->algorithm == algorithm_Keccak)
        Keccak_HashFinal(&instance->keccak, buffer);
    else if (specs->algorithm == algorithm_K12)
        KangarooTwelve_Final(&instance->k12, buffer, "", 0);
    return 0;
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
    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        printf("File '%s' could not be opened.\n", fileName);
        return -1;
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
    fclose(fp);
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

void printHelp()
{
    printf("Usage: KeccakSum [options] [file names] [options] [file names] ...\n\n");
    printf("  --help or -h                To display this page\n");
    printf("  --base64                    Print in base64\n");
    printf("  --hex                       Print in hexadecimal\n");
    printf("  --outputbits <numberOfBits> Number of bits as output\n");
    printf("  --shake128                  Use SHAKE128\n");
    printf("  --shake256                  Use SHAKE256\n");
    printf("  --sha3-224                  Use SHA3-224\n");
    printf("  --sha3-256                  Use SHA3-256\n");
    printf("  --sha3-384                  Use SHA3-384\n");
    printf("  --sha3-512                  Use SHA3-512\n");
    printf("  --no-suffix                 Use 0x01 as delimited suffix (pure Keccak)\n");
    printf("  --ethereum                  Equivalent to --sha3-256 --no-suffix\n");
    printf("  --kangarootwelve or --k12   Use KangarooTwelve\n");
    printf("  --string <string>           String to hash\n");
    printf("  <file name>                 File to hash\n\n");
    printf("The options are processed in order.\nBy default, it uses SHAKE128 and base64 display.\n");
}

int process(int argc, char* argv[])
{
    Specifications specs;
    int base64 = 1;
    int i, r;
    specs.algorithm = algorithm_Keccak;
    specs.rate = 1344;
    specs.capacity = 256;
    specs.hashbitlen = 264;
    specs.delimitedSuffix = 0x1F;

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
        else if (strcmp("--shake128", argv[i]) == 0) {
            specs.rate = 1344;
            specs.capacity = 256;
            specs.hashbitlen = 264;
            specs.delimitedSuffix = 0x1F;
            base64 = 1;
        }
        else if (strcmp("--shake256", argv[i]) == 0) {
            specs.rate = 1088;
            specs.capacity = 512;
            specs.hashbitlen = 528;
            specs.delimitedSuffix = 0x1F;
            base64 = 1;
        }
        else if (strcmp("--sha3-224", argv[i]) == 0) {
            specs.rate = 1152;
            specs.capacity = 448;
            specs.hashbitlen = 224;
            specs.delimitedSuffix = 0x06;
            base64 = 0;
        }
        else if (strcmp("--sha3-256", argv[i]) == 0) {
            specs.rate = 1088;
            specs.capacity = 512;
            specs.hashbitlen = 256;
            specs.delimitedSuffix = 0x06;
            base64 = 0;
        }
        else if (strcmp("--sha3-384", argv[i]) == 0) {
            specs.rate = 832;
            specs.capacity = 768;
            specs.hashbitlen = 384;
            specs.delimitedSuffix = 0x06;
            base64 = 0;
        }
        else if (strcmp("--sha3-512", argv[i]) == 0) {
            specs.rate = 576;
            specs.capacity = 1024;
            specs.hashbitlen = 512;
            specs.delimitedSuffix = 0x06;
            base64 = 0;
        }
        else if (strcmp("--no-suffix", argv[i]) == 0) {
            specs.delimitedSuffix = 0x01;
        }
        else if (strcmp("--ethereum", argv[i]) == 0) {
            specs.rate = 1088;
            specs.capacity = 512;
            specs.hashbitlen = 256;
            specs.delimitedSuffix = 0x01;
            base64 = 0;
        }
        else if ((strcmp("--kangarootwelve", argv[i]) == 0) || (strcmp("--k12", argv[i]) == 0)) {
            specs.algorithm = algorithm_K12;
            specs.hashbitlen = 264;
            base64 = 1;
        }
        else if (strcmp("--string", argv[i]) == 0) {
            if ((i+1) >= argc) {
                printf("Error: missing argument for --string\n");
                return -1;
            }
            i++;
            processString(argv[i], &specs, base64);
        }
        else if (strcmp("--help", argv[i]) == 0 || strcmp("-h", argv[i]) == 0) {
            printHelp();
            return 0;
        }
        else {
            if (strlen(argv[i]) > 2) {
                if ((argv[i][0] == '-') && (argv[i][1] == '-')) {
                    printf("Unrecognized command '%s'\n", argv[i]);
                    return -1;
                }
            }
            r = processFile(argv[i], &specs, base64);
            if (r)
                return r;
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    return process(argc, argv);
}
