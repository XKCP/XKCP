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

#include <string.h>
#include "KeccakDuplex.h"
#include "Keyak.h"

#ifdef PlSnP_P
#include "KeccakParallelDuplex.h"
#endif

#define Keyak_Phase_FeedingAssociatedData   0x01
#define Keyak_Phase_Wrapping                0x02
#define Keyak_Phase_Unwrapping              0x04

#if ((!defined(PlSnP_P)) || (PlSnP_P == 1))
int Keyak_Initialize(Keccak_DuplexInstance* duplex, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce)
{
    unsigned char smallData[2];

    if (keySizeInBits > 224)
        return 1;

    // Key pack: 0x1E, padded key to fit on 29 bytes
    smallData[0] = 0x1E;
    Keccak_DuplexingFeedPartialInput(duplex, smallData, 1);
    Keccak_DuplexingFeedPartialInput(duplex, key, keySizeInBits/8);
    if ((keySizeInBits % 8) == 0)
        smallData[0] = 0x01;
    else {
        unsigned char padding = (unsigned char)1 << (keySizeInBits%8);
        unsigned char mask = padding-1;
        smallData[0] = (key[keySizeInBits/8] & mask) | padding;
    }
    Keccak_DuplexingFeedPartialInput(duplex, smallData, 1);
    Keccak_DuplexingFeedZeroes(duplex, 0x1E - keySizeInBits/8 - 2);

    // 0x01 0x00
    smallData[0] = 0x01;
    smallData[1] = 0x00;
    Keccak_DuplexingFeedPartialInput(duplex, smallData, 2);

    // Nonce (16 bytes)
    Keccak_DuplexingFeedPartialInput(duplex, nonce, 16);

    return 0;
}
#else
int Keyak_Initialize(Keccak_ParallelDuplexInstances* duplex, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce)
{
    unsigned char data[48];
    unsigned int i;

    if (keySizeInBits > 224)
        return 1;

    memset(data, 0, sizeof(data));
    // Key pack: 0x1E, padded key to fit on 29 bytes
    data[0] = 0x1E;
    memcpy(data+1, key, keySizeInBits/8);
    if ((keySizeInBits % 8) == 0)
        data[keySizeInBits/8+1] = 0x01;
    else {
        unsigned char padding = (unsigned char)1 << (keySizeInBits%8);
        unsigned char mask = padding-1;
        data[keySizeInBits/8+1] = (key[keySizeInBits/8] & mask) | padding;
    }
    data[30] = PlSnP_P;
    memcpy(data+32, nonce, 16);
    for(i=0; i<PlSnP_P; i++) {
        data[31] = i;
        Keccak_ParallelDuplexingFeedPartialSingleInput(duplex, i, data, 48);
    }

    Keccak_ParallelDuplexingAll(duplex, 0x05); // 10

    return 0;
}
#endif

#if ((!defined(PlSnP_P)) || (PlSnP_P == 1))
int RiverKeyak_Initialize(Keyak_Instance *instance, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce)
{
    int result = Keccak_DuplexInitialize(&(instance->duplex), 548, 252);
    if (result != 0)
        return result;
    instance->phase = Keyak_Phase_FeedingAssociatedData;
    return Keyak_Initialize(&(instance->duplex), key, keySizeInBits, nonce);
}

int LakeKeyak_Initialize(Keyak_Instance *instance, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce)
{
    int result = Keccak_DuplexInitialize(&(instance->duplex), 1348, 252);
    if (result != 0)
        return result;
    instance->phase = Keyak_Phase_FeedingAssociatedData;
    return Keyak_Initialize(&(instance->duplex), key, keySizeInBits, nonce);
}
#else
int ParallelKeyak_Initialize(Keyak_Instance *instance, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce)
{
    int result = Keccak_ParallelDuplexInitializeAll(&(instance->duplex), 1348, 252);
    if (result != 0)
        return result;
    instance->phase = Keyak_Phase_FeedingAssociatedData;
    return Keyak_Initialize(&(instance->duplex), key, keySizeInBits, nonce);
}
#endif

