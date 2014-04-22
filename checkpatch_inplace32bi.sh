#!/usr/bin/env bash
git checkout HEAD -- KeccakF-1600/Optimized/KeccakF-1600-inplace32BI.c
gsed -ri -f - KeccakF-1600/Optimized/KeccakF-1600-inplace32BI.c <<'EOSED'
s/^([ 	]*toBitInterleavingAndXOR[^;]*);*/\1;/;
s/^([ 	]*fromBitInterleaving[^;]*);*/\1;/;
EOSED
cat - > interleaved.cocci <<'EOCOCCI'
@ toandxor @
expression pI, pS;
expression t, x0, x1;
@@
-toBitInterleavingAndXOR(*(pI++), *(pI++), *(pS++), *(pS++), t, x0, x1);
+UINT32 low, high;
+memcpy(&low, pI++, 4);
+memcpy(&high, pI++, 4);
+toBitInterleavingAndXOR(low, high, *(pS++), *(pS++), t, x0, x1);

@ fromandxor @
expression pI, pS;
expression t, x0, x1;
@@
-fromBitInterleaving(*(pS++), *(pS++), *(pI++), *(pI++), t, x0, x1);
+UINT32 low, high;
+fromBitInterleaving(*(pS++), *(pS++), low, high, t, x0, x1);
+memcpy(pI++, &low, 4);
+memcpy(pI++, &high, 4);
EOCOCCI
spatch --sp-file interleaved.cocci --in-place KeccakF-1600/Optimized/KeccakF-1600-inplace32BI.c
git diff HEAD -- KeccakF-1600/Optimized/KeccakF-1600-inplace32BI.c
