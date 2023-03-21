# -*- coding: utf-8 -*-
# Implementation by Gilles Van Assche, hereby denoted as "the implementer".
#
# For more information, feedback or questions, please refer to our website:
# https://keccak.team/
#
# To the extent possible under law, the implementer has waived all copyright
# and related or neighboring rights to the source code in this file.
# http://creativecommons.org/publicdomain/zero/1.0/

from TurboSHAKE import TurboSHAKE128

def right_encode(x):
    S = bytearray()
    while(x > 0):
        S = bytearray([x % 256]) + S
        x = x//256
    S = S + bytearray([len(S)])
    return S

# inputMessage and customizationString must be of type byte string or byte array
def KangarooTwelve(inputMessage, customizationString, outputByteLen):
    B = 8192
    c = 256
    S = bytearray(inputMessage) + bytearray(customizationString) + right_encode(len(customizationString))
    # === Cut the input string into chunks of B bytes ===
    n = (len(S)+B-1)//B
    Si = [bytearray(S[i*B:(i+1)*B]) for i in range(n)]
    if (n == 1):
        # === Process the tree with only a final node ===
        return TurboSHAKE128(Si[0], 0x07, outputByteLen)
    else:
        # === Process the tree with kangaroo hopping ===
        CVi = [TurboSHAKE128(Si[i+1], 0x0B, c//8) for i in range(n-1)]
        NodeStar = Si[0] + bytearray([3,0,0,0,0,0,0,0]) + bytearray().join(CVi) \
            + right_encode(n-1) + b'\xFF\xFF'
        return TurboSHAKE128(NodeStar, 0x06, outputByteLen)