#if ((!defined(PlSnP_P)) || (PlSnP_P == 1))
void Keyak_ProcessAssociatedData(Keccak_DuplexInstance* duplex, const unsigned char *data, size_t dataSizeInBytes, int last, int bodyFollows)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;

    while(dataSizeInBytes > 0) {
        if (Keccak_DuplexGetInputIndex(duplex) == rhoInBytes)
            Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x04); // 00

        if ((rhoInBytes%SnP_laneLengthInBytes == 0) && (Keccak_DuplexGetInputIndex(duplex) == 0) && (dataSizeInBytes > 2*rhoInBytes)) {
            size_t localSize = Keccak_DuplexingFBWLAbsorb(duplex, data, dataSizeInBytes-rhoInBytes, 0x04); // 00
            data += localSize;
            dataSizeInBytes -= localSize;
        }
        else {
            unsigned int localSize;

            if (dataSizeInBytes > (rhoInBytes - Keccak_DuplexGetInputIndex(duplex)))
                localSize = rhoInBytes - Keccak_DuplexGetInputIndex(duplex);
            else
                localSize = dataSizeInBytes;

            Keccak_DuplexingFeedPartialInput(duplex, data, localSize);

            data += localSize;
            dataSizeInBytes -= localSize;
        }
    }

    if (last) {
        if (bodyFollows)
            Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x06); // 01
        else
            Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x05); // 10
    }
}
#else
void Keyak_ProcessAssociatedData(Keccak_ParallelDuplexInstances* duplex, const unsigned char *data, size_t dataSizeInBytes, int last, int bodyFollows)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;
    unsigned int totalByteInputIndex = Keccak_ParallelDuplexGetTotalInputIndex(duplex);

    while(dataSizeInBytes > 0) {
        if (totalByteInputIndex == rhoInBytes*PlSnP_P) {
            Keccak_ParallelDuplexingAll(duplex, 0x04); // 00
            totalByteInputIndex = 0;
        }

        if ((rhoInBytes%SnP_laneLengthInBytes == 0) && (totalByteInputIndex == 0) && (dataSizeInBytes > 2*rhoInBytes*PlSnP_P)) {
            size_t localSize = Keccak_ParallelDuplexingFBWLAbsorb(duplex, data, dataSizeInBytes-rhoInBytes*PlSnP_P, 0x04); // 00
            data += localSize;
            dataSizeInBytes -= localSize;
        }
        else {
            unsigned int localSize;
            if (dataSizeInBytes > (rhoInBytes*PlSnP_P - totalByteInputIndex))
                localSize = rhoInBytes*PlSnP_P - totalByteInputIndex;
            else
                localSize = dataSizeInBytes;

            Keccak_ParallelDuplexingFeedPartialInterleavedInput(duplex, data, localSize);
            data += localSize;
            dataSizeInBytes -= localSize;
            totalByteInputIndex += localSize;
        }
    }

    if (last) {
        if (bodyFollows)
            Keccak_ParallelDuplexingAll(duplex, 0x06); // 01
        else
            Keccak_ParallelDuplexingAll(duplex, 0x05); // 10
    }
}
#endif

int Keyak_FeedAssociatedData(Keyak_Instance *instance, const unsigned char *data, size_t dataSizeInBytes)
{
    if ((instance->phase & Keyak_Phase_FeedingAssociatedData) == 0)
        return 1;
    Keyak_ProcessAssociatedData(&(instance->duplex), data, dataSizeInBytes, 0, 0);
    instance->phase = Keyak_Phase_FeedingAssociatedData;

    return 0;
}

