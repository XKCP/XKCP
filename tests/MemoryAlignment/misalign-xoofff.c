/*
Xoofff case of the misaligned-buffer test. It is in its own file because
Xoofff.h and Kravatte.h declare phase enumerators with the same names.
*/

#include <stddef.h>
#include "config.h"
#ifdef XKCP_has_Xoofff
#include "Xoofff.h"

int misalignXoofff(const unsigned char *in, size_t inLen, const unsigned char *key, size_t keyLen, unsigned char *out, size_t outLen)
{
    Xoofff_Instance xp;
    if (Xoofff_MaskDerivation(&xp, key, (BitLength)keyLen*8) != 0)
        return 1;
    return Xoofff(&xp, in, (BitLength)inLen*8, out, (BitLength)outLen*8, Xoofff_FlagInit);
}
#endif
