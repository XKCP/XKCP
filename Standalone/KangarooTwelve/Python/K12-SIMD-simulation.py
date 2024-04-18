# -*- coding: utf-8 -*-
# Implementation by Gilles Van Assche, hereby denoted as "the implementer".
#
# For more information, feedback or questions, please refer to our website:
# https://keccak.team/
#
# To the extent possible under law, the implementer has waived all copyright
# and related or neighboring rights to the source code in this file.
# http://creativecommons.org/publicdomain/zero/1.0/

"""
The Python code in this module illustrates how the KangarooTwelve variations, KT128 and KT256, can be implemented using single-instruction-multiple-data (SIMD) instructions.
It does not provide optimized SIMD code, but instead illustrates how one can write SIMD code for K12 by simulating how SIMD works.
As a convention, it simulates SIMD registers of N×64 bits, for some positive integer N, by putting the N words of 64 bits in a list.
For instance, 256-bit SIMD instructions like AVX2 work on N=4 words of 64 bits.
SIMD operations are then applied to all the elements of the list.

K12 is described in https://keccak.team/kangarootwelve.html
In a nutshell, the input string is cut into chunks of 8KiB, called leaves, and the leaves are hashed independently.
With SIMD instructions applying to N×64-bit words, one can efficiently hash N leaves in parallel.
"""

from TurboSHAKE import ROL64, load64, store64, KeccakP1600, KeccakP1600onLanes, TurboSHAKE128, TurboSHAKE256
from K12 import right_encode

