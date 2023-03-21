# -*- coding: utf-8 -*-
# Implementation by Gilles Van Assche, hereby denoted as "the implementer".
#
# For more information, feedback or questions, please refer to our website:
# https://keccak.team/
#
# To the extent possible under law, the implementer has waived all copyright
# and related or neighboring rights to the source code in this file.
# http://creativecommons.org/publicdomain/zero/1.0/

from TurboSHAKE import TurboSHAKE
from K12 import KangarooTwelve

def outputHex(s):
    for i in range(len(s)):
        print("{0:02x}".format(s[i]), end=' ')
    print()
    print()

def printK12TestVectors():
    print("KangarooTwelve(M=empty, C=empty, 32 output bytes):")
    outputHex(KangarooTwelve(b'', b'', 32))
    print("KangarooTwelve(M=empty, C=empty, 64 output bytes):")
    outputHex(KangarooTwelve(b'', b'', 64))
    print("KangarooTwelve(M=empty, C=empty, 10032 output bytes), last 32 bytes:")
    outputHex(KangarooTwelve(b'', b'', 10032)[10000:])
    for i in range(6):
        C = b''
        M = bytearray([(j % 251) for j in range(17**i)])
        print("KangarooTwelve(M=pattern 0x00 to 0xFA for 17^{0:d} bytes, C=empty, 32 output bytes):".format(i))
        outputHex(KangarooTwelve(M, C, 32))
    for i in range(4):
        M = bytearray([0xFF for j in range(2**i-1)])
        C = bytearray([(j % 251) for j in range(41**i)])
        print("KangarooTwelve(M={0:d} times byte 0xFF, C=pattern 0x00 to 0xFA for 41^{1:d} bytes, 32 output bytes):".format(2**i-1, i))
        outputHex(KangarooTwelve(M, C, 32))

def printTurboSHAKETestVectors():
        for c in [256, 512]:
            name = "TurboSHAKE{0:d}".format(c//2)
            for D in [0x01]:
                print("{0:s}(M=empty, D=0x{1:02x}, 32 output bytes):".format(name, D))
                outputHex(TurboSHAKE(c, b'', D, 32))
                print("{0:s}(M=empty, D=0x{1:02x}, 64 output bytes):".format(name, D))
                outputHex(TurboSHAKE(c, b'', D, 64))
                print("{0:s}(M=empty, D=0x{1:02x}, 10032 output bytes), last 32 bytes:".format(name, D))
                outputHex(TurboSHAKE(c, b'', D, 10032)[10000:])
                for i in range(6):
                    M = bytearray([(j % 251) for j in range(17**i)])
                    print("{0:s}(M=pattern 0x00 to 0xFA for 17^{1:d} bytes, D=0x{2:02x}, 32 output bytes):".format(name, i, D))
                    outputHex(TurboSHAKE(c, M, D, 32))
            for D in list(range(0x01, 0x04))+list(range(0x0C, 0x80, 0x17)):
                print("{0:s}(M=empty, D=0x{1:02x}, 32 output bytes):".format(name, D))
                outputHex(TurboSHAKE(c, b'', D, 32))
        for c in range(8, 520, 56):
            name = "TurboSHAKE[c={0:d}]".format(c)
            M = bytearray([(j % 251) for j in range(11**4)])
            D = ((c//8)*41)%127+1
            print("{0:s}(M=pattern 0x00 to 0xFA for 11^4 bytes, D=0x{1:02x}, 32 output bytes):".format(name, D))
            outputHex(TurboSHAKE(c, M, D, 32))

printK12TestVectors()
printTurboSHAKETestVectors()
