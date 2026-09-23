/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche and Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "config.h"
#ifdef XKCP_has_Duplex_Keccak

#ifndef XKCP_has_Sponge_Keccak
#error This test requires an implementation of the Keccak sponge
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "KeccakDuplex.h"
#include "KeccakSponge.h"
#include "UT.h"

#define flavor_DuplexingOnly    1
#define flavor_PartialIO        2
#define flavor_OverwriteAndAdd  3

#ifdef XKCP_has_Duplex_Keccak_width1600
    #define prefix KeccakWidth1600
    #define SnP_width 1600
    #include "testDuplex.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifdef UT_OUTPUT
void writeTestDuplex(void)
{
    FILE *f;
    unsigned int rate;

    f = fopen("TestDuplex.txt", "w");
    assert(f != NULL);
#ifdef XKCP_has_Duplex_Keccak_width1600
    for(rate = 3; rate <= 1600-2; rate += (rate < 68) ? 1 : ((rate < 220) ? 5 : 25))
        KeccakWidth1600_writeTestDuplex(f, rate, 1600-rate, flavor_DuplexingOnly);
#endif
    fclose(f);
}
#endif

void selfTestDuplex(unsigned int rate, unsigned int capacity, int flavor, const char *expected)
{
#ifdef XKCP_has_Duplex_Keccak_width1600
    if (rate+capacity == 1600)
        KeccakWidth1600_selfTestDuplex(rate, capacity, flavor, (const uint8_t*)expected);
    else
#endif
        abort();
}