"""
Evaluates Keccak-p[1600, nr] on N states in parallel independently.
The input states are represented as 5×5 SIMD registers each containing N lanes.
states[x][y] represents a SIMD register that contains N words of 64 bits:
- states[x][y][0] is the lane value at (x, y) for the first state,
- states[x][y][1] is the lane value at (x, y) for the second state, etc.

Parameters
----------

N: integer
    The number of states to apply Keccak-p[1600, nr] to.
    It must be a strictly positive integer.

states: list of list of list of integers
    The N input states as a 5×5 matrix of N lanes, with each lane is a 64-bit word in the form of an integer.
    lanes[x][y][i] for 0 ≤ x,y < 5 and 0 ≤ i < N contains the lane at coordinates (x, y) of the i-th state.

nrRounds: integer
    The number of rounds to perform, must be 12 for Keccak-p[1600, 12] used in KT128 and KT256.

Returns
-------

list of list of list of integers
    The N output states as a 5×5 matrix of N lanes, represented like the "states" input parameter.
"""
def KeccakP1600timesN_SIMD(N, states, nrRounds):
    R = 1
    for round in range(24):
        if (round + nrRounds >= 24):
            # Perform all the round operations in a SIMD fashion.
            # That is, perform whatever Boolean operations on all N words of 64 bits.
            # This is what the "for i in range(N)" simulates.
            # θ
            C = [[states[x][0][i] ^ states[x][1][i] ^ states[x][2][i] ^ states[x][3][i] ^ states[x][4][i] for i in range(N)] for x in range(5)]
            D = [[C[(x+4)%5][i] ^ ROL64(C[(x+1)%5][i], 1) for i in range(N)] for x in range(5)]
            states = [[[states[x][y][i]^D[x][i] for i in range(N)] for y in range(5)] for x in range(5)]
            # ρ and π
            (x, y) = (1, 0)
            current = [states[x][y][i] for i in range(N)]
            for t in range(24):
                (x, y) = (y, (2*x+3*y)%5)
                (current, states[x][y]) = (states[x][y], [ROL64(current[i], (t+1)*(t+2)//2) for i in range(N)])
            # χ
            for y in range(5):
                T = [states[x][y] for x in range(5)]
                for x in range(5):
                    states[x][y] = [T[x][i] ^((~T[(x+1)%5][i]) & T[(x+2)%5][i]) for i in range(N)]
            # ι
            for j in range(7):
                R = ((R << 1) ^ ((R >> 7)*0x71)) % 256
                if (R & 2):
                    states[0][0] = [states[0][0][i] ^ (1 << ((1<<j)-1)) for i in range(N)]
        else:
            for j in range(7):
                R = ((R << 1) ^ ((R >> 7)*0x71)) % 256
    return states


"""
Bitwise add (XOR) data into N states in parallel.
The granularity is the lane (64 bits).
For the first state, we take the 8*laneCount bytes at data[0 : 8*laneCount] and add them to it.
For the second state, we take 8*laneCount bytes 8*laneOffset bytes further, so at data[8*laneOffset : 8*laneOffset + 8*laneCount], and add them to it.
For the third state, we do the same, but again 8*laneOffset bytes further, etc.
For KT128 and KT256, 8*laneOffset is 8192 so that the first state gets data from the first leaf, the second state from the second leaf, etc.

Parameters
----------

N: integer
    The number of states to modify.
    It must be a strictly positive integer, N ≥ 1.

states: list of list of list of integers
    The N input states, as in KeccakP1600timesN_SIMD.

data: byte array
    The data to add to the states.
    Its size must be at least 8*((N-1)*laneOffset + laneCount) bytes.

laneCount: integer
    The number of 64-bit words to add to each state.
    It must be an integer between 0 and 25.

laneOffset: integer
    The distance in 64-bit words between which we take bytes from data and add them to the different states.
    For K12, laneOffset is 1024 (8192 bytes divided by 8 bytes), the distance between leaves.

Returns
-------

list of list of list of integers
    The N output states, as in KeccakP1600timesN_SIMD.
"""
def KeccakP1600timesN_AddLanesAll(N, states, data, laneCount, laneOffset):
    assert(len(data) >= 8*((N-1)*laneOffset + laneCount))
    for y in range(5):
        for x in range(5):
            xy = x + 5*y
            # Loop through the first laneCount lanes (x, y) of the states
            if (xy < laneCount):
                # Load N words of 64 bits in a SIMD register by taking
                # - 64 bits from data[8*xy : 8*xy + 8]
                # - 64 bits from data[8*xy + 8*laneOffset : 8*xy + 8*laneOffset + 8]
                # - etc.
                loadedData = [load64(data[8*(i*laneOffset + xy) : 8*(i*laneOffset + xy + 1)]) for i in range(N)]
                # XOR the loaded data into the states at lane (x, y) in a SIMD fashion
                states[x][y] = [states[x][y][i] ^ loadedData[i] for i in range(N)]
    return states


# Global variables for KT128 and KT256
B = 8192

"""
Evaluates TurboSHAKE128 on N chunks of 8KiB (or leaves) in parallel and returns the chaining values.

Parameters
----------

N: integer
    The number of leaves to evaluate in parallel.
    It must be a strictly positive integer, N ≥ 1.

data: byte array
    The N leaves to hash.
    Its size must be at least N*B bytes.

Returns
-------

byte array
    The N chaining values.
"""
def KT128_ProcessLeaves(N, data):
    assert(len(data) >= N*B)
    rateInLanes = 21
    rateInBytes = rateInLanes*8
    # Initialize N all-zero states
    A = [[[0 for i in range(N)] for y in range(5)] for x in range(5)]

    # First, start with all the complete blocks of 21 lanes (= 168 bytes)
    for j in range(0, B - rateInBytes, rateInBytes):
        # Add 168 bytes from position j of the leaves to the states, offseted by 8KiB, that is:
        # - add bytes data[j : j + 168] to the first state
        # - add bytes data[8KiB + j : 8KiB + j + 168] to the second state
        # - add bytes data[16KiB + j : 16KiB + j + 168] to the third state, etc.
        KeccakP1600timesN_AddLanesAll(N, A, data[j:], rateInLanes, B//8) 
        # Apply Keccak-p[1600, 12 rounds] to all states
        A = KeccakP1600timesN_SIMD(N, A, 12)

    # Set the position of the last, incomplete, block of 16 lanes (= 128 bytes)
    j = (B//rateInBytes)*rateInBytes
    # Add 128 bytes from position j of the leaves to the states, offseted by 8KiB
    KeccakP1600timesN_AddLanesAll(N, A, data[j:], (B - j)//8, B//8)
    # Append the suffix 110 and the first bit of padding to all states
    A[1][3] = [A[1][3][i] ^ 0x0B for i in range(N)]
    # Append the second bit of padding to all states
    A[0][4] = [A[0][4][i] ^ 0x8000000000000000 for i in range(N)]
    # Apply Keccak-p[1600, 12 rounds] to all states
    A = KeccakP1600timesN_SIMD(N, A, 12)

    # Extract the first 256 bits of each state and concatenate them
    CVs = bytearray().join([store64(A[0][0][i]) + store64(A[1][0][i]) + store64(A[2][0][i]) + store64(A[3][0][i]) for i in range(N)])
    return CVs

"""
Same concept as KT128_ProcessLeaves, but for KT256.
The returned chaining value is 512 bits instead of 256 bits, to conform to the 256 bits security level of KT256.
"""
def KT256_ProcessLeaves(N, data):
    assert(len(data) >= N*B)
    rateInLanes = 17
    rateInBytes = rateInLanes*8
    # Initialize N all-zero states
    A = [[[0 for i in range(N)] for y in range(5)] for x in range(5)]

    # First, start with all the complete blocks of 17 lanes (= 136 bytes)
    for j in range(0, B - rateInBytes, rateInBytes):
        # Add 136 bytes from position j of the leaves to the states, offseted by 8KiB, that is:
        # - add bytes data[j : j + 136] to the first state
        # - add bytes data[8KiB + j : 8KiB + j + 136] to the second state
        # - add bytes data[16KiB + j : 16KiB + j + 136] to the third state, etc.
        KeccakP1600timesN_AddLanesAll(N, A, data[j:], rateInLanes, B//8)
        # Apply Keccak-p[1600, 12 rounds] to all states
        A = KeccakP1600timesN_SIMD(N, A, 12)

    # Set the position of the last, incomplete, block of 4 lanes (= 32 bytes)
    j = (B//rateInBytes)*rateInBytes
    # Add 32 bytes from position j of the leaves to the states, offseted by 8KiB
    KeccakP1600timesN_AddLanesAll(N, A, data[j:], (B - j)//8, B//8)
    # Append the suffix 110 and the first bit of padding to all states
    A[4][0] = [A[4][0][i] ^ 0x0B for i in range(N)]
    # Append the second bit of padding to all states
    A[1][3] = [A[1][3][i] ^ 0x8000000000000000 for i in range(N)]
    # Apply Keccak-p[1600, 12 rounds] to all states
    A = KeccakP1600timesN_SIMD(N, A, 12)

    CVs = bytearray().join([
            store64(A[0][0][i]) + store64(A[1][0][i]) + store64(A[2][0][i]) + store64(A[3][0][i]) + 
            store64(A[4][0][i]) + store64(A[0][1][i]) + store64(A[1][1][i]) + store64(A[2][1][i]) 
        for i in range(N)])
    return CVs


"""
Evaluates K12 on the given input message and customization string, and returns the desired number of output bytes.

Parameters
----------

inputMessage: byte string or byte array
    The input message.

customizationString: byte string or byte array
    The customization string.

outputByteLen: integer
    The desired number of output bytes.

Returns
-------

byte array
    The first outputByteLen bytes of K12(inputMessage, customizationString).
"""
def KT128(inputMessage, customizationString, outputByteLen):
    c = 256
    # Concatenate the input message, the customization string and the length of the latter
    S = bytearray(inputMessage) + bytearray(customizationString) + right_encode(len(customizationString))
    if (len(S) <= B):
        # If S fits in one chunk, process the tree with only a final node
        return TurboSHAKE128(S, 0x07, outputByteLen)
    else:
        # If S needs more than one chunk, process the tree with kangaroo hopping
        CVs = bytearray()
        # Process the leaves starting from offset 8KiB, as the first chunk is part of the final node
        j = B
        # Count the number of leaves
        n = 0
        # Process 8 leaves in parallel if possible
        while(j + 8*B <= len(S)):
            CVs = CVs + KT128_ProcessLeaves(8, S[j:])
            j = j + 8*B
            n = n + 8
        # Process 4 leaves in parallel if possible
        while(j + 4*B <= len(S)):
            CVs = CVs + KT128_ProcessLeaves(4, S[j:])
            j = j + 4*B
            n = n + 4
        # Process 2 leaves in parallel if possible
        while(j + 2*B <= len(S)):
            CVs = CVs + KT128_ProcessLeaves(2, S[j:])
            j = j + 2*B
            n = n + 2
        # Process the remaining leaf
        while(j < len(S)):
            CVs = CVs + TurboSHAKE128(S[j:j+B], 0x0B, c//8)
            j = j + B
            n = n + 1
        # Process the final node
        NodeStar = S[0:B] + bytearray([3,0,0,0,0,0,0,0]) + CVs \
            + right_encode(n) + b'\xFF\xFF'
        return TurboSHAKE128(NodeStar, 0x06, outputByteLen)
    

"""
Same concept as KT128, but for KT256.
"""
def KT256(inputMessage, customizationString, outputByteLen):
    c = 512
    # Concatenate the input message, the customization string and the length of the latter
    S = bytearray(inputMessage) + bytearray(customizationString) + right_encode(len(customizationString))
    if (len(S) <= B):
        # If S fits in one chunk, process the tree with only a final node
        return TurboSHAKE256(S, 0x07, outputByteLen)
    else:
        # If S needs more than one chunk, process the tree with kangaroo hopping
        CVs = bytearray()
        # Process the leaves starting from offset 8KiB, as the first chunk is part of the final node
        j = B
        # Count the number of leaves
        n = 0
        # Process 8 leaves in parallel if possible
        while(j + 8*B <= len(S)):
            CVs = CVs + KT256_ProcessLeaves(8, S[j:])
            j = j + 8*B
            n = n + 8
        # Process 4 leaves in parallel if possible
        while(j + 4*B <= len(S)):
            CVs = CVs + KT256_ProcessLeaves(4, S[j:])
            j = j + 4*B
            n = n + 4
        # Process 2 leaves in parallel if possible
        while(j + 2*B <= len(S)):
            CVs = CVs + KT256_ProcessLeaves(2, S[j:])
            j = j + 2*B
            n = n + 2
        # Process the remaining leaf
        while(j < len(S)):
            CVs = CVs + TurboSHAKE256(S[j:j+B], 0x0B, c//8)
            j = j + B
            n = n + 1
        # Process the final node
        NodeStar = S[0:B] + bytearray([3,0,0,0,0,0,0,0]) + CVs \
            + right_encode(n) + b'\xFF\xFF'
        return TurboSHAKE256(NodeStar, 0x06, outputByteLen)


# Test that KeccakP1600timesN_SIMD does what it is supposed to
def Test_KeccakP1600timesN_SIMD():
    for N in range(1, 5):
        print("Testing KeccakP1600timesN_SIMD for N =", N)
        lanes = [[[(x+y+i+x*y*i) % (2**64) for i in range(N)] for y in range(5)] for x in range(5)]
        lanes_t = [[[lanes[x][y][i] for y in range(5)] for x in range(5)] for i in range(N)]
        ref_lanes_t = [KeccakP1600onLanes(lanes_t[i], 24) for i in range(N)]
        ref_lanes = [[[ref_lanes_t[i][x][y] for i in range(N)] for y in range(5)] for x in range(5)]
        test_lanes = KeccakP1600timesN_SIMD(N, lanes, 24)
        assert(ref_lanes == test_lanes)
    return

# Test that KangarooTwelve_ProcessLeaves does what it is supposed to
def Test_KT128_ProcessLeaves():
    c = 256
    for N in range(1, 5):
        print("Testing KT128_ProcessLeaves for N =", N)
        S = bytearray([(i%247) for i in range(B*N)])
        Si = [bytearray(S[i*B:(i+1)*B]) for i in range(N)]
        ref_CVs = bytearray().join([TurboSHAKE128(Si[i], 0x0B, c//8) for i in range(N)])
        test_CVs = KT128_ProcessLeaves(N, S)
        assert(test_CVs == ref_CVs)

def outputHex(s):
    for i in range(len(s)):
        print("{0:02x}".format(s[i]), end=' ')
    print()
    print()

# for testing purposes, inspired by the test vectors of the update KangarooTwelve draft
# https://datatracker.ietf.org/doc/html/draft-irtf-cfrg-kangarootwelve
def ptn(n):
    pattern = bytes(range(0xFA + 1))  # Include 0xFA
    repetitions = n // len(pattern)
    remainder = n % len(pattern)
    repeated_pattern = pattern * repetitions + pattern[:remainder]
    return repeated_pattern

# Produce test vectors
def printKT128TestVectors():
    print("KT128(M=empty, C=empty, 32 output bytes):")
    outputHex(KT128(b'', b'', 32))
    print("KT128(M=empty, C=empty, 64 output bytes):")
    outputHex(KT128(b'', b'', 64))
    print("KT128(M=empty, C=empty, 10032 output bytes), last 32 bytes:")
    outputHex(KT128(b'', b'', 10032)[10000:])
    for i in range(6):
        C = b''
        M = ptn(17**i)
        print("KT128(M=pattern 0x00 to 0xFA for 17^{0:d} bytes, C=empty, 32 output bytes):".format(i))
        outputHex(KT128(M, C, 32))
    for i in range(4):
        M = bytearray([0xFF for j in range(2**i-1)])
        C = ptn(41**i)
        print("KT128(M={0:d} times byte 0xFF, C=pattern 0x00 to 0xFA for 41^{1:d} bytes, 32 output bytes):".format(2**i-1, i))
        outputHex(KT128(M, C, 32))

def printKT256TestVectors():
    print("KT256(M=empty, C=empty, 64 output bytes):")
    outputHex(KT256(b'', b'', 64))
    print("KT256(M=empty, C=empty, 128 output bytes):")
    outputHex(KT256(b'', b'', 128))
    for i in range(6):
        C = b''
        M = ptn(17**i)
        print("KT256(M=pattern 0x00 to 0xFA for 17^{0:d} bytes, C=empty, 64 output bytes):".format(i))
        outputHex(KT256(M, C, 64))
    for i in range(4):
        M = bytearray([0xFF for j in range(2**i-1)])
        C = ptn(41**i)
        print("KT256(M={0:d} times byte 0xFF, C=pattern 0x00 to 0xFA for 41^{1:d} bytes, 64 output bytes):".format(2**i-1, i))
        outputHex(KT256(M, C, 64))


Test_KeccakP1600timesN_SIMD()
Test_KT128_ProcessLeaves()
printKT128TestVectors()
printKT256TestVectors()
