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

#ifndef _PlSnP_Interface_h_
#define _PlSnP_Interface_h_

#include "SnP-interface.h"
#include "NumberOfParallelInstances.h"
#include "PlSnP-FBWL-default.h"

#define PlSnP_statesSizeInBytes             (PlSnP_P*SnP_stateSizeInBytes)

#define PlSnP_StaticInitialize              PlSnP_StaticInitialize_Serial
#define PlSnP_InitializeAll                 PlSnP_InitializeAll_Serial
#define PlSnP_XORBytes                      PlSnP_XORBytes_Serial
#define PlSnP_XORLanesAll                   PlSnP_XORLanesAll_Serial
#define PlSnP_OverwriteBytes                PlSnP_OverwriteBytes_Serial
#define PlSnP_OverwriteLanesAll             PlSnP_OverwriteLanesAll_Serial
#define PlSnP_OverwriteWithZeroes           PlSnP_OverwriteWithZeroes_Serial
#define PlSnP_ComplementBit                 PlSnP_ComplementBit_Serial
#define PlSnP_ComplementBitAll              PlSnP_ComplementBitAll_Serial
#define PlSnP_Permute                       PlSnP_Permute_Serial
#define PlSnP_PermuteAll                    PlSnP_PermuteAll_Serial
#define PlSnP_ExtractBytes                  PlSnP_ExtractBytes_Serial
#define PlSnP_ExtractLanesAll               PlSnP_ExtractLanesAll_Serial
#define PlSnP_ExtractAndXORBytes            PlSnP_ExtractAndXORBytes_Serial
#define PlSnP_ExtractAndXORLanesAll         PlSnP_ExtractAndXORLanesAll_Serial

#define PlSnP_FBWL_Absorb                   PlSnP_FBWL_Absorb_Default
#define PlSnP_FBWL_Squeeze                  PlSnP_FBWL_Squeeze_Default
#define PlSnP_FBWL_Wrap                     PlSnP_FBWL_Wrap_Default
#define PlSnP_FBWL_Unwrap                   PlSnP_FBWL_Unwrap_Default

#endif
