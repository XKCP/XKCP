/*
Implementation taken from:
https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
(2015-12-16)

Available under the Creative Commons Attribution-ShareAlike License:
https://creativecommons.org/licenses/by-sa/3.0/
*/

#include <string.h>

int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize);
