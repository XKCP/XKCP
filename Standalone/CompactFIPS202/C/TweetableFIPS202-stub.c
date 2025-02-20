#if DEBUG
#include <assert.h>
#endif
#include <string.h>

typedef unsigned char u8;
typedef unsigned long long u64;

int crypto_hash_shake128(u8 *h, u64 d, const u8 *m, u64 n);
int crypto_hash_shake256(u8 *h, u64 d, const u8 *m, u64 n);
int crypto_hash_sha3224(u8 *h, const u8 *m, u64 n);
int crypto_hash_sha3256(u8 *h, const u8 *m, u64 n);
int crypto_hash_sha3384(u8 *h, const u8 *m, u64 n);
int crypto_hash_sha3512(u8 *h, const u8 *m, u64 n);

void Keccak(int rate, int capacity, const unsigned char *input, unsigned long long int inputByteLen, unsigned char delimitedSuffix, unsigned char *output, unsigned long long int outputByteLen)
{
    if ((rate == 1344) && (capacity == 256) && (delimitedSuffix == 0x1F)) {
        crypto_hash_shake128(output, outputByteLen, input, inputByteLen);
    }
    else
    if ((rate == 1088) && (capacity == 512) && (delimitedSuffix == 0x1F)) {
        crypto_hash_shake256(output, outputByteLen, input, inputByteLen);
    }
    else
    if ((rate == 1152) && (capacity == 448) && (delimitedSuffix == 0x06) && (outputByteLen == 28))
        crypto_hash_sha3224(output, input, inputByteLen);
    else
    if ((rate == 1088) && (capacity == 512) && (delimitedSuffix == 0x06) && (outputByteLen == 32))
        crypto_hash_sha3256(output, input, inputByteLen);
    else
    if ((rate == 832) && (capacity == 768) && (delimitedSuffix == 0x06) && (outputByteLen == 48))
        crypto_hash_sha3384(output, input, inputByteLen);
    else
    if ((rate == 576) && (capacity == 1024) && (delimitedSuffix == 0x06) && (outputByteLen == 64))
        crypto_hash_sha3512(output, input, inputByteLen);
    else
#if DEBUG
        assert(0);
#endif
}