void testDuplex()
{
    unsigned int flavor;
    const char *flavorString = 0;

#ifdef UT_OUTPUT
    writeTestDuplex();
#endif

    for(flavor=1; flavor<=3; flavor++) {
#if !defined(UT_EMBEDDED)
        if (flavor == flavor_DuplexingOnly)
            flavorString = "Testing duplexing in one call";
        else if (flavor == flavor_PartialIO)
            flavorString = "Testing duplexing with partial input/output";
        else if (flavor == flavor_OverwriteAndAdd)
            flavorString = "Testing duplexing with overwrite and add";
#endif
#ifdef XKCP_has_Duplex_Keccak_width1600
    UT_startTest("Keccak with width 1600", flavorString);
    selfTestDuplex(3, 1597, flavor, "\x0d\x85\xef\x10\xb7\x9f\x5e\xe0"); /* Keccak[r=3, c=1597] */
    selfTestDuplex(4, 1596, flavor, "\x0f\x9f\xf7\x73\x7d\x20\x3b\x64"); /* Keccak[r=4, c=1596] */
    selfTestDuplex(5, 1595, flavor, "\x50\x86\xe1\x23\xa3\x5e\xd2\xfa"); /* Keccak[r=5, c=1595] */
    selfTestDuplex(6, 1594, flavor, "\x5b\x41\x2c\xe8\xec\xc6\xca\x5e"); /* Keccak[r=6, c=1594] */
    selfTestDuplex(7, 1593, flavor, "\xcc\x5c\xf6\x13\x1a\x90\x56\x2a"); /* Keccak[r=7, c=1593] */
    selfTestDuplex(8, 1592, flavor, "\x20\x7e\x1c\x05\x2e\x3b\xa8\xf8"); /* Keccak[r=8, c=1592] */
    selfTestDuplex(9, 1591, flavor, "\x9a\xcc\xa1\xf4\x97\xc1\x38\x52"); /* Keccak[r=9, c=1591] */
    selfTestDuplex(10, 1590, flavor, "\xa7\x50\x5e\x83\xa1\xab\x5c\xa7"); /* Keccak[r=10, c=1590] */
    selfTestDuplex(11, 1589, flavor, "\xfa\x9a\x24\x02\x81\x7e\xac\x4a"); /* Keccak[r=11, c=1589] */
    selfTestDuplex(12, 1588, flavor, "\x8e\xec\xde\xee\x9b\xbc\xe3\x17"); /* Keccak[r=12, c=1588] */
    selfTestDuplex(13, 1587, flavor, "\x3c\x51\xe6\x12\xc9\x61\xce\x51"); /* Keccak[r=13, c=1587] */
    selfTestDuplex(14, 1586, flavor, "\x88\x27\x45\x7b\xb6\x69\x1d\xd0"); /* Keccak[r=14, c=1586] */
    selfTestDuplex(15, 1585, flavor, "\x23\xaa\x2f\xdc\x9e\x3d\x96\x06"); /* Keccak[r=15, c=1585] */
    selfTestDuplex(16, 1584, flavor, "\x55\x1f\x43\xe9\x80\xed\x8e\x58"); /* Keccak[r=16, c=1584] */
    selfTestDuplex(17, 1583, flavor, "\x53\x16\x86\xa6\x14\x39\xc0\x4b"); /* Keccak[r=17, c=1583] */
    selfTestDuplex(18, 1582, flavor, "\xba\xe1\x97\xf9\x1c\x71\xf9\xf5"); /* Keccak[r=18, c=1582] */
    selfTestDuplex(19, 1581, flavor, "\x93\x2f\x75\x2a\x35\x41\x98\x74"); /* Keccak[r=19, c=1581] */
    selfTestDuplex(20, 1580, flavor, "\x12\xa4\x13\x86\xa1\xea\x45\x5a"); /* Keccak[r=20, c=1580] */
    selfTestDuplex(21, 1579, flavor, "\x9a\x04\x09\x58\x0a\x1a\x94\xb8"); /* Keccak[r=21, c=1579] */
    selfTestDuplex(22, 1578, flavor, "\x2c\x1c\x08\x1f\x6f\xb6\xd7\xad"); /* Keccak[r=22, c=1578] */
    selfTestDuplex(23, 1577, flavor, "\x46\x1c\x7a\x7d\x4d\xc8\x3e\x5a"); /* Keccak[r=23, c=1577] */
    selfTestDuplex(24, 1576, flavor, "\x13\x6e\xc4\x38\x43\xb5\x21\x51"); /* Keccak[r=24, c=1576] */
    selfTestDuplex(25, 1575, flavor, "\x92\x96\x0e\x89\x56\xbd\xff\x78"); /* Keccak[r=25, c=1575] */
    selfTestDuplex(26, 1574, flavor, "\x2d\xb2\x13\x6f\x95\x4f\xbb\xe6"); /* Keccak[r=26, c=1574] */
    selfTestDuplex(27, 1573, flavor, "\xf1\x85\x51\x14\x64\x66\xa4\x70"); /* Keccak[r=27, c=1573] */
    selfTestDuplex(28, 1572, flavor, "\xfe\x58\x23\x0a\xd3\x07\x14\x88"); /* Keccak[r=28, c=1572] */
    selfTestDuplex(29, 1571, flavor, "\x22\xfe\x64\x51\x30\x19\xb2\x6d"); /* Keccak[r=29, c=1571] */
    selfTestDuplex(30, 1570, flavor, "\x30\xbf\xea\xcd\xd0\x02\x56\xe2"); /* Keccak[r=30, c=1570] */
    selfTestDuplex(31, 1569, flavor, "\x5f\xc8\xd0\x81\xae\x5e\x94\x7a"); /* Keccak[r=31, c=1569] */
    selfTestDuplex(32, 1568, flavor, "\x08\x8e\x89\x3b\x12\x8e\x29\x09"); /* Keccak[r=32, c=1568] */
    selfTestDuplex(33, 1567, flavor, "\xa8\x3b\xb0\xd1\x1c\x54\x9a\xf1"); /* Keccak[r=33, c=1567] */
    selfTestDuplex(34, 1566, flavor, "\xe9\x51\xad\x33\x01\x6d\x2a\x4b"); /* Keccak[r=34, c=1566] */
    selfTestDuplex(35, 1565, flavor, "\xb7\x5d\x8f\xce\x90\x32\x9d\x03"); /* Keccak[r=35, c=1565] */
    selfTestDuplex(36, 1564, flavor, "\xe3\xc4\x71\xf6\xfd\x42\x6f\xe4"); /* Keccak[r=36, c=1564] */
    selfTestDuplex(37, 1563, flavor, "\x0b\x26\xa7\x84\x34\xfb\x8c\x19"); /* Keccak[r=37, c=1563] */
    selfTestDuplex(38, 1562, flavor, "\x17\xf4\x24\x5b\x84\x3d\x38\xe8"); /* Keccak[r=38, c=1562] */
    selfTestDuplex(39, 1561, flavor, "\xbc\xe1\x63\x27\x55\x9a\x7c\x37"); /* Keccak[r=39, c=1561] */
    selfTestDuplex(40, 1560, flavor, "\xf9\x68\x98\x88\x51\xb6\xbe\x49"); /* Keccak[r=40, c=1560] */
    selfTestDuplex(41, 1559, flavor, "\x66\x66\x0e\x24\x13\x96\x4e\x06"); /* Keccak[r=41, c=1559] */
    selfTestDuplex(42, 1558, flavor, "\x33\x20\x6d\x45\x57\xb5\x7f\xe4"); /* Keccak[r=42, c=1558] */
    selfTestDuplex(43, 1557, flavor, "\xa4\x75\xbe\x73\x37\x36\xbc\xe4"); /* Keccak[r=43, c=1557] */
    selfTestDuplex(44, 1556, flavor, "\x62\x21\x17\x44\x35\x06\x8a\x66"); /* Keccak[r=44, c=1556] */
    selfTestDuplex(45, 1555, flavor, "\x8c\x6a\xc1\x33\x71\x8c\xd5\xf9"); /* Keccak[r=45, c=1555] */
    selfTestDuplex(46, 1554, flavor, "\x9d\xfc\xb7\xaf\x8d\xe7\x0e\xd5"); /* Keccak[r=46, c=1554] */
    selfTestDuplex(47, 1553, flavor, "\xf8\x95\x0e\xae\x24\x24\xcd\x31"); /* Keccak[r=47, c=1553] */
    selfTestDuplex(48, 1552, flavor, "\xe3\xfc\x9e\x05\xd3\x65\x34\xa9"); /* Keccak[r=48, c=1552] */
    selfTestDuplex(49, 1551, flavor, "\x51\xa9\x45\x96\x1a\x32\x34\x1d"); /* Keccak[r=49, c=1551] */
    selfTestDuplex(50, 1550, flavor, "\x5a\x7a\x87\xbc\xa5\x66\x8c\x95"); /* Keccak[r=50, c=1550] */
    selfTestDuplex(51, 1549, flavor, "\x75\xbd\xd2\xa1\xde\xc9\xac\x22"); /* Keccak[r=51, c=1549] */
    selfTestDuplex(52, 1548, flavor, "\x72\x49\x0a\xb8\xd7\xad\xaa\x12"); /* Keccak[r=52, c=1548] */
    selfTestDuplex(53, 1547, flavor, "\x1b\xf5\x6f\x1f\xef\xa8\x8a\xe8"); /* Keccak[r=53, c=1547] */
    selfTestDuplex(54, 1546, flavor, "\xb2\x39\xb9\xf4\x2d\x37\xe6\x69"); /* Keccak[r=54, c=1546] */
    selfTestDuplex(55, 1545, flavor, "\x1c\xe1\x02\x2c\xb0\x6b\x8a\x6e"); /* Keccak[r=55, c=1545] */
    selfTestDuplex(56, 1544, flavor, "\xe5\x60\x71\x95\xa6\xf3\xd4\xf4"); /* Keccak[r=56, c=1544] */
    selfTestDuplex(57, 1543, flavor, "\xba\x8b\xfb\x20\x6f\xf9\x1e\x4d"); /* Keccak[r=57, c=1543] */
    selfTestDuplex(58, 1542, flavor, "\x06\xb2\x2e\x8c\x45\x36\x77\x4a"); /* Keccak[r=58, c=1542] */
    selfTestDuplex(59, 1541, flavor, "\x20\x52\x9d\x35\x0f\x73\x1b\x11"); /* Keccak[r=59, c=1541] */
    selfTestDuplex(60, 1540, flavor, "\x01\x13\xc8\x48\x3f\x43\x55\xac"); /* Keccak[r=60, c=1540] */
    selfTestDuplex(61, 1539, flavor, "\x6b\xf5\x20\x74\xf5\x74\x66\x20"); /* Keccak[r=61, c=1539] */
    selfTestDuplex(62, 1538, flavor, "\xc0\x16\x35\xe3\x62\xdf\xf3\x32"); /* Keccak[r=62, c=1538] */
    selfTestDuplex(63, 1537, flavor, "\x85\xea\xb4\x37\x79\x28\x78\xfd"); /* Keccak[r=63, c=1537] */
    selfTestDuplex(64, 1536, flavor, "\x42\x7f\x91\xfb\x62\xdb\xb1\xed"); /* Keccak[r=64, c=1536] */
    selfTestDuplex(65, 1535, flavor, "\xa3\x59\x56\xb2\x1b\x23\xd2\xd7"); /* Keccak[r=65, c=1535] */
    selfTestDuplex(66, 1534, flavor, "\x42\xc7\x13\x2c\x5b\x3c\x86\x2a"); /* Keccak[r=66, c=1534] */
    selfTestDuplex(67, 1533, flavor, "\x7e\xcf\x89\xcc\xbd\x2c\x75\x6e"); /* Keccak[r=67, c=1533] */
    selfTestDuplex(68, 1532, flavor, "\xd1\xe0\xb3\xad\xe0\xf3\x14\x94"); /* Keccak[r=68, c=1532] */
    selfTestDuplex(73, 1527, flavor, "\xbc\x4d\xed\x17\xdb\x81\x1b\x9e"); /* Keccak[r=73, c=1527] */
    selfTestDuplex(78, 1522, flavor, "\xd5\xad\xa2\x59\xf0\xb3\xec\x16"); /* Keccak[r=78, c=1522] */
    selfTestDuplex(83, 1517, flavor, "\x2f\x94\xdf\x15\xb1\x96\x0b\x8e"); /* Keccak[r=83, c=1517] */
    selfTestDuplex(88, 1512, flavor, "\x43\x89\x89\x58\xf1\x86\x70\xd3"); /* Keccak[r=88, c=1512] */
    selfTestDuplex(93, 1507, flavor, "\xc2\xf9\xf5\xa0\xbc\x38\x28\x5c"); /* Keccak[r=93, c=1507] */
    selfTestDuplex(98, 1502, flavor, "\x66\x51\x77\x34\x3b\x59\x1a\xb8"); /* Keccak[r=98, c=1502] */
    selfTestDuplex(103, 1497, flavor, "\x1c\x76\xe2\xc9\x1a\x3b\x00\xc9"); /* Keccak[r=103, c=1497] */
    selfTestDuplex(108, 1492, flavor, "\x29\x9a\x1f\x24\x97\xeb\x83\xc5"); /* Keccak[r=108, c=1492] */
    selfTestDuplex(113, 1487, flavor, "\x5a\x8c\xcf\xd6\xda\x9e\x11\xec"); /* Keccak[r=113, c=1487] */
    selfTestDuplex(118, 1482, flavor, "\x4f\x60\x19\xf3\x2a\xf6\x5a\xba"); /* Keccak[r=118, c=1482] */
    selfTestDuplex(123, 1477, flavor, "\xcc\x87\x0c\x18\x87\x00\x35\x98"); /* Keccak[r=123, c=1477] */
    selfTestDuplex(128, 1472, flavor, "\xe4\xa5\x8c\xaf\x8a\xea\x3e\xdb"); /* Keccak[r=128, c=1472] */
    selfTestDuplex(133, 1467, flavor, "\xee\xb6\x68\xc1\x3e\x56\xe1\xab"); /* Keccak[r=133, c=1467] */
    selfTestDuplex(138, 1462, flavor, "\x3c\xa5\xdc\x53\x6d\x0d\xb8\x83"); /* Keccak[r=138, c=1462] */
    selfTestDuplex(143, 1457, flavor, "\xc6\x31\xb2\xdc\xa0\x35\x4b\x6b"); /* Keccak[r=143, c=1457] */
    selfTestDuplex(148, 1452, flavor, "\x73\xaa\x16\xa7\x3b\xcc\x4d\xf1"); /* Keccak[r=148, c=1452] */
    selfTestDuplex(153, 1447, flavor, "\xad\x80\xf5\xe9\xbe\xde\xd2\x31"); /* Keccak[r=153, c=1447] */
    selfTestDuplex(158, 1442, flavor, "\xae\x13\x52\x3e\xa5\x86\x89\x1f"); /* Keccak[r=158, c=1442] */
    selfTestDuplex(163, 1437, flavor, "\x25\xee\x23\x8d\x4c\x6e\x04\xfb"); /* Keccak[r=163, c=1437] */
    selfTestDuplex(168, 1432, flavor, "\x3b\x51\x9d\x74\x04\x4c\x1b\xa3"); /* Keccak[r=168, c=1432] */
    selfTestDuplex(173, 1427, flavor, "\xb7\xe7\x81\xac\xf1\x0f\xb3\xd7"); /* Keccak[r=173, c=1427] */
    selfTestDuplex(178, 1422, flavor, "\x07\x01\xb1\xbc\xb2\xe3\x24\xec"); /* Keccak[r=178, c=1422] */
    selfTestDuplex(183, 1417, flavor, "\x0d\x0f\xf1\x56\x08\x5c\x41\x9c"); /* Keccak[r=183, c=1417] */
    selfTestDuplex(188, 1412, flavor, "\xe7\x39\x02\x70\xe8\x9a\xfa\x96"); /* Keccak[r=188, c=1412] */
    selfTestDuplex(193, 1407, flavor, "\x06\x26\x83\x6d\x72\x84\xa6\xda"); /* Keccak[r=193, c=1407] */
    selfTestDuplex(198, 1402, flavor, "\xd7\x4f\x56\x6d\x21\x4a\xf4\x64"); /* Keccak[r=198, c=1402] */
    selfTestDuplex(203, 1397, flavor, "\x6c\x06\x8f\xc3\x94\x73\xd4\x7e"); /* Keccak[r=203, c=1397] */
    selfTestDuplex(208, 1392, flavor, "\xbb\x96\x81\x5d\x42\xc8\xb4\x76"); /* Keccak[r=208, c=1392] */
    selfTestDuplex(213, 1387, flavor, "\x6c\x4c\x44\x9e\x13\x7c\x60\x45"); /* Keccak[r=213, c=1387] */
    selfTestDuplex(218, 1382, flavor, "\xea\x89\xc6\xfb\xa5\xef\x88\x22"); /* Keccak[r=218, c=1382] */
    selfTestDuplex(223, 1377, flavor, "\xb9\xd5\xe3\x0f\x60\x33\x75\x35"); /* Keccak[r=223, c=1377] */
    selfTestDuplex(248, 1352, flavor, "\x0d\xb6\x1f\xaa\xbb\x4f\x79\x01"); /* Keccak[r=248, c=1352] */
    selfTestDuplex(273, 1327, flavor, "\xe7\x38\x00\x8f\x51\xa5\xd4\xd9"); /* Keccak[r=273, c=1327] */
    selfTestDuplex(298, 1302, flavor, "\xc5\x15\x0d\x8a\xb7\x6a\xf2\x0c"); /* Keccak[r=298, c=1302] */
    selfTestDuplex(323, 1277, flavor, "\x1f\x2d\xa8\xaf\x3c\xa7\x48\x25"); /* Keccak[r=323, c=1277] */
    selfTestDuplex(348, 1252, flavor, "\xff\xea\x64\xf9\xfa\x89\xa8\x82"); /* Keccak[r=348, c=1252] */
    selfTestDuplex(373, 1227, flavor, "\x55\x15\xc0\x8f\xa6\x20\x38\x7f"); /* Keccak[r=373, c=1227] */
    selfTestDuplex(398, 1202, flavor, "\x22\xce\x93\x1f\xcf\x5f\x4d\x0c"); /* Keccak[r=398, c=1202] */
    selfTestDuplex(423, 1177, flavor, "\xba\xeb\x41\x8c\x8b\x00\x6c\x5e"); /* Keccak[r=423, c=1177] */
    selfTestDuplex(448, 1152, flavor, "\x9c\x04\xa0\x6a\xd4\x67\xcc\xdc"); /* Keccak[r=448, c=1152] */
    selfTestDuplex(473, 1127, flavor, "\xe5\x48\xd6\x6c\x00\xf1\xc3\x86"); /* Keccak[r=473, c=1127] */
    selfTestDuplex(498, 1102, flavor, "\xf7\xca\x7f\xb5\xae\xa3\x2d\xa7"); /* Keccak[r=498, c=1102] */
    selfTestDuplex(523, 1077, flavor, "\x79\x91\x6e\x50\x13\xf6\x40\x66"); /* Keccak[r=523, c=1077] */
    selfTestDuplex(548, 1052, flavor, "\xf0\xb3\x13\xe5\x59\xff\xe2\xfc"); /* Keccak[r=548, c=1052] */
    selfTestDuplex(573, 1027, flavor, "\xa7\x79\x4f\x99\x03\xad\x0b\xe7"); /* Keccak[r=573, c=1027] */
    selfTestDuplex(598, 1002, flavor, "\xb5\x13\x0d\x4d\x46\xfa\x51\xd1"); /* Keccak[r=598, c=1002] */
    selfTestDuplex(623, 977, flavor, "\x26\x8c\xcf\x07\xa0\xb0\xbe\x5b"); /* Keccak[r=623, c=977] */
    selfTestDuplex(648, 952, flavor, "\x33\x58\xde\xc1\x9f\x72\xd9\x7f"); /* Keccak[r=648, c=952] */
    selfTestDuplex(673, 927, flavor, "\xa3\xc0\x45\xf5\xb1\x2e\xe9\x08"); /* Keccak[r=673, c=927] */
    selfTestDuplex(698, 902, flavor, "\x58\xd7\xf3\x0e\xdb\xf5\x67\xf1"); /* Keccak[r=698, c=902] */
    selfTestDuplex(723, 877, flavor, "\x72\x1a\xbe\xba\xd6\xe7\x68\x3f"); /* Keccak[r=723, c=877] */
    selfTestDuplex(748, 852, flavor, "\xb9\xb8\x46\xf1\xb9\x9c\x22\x68"); /* Keccak[r=748, c=852] */
    selfTestDuplex(773, 827, flavor, "\xec\x0e\xc7\xe7\x10\x54\xff\x72"); /* Keccak[r=773, c=827] */
    selfTestDuplex(798, 802, flavor, "\x4e\xe2\xfc\xc3\x49\x9c\xa4\x08"); /* Keccak[r=798, c=802] */
    selfTestDuplex(823, 777, flavor, "\xd8\x33\xe2\x95\xd8\x99\xdd\x91"); /* Keccak[r=823, c=777] */
    selfTestDuplex(848, 752, flavor, "\x37\xb3\x1e\xe4\x55\x21\x14\xc4"); /* Keccak[r=848, c=752] */
    selfTestDuplex(873, 727, flavor, "\x99\xca\x45\xe1\xc9\x23\x63\xdd"); /* Keccak[r=873, c=727] */
    selfTestDuplex(898, 702, flavor, "\x4f\x2d\x55\x38\x68\x96\xa9\x08"); /* Keccak[r=898, c=702] */
    selfTestDuplex(923, 677, flavor, "\x5c\x6b\xd7\xf0\x95\x16\x6c\x9d"); /* Keccak[r=923, c=677] */
    selfTestDuplex(948, 652, flavor, "\xb7\x56\x18\x4d\x61\x77\x83\x4b"); /* Keccak[r=948, c=652] */
    selfTestDuplex(973, 627, flavor, "\x65\x28\x9a\x19\x1c\xed\x06\x96"); /* Keccak[r=973, c=627] */
    selfTestDuplex(998, 602, flavor, "\x68\xeb\xa2\x02\x77\x9a\x96\xe3"); /* Keccak[r=998, c=602] */
    selfTestDuplex(1023, 577, flavor, "\x1e\x4b\xb4\x7f\x29\x64\x0d\x7e"); /* Keccak[r=1023, c=577] */
    selfTestDuplex(1048, 552, flavor, "\xca\x8c\xdc\x76\xe6\x19\x13\x4e"); /* Keccak[r=1048, c=552] */
    selfTestDuplex(1073, 527, flavor, "\x70\x8a\xbc\xfc\x89\x03\xc6\x67"); /* Keccak[r=1073, c=527] */
    selfTestDuplex(1098, 502, flavor, "\xf3\x6b\x2e\x1a\xf5\x71\xf4\xcb"); /* Keccak[r=1098, c=502] */
    selfTestDuplex(1123, 477, flavor, "\xd0\x60\x08\x29\xd3\xd0\xf4\x22"); /* Keccak[r=1123, c=477] */
    selfTestDuplex(1148, 452, flavor, "\x08\x5b\x51\xc5\x61\x52\x1b\x43"); /* Keccak[r=1148, c=452] */
    selfTestDuplex(1173, 427, flavor, "\x7b\x71\xe2\x1d\x63\xeb\x3f\x80"); /* Keccak[r=1173, c=427] */
    selfTestDuplex(1198, 402, flavor, "\x8c\x81\xec\x7d\xc0\x36\xe8\xde"); /* Keccak[r=1198, c=402] */
    selfTestDuplex(1223, 377, flavor, "\xa9\x78\xbb\xdf\xcb\x2b\xda\x55"); /* Keccak[r=1223, c=377] */
    selfTestDuplex(1248, 352, flavor, "\x86\x8d\x6b\xc7\x4d\xef\x18\xd2"); /* Keccak[r=1248, c=352] */
    selfTestDuplex(1273, 327, flavor, "\x40\x6c\xdf\xa0\x7a\x43\x5b\xb3"); /* Keccak[r=1273, c=327] */
    selfTestDuplex(1298, 302, flavor, "\xad\x92\x7d\x83\xf8\x22\xdd\xa0"); /* Keccak[r=1298, c=302] */
    selfTestDuplex(1323, 277, flavor, "\x37\x22\x0c\x42\x82\x79\x86\x07"); /* Keccak[r=1323, c=277] */
    selfTestDuplex(1348, 252, flavor, "\x7c\x61\x90\x43\x8c\x9b\xef\x74"); /* Keccak[r=1348, c=252] */
    selfTestDuplex(1373, 227, flavor, "\xda\xc2\x06\x1a\xee\x8c\x6f\xd7"); /* Keccak[r=1373, c=227] */
    selfTestDuplex(1398, 202, flavor, "\xf0\x49\x79\xa9\x5b\x15\xde\xb1"); /* Keccak[r=1398, c=202] */
    selfTestDuplex(1423, 177, flavor, "\x05\x54\xaf\xee\xaa\xdd\x6b\x0a"); /* Keccak[r=1423, c=177] */
    selfTestDuplex(1448, 152, flavor, "\xb7\x42\x50\x42\xcd\x6a\x20\xb6"); /* Keccak[r=1448, c=152] */
    selfTestDuplex(1473, 127, flavor, "\x56\xcf\x98\x1b\xad\x83\x96\xb2"); /* Keccak[r=1473, c=127] */
    selfTestDuplex(1498, 102, flavor, "\xeb\x6e\xb2\x8f\x51\xf7\xf7\xe6"); /* Keccak[r=1498, c=102] */
    selfTestDuplex(1523, 77, flavor, "\x85\x8b\xe0\x4e\x47\x5b\xc6\x74"); /* Keccak[r=1523, c=77] */
    selfTestDuplex(1548, 52, flavor, "\x0b\x24\x99\x0d\xd2\x05\x2c\x2d"); /* Keccak[r=1548, c=52] */
    selfTestDuplex(1573, 27, flavor, "\x7f\x3f\x68\xeb\x74\x29\x23\x4c"); /* Keccak[r=1573, c=27] */
    selfTestDuplex(1598, 2, flavor, "\xe0\x17\xcc\x63\x9f\xc5\x57\x5f"); /* Keccak[r=1598, c=2] */
    UT_endTest();
#endif
    }
}
#endif /* XKCP_has_Duplex_Keccak */