#if ((!defined(PlSnP_P)) || (PlSnP_P == 1))
void Keyak_ProcessPlaintext(Keccak_DuplexInstance* duplex, const unsigned char *plaintext, unsigned char *ciphertext, size_t dataSizeInBytes, int last)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;

    while(dataSizeInBytes > 0) {
        if (Keccak_DuplexGetInputIndex(duplex) == rhoInBytes)
            Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x07); // 11

        if ((rhoInBytes%SnP_laneLengthInBytes == 0) && (Keccak_DuplexGetInputIndex(duplex) == 0) && (dataSizeInBytes > 2*rhoInBytes)) {
            size_t localSize = Keccak_DuplexingFBWLWrap(duplex, plaintext, ciphertext, dataSizeInBytes-rhoInBytes, 0x07); // 11
            plaintext += localSize;
            ciphertext += localSize;
            dataSizeInBytes -= localSize;
        }
        else {
            unsigned int localSize;

            if (dataSizeInBytes > (rhoInBytes - Keccak_DuplexGetInputIndex(duplex)))
                localSize = rhoInBytes - Keccak_DuplexGetInputIndex(duplex);
            else
                localSize = dataSizeInBytes;

            if (plaintext != ciphertext)
                memcpy(ciphertext, plaintext, localSize);
            Keccak_DuplexingGetFurtherOutputAndXOR(duplex, ciphertext, localSize);
            Keccak_DuplexingOverwritePartialInput(duplex, ciphertext, localSize);

            plaintext += localSize;
            ciphertext += localSize;
            dataSizeInBytes -= localSize;
        }
    }

    if (last)
        Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x05); // 10
}
#else
void Keyak_ProcessPlaintext(Keccak_ParallelDuplexInstances* duplex, const unsigned char *plaintext, unsigned char *ciphertext, size_t dataSizeInBytes, int last)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;
    unsigned int totalByteInputIndex = Keccak_ParallelDuplexGetTotalInputIndex(duplex);

    while(dataSizeInBytes > 0) {

        if (totalByteInputIndex == rhoInBytes*PlSnP_P) {
            Keccak_ParallelDuplexingAll(duplex, 0x07); // 11
            totalByteInputIndex = 0;
        }

        if ((rhoInBytes%SnP_laneLengthInBytes == 0) && (totalByteInputIndex == 0) && (dataSizeInBytes > 2*rhoInBytes*PlSnP_P)) {
            size_t localSize = Keccak_ParallelDuplexingFBWLWrap(duplex, plaintext, ciphertext, dataSizeInBytes-rhoInBytes*PlSnP_P, 0x07); // 11
            plaintext += localSize;
            ciphertext += localSize;
            dataSizeInBytes -= localSize;
        }
        else {
            unsigned int localSize;
            if (dataSizeInBytes > (rhoInBytes*PlSnP_P - totalByteInputIndex))
                localSize = rhoInBytes*PlSnP_P - totalByteInputIndex;
            else
                localSize = dataSizeInBytes;

            if (plaintext != ciphertext)
                memcpy(ciphertext, plaintext, localSize);
            Keccak_ParallelDuplexingGetFurtherInterleavedOutputAndXOR(duplex, ciphertext, localSize);
            Keccak_ParallelDuplexingOverwritePartialInterleavedInput(duplex, ciphertext, localSize);
            plaintext += localSize;
            ciphertext += localSize;
            dataSizeInBytes -= localSize;
            totalByteInputIndex += localSize;
        }
    }

    if (last)
        Keccak_ParallelDuplexingAll(duplex, 0x05); // 10
}
#endif

int Keyak_WrapPlaintext(Keyak_Instance *instance, const unsigned char *plaintext, unsigned char *ciphertext, size_t dataSizeInBytes)
{
    if (dataSizeInBytes > 0) {
        if ((instance->phase & Keyak_Phase_FeedingAssociatedData) != 0) {
            Keyak_ProcessAssociatedData(&(instance->duplex), 0, 0, 1, 1);
            instance->phase = Keyak_Phase_Wrapping;
        }
        if ((instance->phase & Keyak_Phase_Wrapping) == 0)
            return 1;
        Keyak_ProcessPlaintext(&(instance->duplex), plaintext, ciphertext, dataSizeInBytes, 0);
        instance->phase = Keyak_Phase_Wrapping;
    }

    return 0;
}

