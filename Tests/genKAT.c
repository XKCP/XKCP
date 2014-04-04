/*
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michaël Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/

This code is based on genKAT.c by NIST.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "KeccakHash.h"

#define MAX_MARKER_LEN      50
#define SUBMITTER_INFO_LEN  128

typedef enum { KAT_SUCCESS = 0, KAT_FILE_OPEN_ERROR = 1, KAT_HEADER_ERROR = 2, KAT_DATA_ERROR = 3, KAT_HASH_ERROR = 4 } STATUS_CODES;

#define ExcludeExtremelyLong

#define SqueezingOutputLength 4096

STATUS_CODES    genShortMsgHash(unsigned int rate, unsigned int capacity, unsigned char delimitedSuffix, unsigned int hashbitlen, unsigned int squeezedOutputLength, const char *fileName, const char *description);
int     FindMarker(FILE *infile, const char *marker);
int     ReadHex(FILE *infile, BitSequence *A, int Length, char *str);
void    fprintBstr(FILE *fp, char *S, BitSequence *A, int L);
void convertShortMsgToPureLSB();

STATUS_CODES
genKAT_main()
{
    // The following instances are from the FIPS 202 draft.
    // http://csrc.nist.gov/publications/
    //
    // Note: "SakuraSequential" translates into "input followed by 11",
    // see http://keccak.noekeon.org/Sakura.pdf for more details.
    //
    genShortMsgHash(1344, 256, 0x1F, 0, 4096,
        "ShortMsgKAT_SHAKE128.txt",
        "Keccak(SakuraSequential|11)[r=1344, c=256], or SHAKE128 as in FIPS 202 draft");
    genShortMsgHash(1088, 512, 0x1F, 0, 4096,
        "ShortMsgKAT_SHAKE256.txt",
        "Keccak(SakuraSequential|11)[r=1088, c=512], or SHAKE256 as in FIPS 202 draft");
    genShortMsgHash(1152, 448, 0x06, 224, 0,
        "ShortMsgKAT_SHA3-224.txt",
        "Keccak(input|01)[r=1152, c=448] truncated to 224 bits, or SHA3-224 as in FIPS 202 draft");
    genShortMsgHash(1088, 512, 0x06, 256, 0,
        "ShortMsgKAT_SHA3-256.txt",
        "Keccak(input|01)[r=1088, c=512] truncated to 256 bits, or SHA3-256 as in FIPS 202 draft");
    genShortMsgHash(832, 768, 0x06, 384, 0,
        "ShortMsgKAT_SHA3-384.txt",
        "Keccak(input|01)[r=832, c=768] truncated to 384 bits, or SHA3-384 as in FIPS 202 draft");
    genShortMsgHash(576, 1024, 0x06, 512, 0,
        "ShortMsgKAT_SHA3-512.txt",
        "Keccak(input|01)[r=576, c=1024] truncated to 512 bits, or SHA3-512 as in FIPS 202 draft");

    return KAT_SUCCESS;
}

void convertShortMsgToPureLSB()
{
    int         msglen, msgbytelen, done;
    BitSequence Msg[256];
    BitSequence Squeezed[SqueezingOutputLength/8];
    Keccak_HashInstance   hash;
    FILE        *fp_in, *fp_out;

    if ( (fp_in = fopen("ShortMsgKAT.txt", "r")) == NULL ) {
        printf("Couldn't open <ShortMsgKAT.txt> for read\n");
        return;
    }

    if ( (fp_out = fopen("ShortMsgKAT-PureLSB.txt", "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", "ShortMsgKAT-PureLSB.txt");
        return;
    }

    done = 0;
    do {
        if ( FindMarker(fp_in, "Len = ") )
            fscanf(fp_in, "%d", &msglen);
        else {
            done = 1;
            break;
        }
        msgbytelen = (msglen+7)/8;

        if ( !ReadHex(fp_in, Msg, msgbytelen, "Msg = ") ) {
            printf("ERROR: unable to read 'Msg' from <ShortMsgKAT.txt>\n");
            return;
        }
        // Align the last byte on the least significant bit
        if ((msglen % 8) != 0)
            Msg[msgbytelen-1] = Msg[msgbytelen-1] >> (8-(msglen%8));

        fprintf(fp_out, "\nLen = %d\n", msglen);
        fprintBstr(fp_out, "Msg = ", Msg, msgbytelen);
        fprintf(fp_out, "MD = ??\n", msglen);
    } while ( !done );

    fclose(fp_in);
    fclose(fp_out);
}

STATUS_CODES
genShortMsgHash(unsigned int rate, unsigned int capacity, unsigned char delimitedSuffix, unsigned int hashbitlen, unsigned int squeezedOutputLength, const char *fileName, const char *description)
{
    int         msglen, msgbytelen, done;
    BitSequence Msg[256];
    BitSequence Squeezed[SqueezingOutputLength/8];
    Keccak_HashInstance   hash;
    FILE        *fp_in, *fp_out;
    
    if ((squeezedOutputLength > SqueezingOutputLength) || (hashbitlen > SqueezingOutputLength)) {
        printf("Requested output length too long.\n");
        return KAT_HASH_ERROR;
    }

    if ( (fp_in = fopen("ShortMsgKAT.txt", "r")) == NULL ) {
        printf("Couldn't open <ShortMsgKAT.txt> for read\n");
        return KAT_FILE_OPEN_ERROR;
    }
    
    if ( (fp_out = fopen(fileName, "w")) == NULL ) {
        printf("Couldn't open <%s> for write\n", fileName);
        return KAT_FILE_OPEN_ERROR;
    }
    fprintf(fp_out, "# %s\n", description);
    
    done = 0;
    do {
        if ( FindMarker(fp_in, "Len = ") )
            fscanf(fp_in, "%d", &msglen);
        else {
            done = 1;
            break;
        }
        msgbytelen = (msglen+7)/8;

        if ( !ReadHex(fp_in, Msg, msgbytelen, "Msg = ") ) {
            printf("ERROR: unable to read 'Msg' from <ShortMsgKAT.txt>\n");
            return KAT_DATA_ERROR;
        }
        fprintf(fp_out, "\nLen = %d\n", msglen);
        fprintBstr(fp_out, "Msg = ", Msg, msgbytelen);

        Keccak_HashInitialize(&hash, rate, capacity, hashbitlen, delimitedSuffix);
        Keccak_HashUpdate(&hash, Msg, msglen);
        Keccak_HashFinal(&hash, Squeezed);
        if (hashbitlen > 0)
            fprintBstr(fp_out, "MD = ", Squeezed, hashbitlen/8);
        if (squeezedOutputLength > 0) {
            Keccak_HashSqueeze(&hash, Squeezed, squeezedOutputLength);
            fprintBstr(fp_out, "Squeezed = ", Squeezed, SqueezingOutputLength/8);
        }
    } while ( !done );
    printf("finished ShortMsgKAT for <%s>\n", fileName);
    
    fclose(fp_in);
    fclose(fp_out);
    
    return KAT_SUCCESS;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
FindMarker(FILE *infile, const char *marker)
{
    char    line[MAX_MARKER_LEN];
    int     i, len;

    len = (int)strlen(marker);
    if ( len > MAX_MARKER_LEN-1 )
        len = MAX_MARKER_LEN-1;

    for ( i=0; i<len; i++ )
        if ( (line[i] = fgetc(infile)) == EOF )
            return 0;
    line[len] = '\0';

    while ( 1 ) {
        if ( !strncmp(line, marker, len) )
            return 1;

        for ( i=0; i<len-1; i++ )
            line[i] = line[i+1];
        if ( (line[len-1] = fgetc(infile)) == EOF )
            return 0;
        line[len] = '\0';
    }

    // shouldn't get here
    return 0;
}

//
// ALLOW TO READ HEXADECIMAL ENTRY (KEYS, DATA, TEXT, etc.)
//
int
ReadHex(FILE *infile, BitSequence *A, int Length, char *str)
{
    int         i, ch, started;
    BitSequence ich;

    if ( Length == 0 ) {
        A[0] = 0x00;
        return 1;
    }
    memset(A, 0x00, Length);
    started = 0;
    if ( FindMarker(infile, str) )
        while ( (ch = fgetc(infile)) != EOF ) {
            if ( !isxdigit(ch) ) {
                if ( !started ) {
                    if ( ch == '\n' )
                        break;
                    else
                        continue;
                }
                else
                    break;
            }
            started = 1;
            if ( (ch >= '0') && (ch <= '9') )
                ich = ch - '0';
            else if ( (ch >= 'A') && (ch <= 'F') )
                ich = ch - 'A' + 10;
            else if ( (ch >= 'a') && (ch <= 'f') )
                ich = ch - 'a' + 10;
            
            for ( i=0; i<Length-1; i++ )
                A[i] = (A[i] << 4) | (A[i+1] >> 4);
            A[Length-1] = (A[Length-1] << 4) | ich;
        }
    else
        return 0;

    return 1;
}

void
fprintBstr(FILE *fp, char *S, BitSequence *A, int L)
{
    int     i;

    fprintf(fp, "%s", S);

    for ( i=0; i<L; i++ )
        fprintf(fp, "%02X", A[i]);

    if ( L == 0 )
        fprintf(fp, "00");

    fprintf(fp, "\n");
}
