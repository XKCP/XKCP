# -*- coding: utf-8 -*-
# Implementation by Gilles Van Assche, hereby denoted as "the implementer".
#
# For more information, feedback or questions, please refer to our website:
# https://keccak.team/
#
# To the extent possible under law, the implementer has waived all copyright
# and related or neighboring rights to the source code in this file.
# http://creativecommons.org/publicdomain/zero/1.0/

import CompactFIPS202
import base64
import binascii
import io
import sys

instance=(1344, 256, 0x1F, 264)
printAsBase64=True
for arg in sys.argv[1:]:
    if (arg.startswith('--')):
        if (arg == '--hex'):
            printAsBase64 = False
        elif (arg == '--base64'):
            printAsBase64 = True
        elif (arg == '--shake128'):
            instance=(1344, 256, 0x1F, 264)
            printAsBase64=True
        elif (arg == '--shake256'):
            instance=(1088, 512, 0x1F, 528)
            printAsBase64=True
        elif (arg == '--sha3-224'):
            instance=(1152, 448, 0x06, 224)
            printAsBase64=False
        elif (arg == '--sha3-256'):
            instance=(1088, 512, 0x06, 256)
            printAsBase64=False
        elif (arg == '--sha3-384'):
            instance=(832, 768, 0x06, 384)
            printAsBase64=False
        elif (arg == '--sha3-512'):
            instance=(576, 1024, 0x06, 512)
            printAsBase64=False
        else:
            print('Unrecognized option:', arg)
    else:
        fileName = arg
        try:
            with open(fileName, 'rb') as f:
                b = bytearray(f.read())
                (r, c, s, n) = instance
                h = CompactFIPS202.Keccak(r, c, b, s, n//8)
                if (printAsBase64):
                    print(bytes.decode(base64.standard_b64encode(h)), '', fileName)
                else:
                    print(bytes.decode(binascii.hexlify(h)), '', fileName)
        except IOError:
            pass
        except:
            raise
