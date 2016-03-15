/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"
#include "KeccakCodePackage.h"

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

#define bufferSize 45696

int processFile(const char *fileName, unsigned int rate, unsigned int capacity, unsigned int hashbitlen, unsigned char delimitedSuffix, int base64)
{
    FILE *fp;
    Keccak_HashInstance keccak;
    size_t read;
    unsigned char buffer[bufferSize];
    char display[bufferSize*2+1];

    if (hashbitlen > (bufferSize*8)) {
        printf("The requested digest length (%d bits) does not fit in the buffer.\n", hashbitlen);
        return -1;
    }
    fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("File '%s' could not be opened.\n", fileName);
        return -1;
    }
    if (Keccak_HashInitialize(&keccak, rate, capacity, hashbitlen, delimitedSuffix)) {
        printf("Incorrect Keccak parameters (%d, %d, %d, 0x%02x).\n", rate, capacity, hashbitlen, delimitedSuffix);
        fclose(fp);
        return -1;
    }
    do {
        read = fread(buffer, 1, bufferSize, fp);
        if (read > 0)
            Keccak_HashUpdate(&keccak, buffer, read*8);
    } while(read>0);
    fclose(fp);
    Keccak_HashFinal(&keccak, buffer);
    if (base64) {
        if (base64encode(buffer, (hashbitlen+7)/8, display, bufferSize*2)) {
            printf("Error while converting to base64.\n");
            return -1;
        }
    }
    else {
        if (hexencode(buffer, (hashbitlen+7)/8, display, bufferSize*2)) {
            printf("Error while converting to hex.\n");
            return -1;
        }
    }
    printf(display);
    printf("  ");
    printf(fileName);
    printf("\n");
    return 0;
}

int process(int argc, char* argv[])
{
    unsigned int rate = 1344;
    unsigned int capacity = 256;
    unsigned int hashbitlen = 264;
    unsigned char delimitedSuffix = 0x1F;
    int base64 = 1;
    int i, r;

    for(i=1; i<argc; i++) {
        if (strcmp("--base64", argv[i]) == 0)
            base64 = 1;
        else if (strcmp("--hex", argv[i]) == 0)
            base64 = 0;
        else if (strcmp("--shake128", argv[i]) == 0) {
            rate = 1344;
            capacity = 256;
            hashbitlen = 264;
            delimitedSuffix = 0x1F;
            base64 = 1;
        }
        else if (strcmp("--shake256", argv[i]) == 0) {
            rate = 1088;
            capacity = 512;
            hashbitlen = 528;
            delimitedSuffix = 0x1F;
            base64 = 1;
        }
        else if (strcmp("--sha3-224", argv[i]) == 0) {
            rate = 1152;
            capacity = 448;
            hashbitlen = 224;
            delimitedSuffix = 0x06;
            base64 = 0;
        }
        else if (strcmp("--sha3-256", argv[i]) == 0) {
            rate = 1088;
            capacity = 512;
            hashbitlen = 256;
            delimitedSuffix = 0x06;
            base64 = 0;
        }
        else if (strcmp("--sha3-384", argv[i]) == 0) {
            rate = 832;
            capacity = 768;
            hashbitlen = 384;
            delimitedSuffix = 0x06;
            base64 = 0;
        }
        else if (strcmp("--sha3-512", argv[i]) == 0) {
            rate = 576;
            capacity = 1024;
            hashbitlen = 512;
            delimitedSuffix = 0x06;
            base64 = 0;
        }
        else {
            if (strlen(argv[i]) > 2) {
                if ((argv[i][0] == '-') && (argv[i][1] == '-')) {
                    printf("Unrecognized command '%s'\n", argv[i]);
                    return -1;
                }
            }
            r = processFile(argv[i], rate, capacity, hashbitlen, delimitedSuffix, base64);
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
