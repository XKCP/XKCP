# -*- coding: utf-8 -*-
# Implementation by Gilles Van Assche, hereby denoted as "the implementer".
#
# For more information, feedback or questions, please refer to our website:
# https://keccak.team/
#
# To the extent possible under law, the implementer has waived all copyright
# and related or neighboring rights to the source code in this file.
# http://creativecommons.org/publicdomain/zero/1.0/

from __future__ import print_function
import K12

def generateSimpleRawMaterial(length, seed1, seed2):
    seed2 = seed2 % 8
    return bytes([(seed1 + 161*length - ((i%256) << seed2) - ((i%256) >> (8-seed2)) + i)%256 for i in range(length)])

customizationByteSize = 32

def performTestKangarooTwelveOneInput(inputLen, outputLen, customLen):
    customizationString = generateSimpleRawMaterial(customizationByteSize, customLen, 97)[0:customLen]
    inputMessage = generateSimpleRawMaterial(inputLen, outputLen, inputLen + customLen)
    print("outputLen {0:5d}, inputLen {1:5d}, customLen {2:3d}".format(outputLen, inputLen, customLen))
    output = K12.KangarooTwelve(inputMessage, customizationString, outputLen)
    print("Kangaroo-Twelve")
    print("Input of {0:d} bytes:".format(inputLen), end='')
    for i in range(min(inputLen, 16)):
        print(" {0:02x}".format(inputMessage[i]), end='')
    if (inputLen > 16):
        print(" ...", end='')
    print("")
    print("Output of {0:d} bytes:".format(outputLen), end='')
    for i in range(outputLen):
        print(" {0:02x}".format(output[i]), end='')
    print("")
    print("")

def performTestKangarooTwelve():
    cBlockSize = 8192
    outputLen = 256//8
    customLen = 0
    for inputLen in range(cBlockSize*9+124):
        performTestKangarooTwelveOneInput(inputLen, outputLen, customLen)

    outputLen = 128//8
    while(outputLen <= 512//8):
        inputLen = 0
        while(inputLen <= (3*cBlockSize)):
            customLen = 0
            while(customLen <= customizationByteSize):
                performTestKangarooTwelveOneInput(inputLen, outputLen, customLen)
                customLen += 7
            inputLen = (inputLen + 167) if (inputLen > 0) else 1
        outputLen = outputLen*2

def performShortTestKangarooTwelve():
    cBlockSize = 8192
    outputLen = 256//8
    customLen = 0
    for inputLen in range(4):
        performTestKangarooTwelveOneInput(inputLen, outputLen, customLen)
    performTestKangarooTwelveOneInput(27121, outputLen, customLen)

#performTestKangarooTwelve()
#performShortTestKangarooTwelve()

def outputHex(s):
    for i in range(len(s)):
        print("{0:02x}".format(s[i]), end=' ')
    print()
    print()

def printTestVectors():
    print("KangarooTwelve(M=empty, C=empty, 32 output bytes):")
    outputHex(K12.KangarooTwelve(b'', b'', 32))
    print("KangarooTwelve(M=empty, C=empty, 64 output bytes):")
    outputHex(K12.KangarooTwelve(b'', b'', 64))
    print("KangarooTwelve(M=empty, C=empty, 10032 output bytes), last 32 bytes:")
    outputHex(K12.KangarooTwelve(b'', b'', 10032)[10000:])
    for i in range(7):
        C = b''
        M = bytearray([(j % 251) for j in range(17**i)])
        print("KangarooTwelve(M=pattern 0x00 to 0xFA for 17^{0:d} bytes, C=empty, 32 output bytes):".format(i))
        outputHex(K12.KangarooTwelve(M, C, 32))
    for i in range(4):
        M = bytearray([0xFF for j in range(2**i-1)])
        C = bytearray([(j % 251) for j in range(41**i)])
        print("KangarooTwelve(M={0:d} times byte 0xFF, C=pattern 0x00 to 0xFA for 41^{1:d} bytes, 32 output bytes):".format(2**i-1, i))
        outputHex(K12.KangarooTwelve(M, C, 32))

printTestVectors()