#if ((!defined(PlSnP_P)) || (PlSnP_P == 1))
void Keyak_ProcessCiphertext(Keccak_DuplexInstance* duplex, const unsigned char *ciphertext, unsigned char *plaintext, size_t dataSizeInBytes, int last)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;

    while(dataSizeInBytes > 0) {
        if (Keccak_DuplexGetInputIndex(duplex) == rhoInBytes)
            Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x07); // 11

        if ((rhoInBytes%SnP_laneLengthInBytes == 0) && (Keccak_DuplexGetInputIndex(duplex) == 0) && (dataSizeInBytes > 2*rhoInBytes)) {
            size_t localSize = Keccak_DuplexingFBWLUnwrap(duplex, ciphertext, plaintext, dataSizeInBytes-rhoInBytes, 0x07); // 11
            ciphertext += localSize;
            plaintext += localSize;
            dataSizeInBytes -= localSize;
        }
        else {
            unsigned int localSize;

            if (dataSizeInBytes > (rhoInBytes - Keccak_DuplexGetInputIndex(duplex)))
                localSize = rhoInBytes - Keccak_DuplexGetInputIndex(duplex);
            else
                localSize = dataSizeInBytes;

            if (plaintext != ciphertext)
                memcpy(plaintext, ciphertext, localSize);
            Keccak_DuplexingGetFurtherOutputAndXOR(duplex, plaintext, localSize);
            Keccak_DuplexingFeedPartialInput(duplex, plaintext, localSize);

            ciphertext += localSize;
            plaintext += localSize;
            dataSizeInBytes -= localSize;
        }
    }

    if (last)
        Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x05); // 10
}
#else
void Keyak_ProcessCiphertext(Keccak_ParallelDuplexInstances* duplex, const unsigned char *ciphertext, unsigned char *plaintext, size_t dataSizeInBytes, int last)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;
    unsigned int totalByteInputIndex = Keccak_ParallelDuplexGetTotalInputIndex(duplex);

    while(dataSizeInBytes > 0) {
        if (totalByteInputIndex == rhoInBytes*PlSnP_P) {
            Keccak_ParallelDuplexingAll(duplex, 0x07); // 11
            totalByteInputIndex = 0;
        }

        if ((rhoInBytes%SnP_laneLengthInBytes == 0) && (totalByteInputIndex == 0) && (dataSizeInBytes > 2*rhoInBytes*PlSnP_P)) {
            size_t localSize = Keccak_ParallelDuplexingFBWLUnwrap(duplex, ciphertext, plaintext, dataSizeInBytes-rhoInBytes*PlSnP_P, 0x07); // 11
            plaintext += localSize;
            ciphertext += localSize;
            dataSizeInBytes -= localSize;
        }
        else {
            unsigned int localSize;
            if (dataSizeInBytes > (rhoInBytes*PlSnP_P - totalByteInputIndex))
                localSize = rhoInBytes*PlSnP_P - totalByteInputIndex;
            else
                localSize = dataSizeInBytes;

            if (plaintext != ciphertext)
                memcpy(plaintext, ciphertext, localSize);
            Keccak_ParallelDuplexingGetFurtherInterleavedOutputAndXOR(duplex, plaintext, localSize);
            Keccak_ParallelDuplexingFeedPartialInterleavedInput(duplex, plaintext, localSize);
            ciphertext += localSize;
            plaintext += localSize;
            dataSizeInBytes -= localSize;
            totalByteInputIndex += localSize;
        }
    }

    if (last)
        Keccak_ParallelDuplexingAll(duplex, 0x05); // 10
}
#endif

int Keyak_UnwrapCiphertext(Keyak_Instance *instance, const unsigned char *ciphertext, unsigned char *plaintext, size_t dataSizeInBytes)
{
    if (dataSizeInBytes > 0) {
        if ((instance->phase & Keyak_Phase_FeedingAssociatedData) != 0) {
            Keyak_ProcessAssociatedData(&(instance->duplex), 0, 0, 1, 1);
            instance->phase = Keyak_Phase_Unwrapping;
        }
        if ((instance->phase & Keyak_Phase_Unwrapping) == 0)
            return 1;
        Keyak_ProcessCiphertext(&(instance->duplex), ciphertext, plaintext, dataSizeInBytes, 0);
        instance->phase = Keyak_Phase_Unwrapping;
    }

    return 0;
}

#if ((!defined(PlSnP_P)) || (PlSnP_P == 1))
void Keyak_ProcessTag(Keccak_DuplexInstance* duplex, unsigned char *tag, size_t tagSizeInBytes)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;

    while(tagSizeInBytes > 0) {
        unsigned int localSize;

        if (Keccak_DuplexGetOutputIndex(duplex) == rhoInBytes)
            Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x02); // 0

        if (tagSizeInBytes > (rhoInBytes - Keccak_DuplexGetOutputIndex(duplex)))
            localSize = rhoInBytes - Keccak_DuplexGetOutputIndex(duplex);
        else
            localSize = tagSizeInBytes;

        Keccak_DuplexingGetFurtherOutput(duplex, tag, localSize);
        tag += localSize;
        tagSizeInBytes -= localSize;
    }
}
#else
void Keyak_ProcessTag(Keccak_ParallelDuplexInstances* duplex, unsigned char *tag, size_t tagSizeInBytes)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;
    unsigned char buffer[32];
    unsigned int i;

    // This assumes that (P-1)*256 <= rho_max
    for(i=1; i<PlSnP_P; i++) {
        Keccak_ParallelDuplexingGetFurtherSingleOutput(duplex, i, buffer, 32);
        Keccak_ParallelDuplexingFeedPartialSingleInput(duplex, 0, buffer, 32);
    }
    Keccak_ParallelDuplexing(duplex, 0, 0x05); // 10

    while(tagSizeInBytes > 0) {
        unsigned int localSize;

        if (Keccak_ParallelDuplexGetOutputIndex(duplex, 0) == rhoInBytes)
            Keccak_ParallelDuplexing(duplex, 0, 0x02); // 0

        if (tagSizeInBytes > (rhoInBytes - Keccak_ParallelDuplexGetOutputIndex(duplex, 0)))
            localSize = rhoInBytes - Keccak_ParallelDuplexGetOutputIndex(duplex, 0);
        else
            localSize = tagSizeInBytes;

        Keccak_ParallelDuplexingGetFurtherSingleOutput(duplex, 0, tag, localSize);
        tag += localSize;
        tagSizeInBytes -= localSize;
    }
}
#endif

int Keyak_GetTag(Keyak_Instance *instance, unsigned char *tag, size_t tagSizeInBytes)
{
    if ((instance->phase & Keyak_Phase_FeedingAssociatedData) != 0)
        Keyak_ProcessAssociatedData(&(instance->duplex), 0, 0, 1, 0);
    else if ((instance->phase & Keyak_Phase_Wrapping) != 0)
        Keyak_ProcessPlaintext(&(instance->duplex), 0, 0, 0, 1);
    else if ((instance->phase & Keyak_Phase_Unwrapping) != 0)
        Keyak_ProcessCiphertext(&(instance->duplex), 0, 0, 0, 1);
    Keyak_ProcessTag(&(instance->duplex), tag, tagSizeInBytes);
    instance->phase = Keyak_Phase_FeedingAssociatedData;

    return 0;
}

#if ((!defined(PlSnP_P)) || (PlSnP_P == 1))
void Keyak_ProcessForget(Keccak_DuplexInstance* duplex)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;

    Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x01);
    Keccak_DuplexingOverwriteWithZeroes(duplex, rhoInBytes);
    Keccak_Duplexing(duplex, 0, 0, 0, 0, 0x01);
}
#else
void Keyak_ProcessForget(Keccak_ParallelDuplexInstances* duplex)
{
    unsigned int rhoInBytes = (duplex->rate-4)/8;
    unsigned int i;

    Keccak_ParallelDuplexingAll(duplex, 0x01);
    for(i=0; i<PlSnP_P; i++)
        Keccak_ParallelDuplexingOverwriteWithZeroes(duplex, i, rhoInBytes);
    Keccak_ParallelDuplexingAll(duplex, 0x01);
}
#endif

int Keyak_Forget(Keyak_Instance *instance)
{
    Keyak_ProcessForget(&(instance->duplex));
    instance->phase = Keyak_Phase_FeedingAssociatedData;
    return 0;
}
