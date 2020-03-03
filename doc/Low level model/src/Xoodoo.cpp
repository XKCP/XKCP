/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

The Xoodoo permutation, designed by Joan Daemen, Seth Hoffert, Gilles Van Assche and Ronny Van Keer.

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <sstream>
#include "Exception.h"
#include "Xoodoo.h"

using namespace std;

Xoodoo::Xoodoo()
    : sizeX(4), sizeZ(32), p(2, 8, 5, 14, 11)
{
	thetaOrder = computeThetaOrder();
}

unsigned int Xoodoo::getWidth() const
{
    return sizeX*sizeY*sizeZ;
}

void Xoodoo::round(XoodooState& state, LaneValue rc) const
{
    theta(state);
    rhoWest(state);
    state.getLanes()[0] ^= rc;
    chi(state);
    rhoEast(state);
}

void Xoodoo::permute(XoodooState& state, unsigned int numberOfRounds) const
{
    const LaneValue rc[12] = {
        0x00000058,
        0x00000038,
        0x000003C0,
        0x000000D0,
        0x00000120,
        0x00000014,
        0x00000060,
        0x0000002C,
        0x00000380,
        0x000000F0,
        0x000001A0,
        0x00000012
    };
    if (numberOfRounds > 12)
        throw Exception("Unsupported number of rounds");
    for(int i=1-(int)numberOfRounds; i<=0; i++)
        round(state, rc[i+11]);
}

void Xoodoo::apply(std::vector<std::uint8_t>& state, unsigned int roundCount) const
{
    XoodooState xoodoostate(*this);
    vector<LaneValue>& lanes = xoodoostate.getLanes();
    for(unsigned int y=0; y<sizeY; y++)
    for(unsigned int x=0; x<sizeX; x++) {
        unsigned int iprime = sizeY*x + y;
        unsigned int i = sizeX*y + x;
        lanes[iprime] = 0;
        for(unsigned int j=0; j<(sizeZ/8); j++)
            lanes[iprime] |= LaneValue((uint8_t)(state[i*sizeZ/8+j])) << (8*j);
    }
    permute(xoodoostate, roundCount);
    for(unsigned int y=0; y<sizeY; y++)
    for(unsigned int x=0; x<sizeX; x++) {
        unsigned int iprime = sizeY*x + y;
        unsigned int i = sizeX*y + x;
        for(unsigned int j=0; j<(sizeZ/8); j++)
            state[i*(sizeZ/8)+j] = (uint8_t)((lanes[iprime] >> (8*j)) & 0xFF);
    }
}

string Xoodoo::getDescription() const
{
    stringstream a;
    a << "Xoodoo["
        << dec << sizeX << "\xC3\x97" << sizeY << "\xC3\x97" << sizeZ
        << ", e0=" << dec << p.e0
        << ", e1=" << dec << p.e1
		<< ", t1=" << dec << p.t1
		<< ", t2=" << dec << p.t2
		<< ", t3=" << dec << p.t3
		<< ", w1=" << dec << p.w1
		<< "]";
    return a.str();
}

void Xoodoo::rhoEast(XoodooState& state) const
{
    vector<LaneValue>& lanes = state.getLanes();
    for(unsigned int x=0; x<sizeX; x++)
        lanes[1+3*x] = translateZ(lanes[1+3*x], 1);
    vector<LaneValue> temp(sizeX);
    for(unsigned int x=0; x<sizeX; x++) {
        int xprime = x+p.e0;
        reduceX(xprime);
        temp[xprime] = translateZ(lanes[2+3*x], p.e1);
    }
    for(unsigned int x=0; x<sizeX; x++)
        lanes[2+3*x] = temp[x];
}

void Xoodoo::rhoEast(int x, int y, int z, int& X, int& Y, int& Z) const
{
    reduceY(y);
    switch(y) {
        case 0:
            X = x;
            Y = y;
            Z = z;
            break;
        case 1:
            X = x;
            Y = y;
            Z = z+1;
            break;
        case 2:
            X = x+p.e0;
            Y = y;
            Z = z+p.e1;
            break;
    }
    reduceXYZ(X, Y, Z);
}

void Xoodoo::inverseRhoEast(XoodooState& state) const
{
    vector<LaneValue>& lanes = state.getLanes();
    for(unsigned int x=0; x<sizeX; x++)
        lanes[1+3*x] = translateZ(lanes[1+3*x], -1);
    vector<LaneValue> temp(sizeX);
    for(unsigned int x=0; x<sizeX; x++) {
        int xprime = x-p.e0;
        reduceX(xprime);
        temp[xprime] = translateZ(lanes[2+3*x], -p.e1);
    }
    for(unsigned int x=0; x<sizeX; x++)
        lanes[2+3*x] = temp[x];
}

void Xoodoo::inverseRhoEast(int X, int Y, int Z, int& x, int& y, int& z) const
{
    reduceY(Y);
    switch(Y) {
        case 0:
            x = X;
            y = Y;
            z = Z;
            break;
        case 1:
            x = X;
            y = Y;
            z = Z-1;
            break;
        case 2:
            x = X-p.e0;
            y = Y;
            z = Z-p.e1;
            break;
    }
    reduceXYZ(x, y, z);
}

void Xoodoo::rhoWest(XoodooState& state) const
{
    vector<LaneValue>& lanes = state.getLanes();
    vector<LaneValue> temp(sizeX);
    for(unsigned int x=0; x<sizeX; x++) {
        int xprime = x+1;
        reduceX(xprime);
        temp[xprime] = lanes[1+3*x];
    }
    for(unsigned int x=0; x<sizeX; x++)
        lanes[1+3*x] = temp[x];
    for(unsigned int x=0; x<sizeX; x++)
        lanes[2+3*x] = translateZ(lanes[2+3*x], p.w1);
}

void Xoodoo::rhoWest(int x, int y, int z, int& X, int& Y, int& Z) const
{
    reduceY(y);
    switch(y) {
        case 0:
            X = x;
            Y = y;
            Z = z;
            break;
        case 1:
            X = x+1;
            Y = y;
            Z = z;
            break;
        case 2:
            X = x;
            Y = y;
            Z = z+p.w1;
            break;
    }
    reduceXYZ(X, Y, Z);
}

void Xoodoo::inverseRhoWest(XoodooState& state) const
{
    vector<LaneValue>& lanes = state.getLanes();
    vector<LaneValue> temp(sizeX);
    for(unsigned int x=0; x<sizeX; x++) {
        int xprime = x-1;
        reduceX(xprime);
        temp[xprime] = lanes[1+3*x];
    }
    for(unsigned int x=0; x<sizeX; x++)
        lanes[1+3*x] = temp[x];
    for(unsigned int x=0; x<sizeX; x++)
        lanes[2+3*x] = translateZ(lanes[2+3*x], -p.w1);
}

void Xoodoo::inverseRhoWest(int X, int Y, int Z, int& x, int& y, int& z) const
{
    reduceY(Y);
    switch(Y) {
        case 0:
            x = X;
            y = Y;
            z = Z;
            break;
        case 1:
            x = X-1;
            y = Y;
            z = Z;
            break;
        case 2:
            x = X;
            y = Y;
            z = Z-p.w1;
            break;
    }
    reduceXYZ(x, y, z);
}

unsigned int Xoodoo::computeThetaOrder() const
{
    unsigned int oddPartSizeX = sizeX;
    unsigned int powerTwoPartSizeX = 1;
    while ( (oddPartSizeX & 1) == 0 ) {
        oddPartSizeX >>= 1;
		powerTwoPartSizeX <<= 1;
	}
    unsigned int order = sizeZ;
	if (powerTwoPartSizeX > order) order = powerTwoPartSizeX;
    switch(oddPartSizeX) {
        case 1 :
            break;
        case 3 :
            order *= 3;
            break;
        case 5 :
            order *= 15;
            break;
        case 7 :
            order *= 7;
			break;
		throw Exception((string)"sizeX is not a power of two times 3, 5 or 7");
    }
    return order;
}

void Xoodoo::theta(XoodooState& state) const
{
	vector<LaneValue>& lanes = state.getLanes();
	vector<LaneValue> parity(sizeX);
	vector<LaneValue> effect(sizeX);
	for (unsigned int x = 0; x < sizeX; x++) parity[x] = lanes[3 * x] ^ lanes[1 + 3 * x] ^ lanes[2 + 3 * x];
	for (unsigned int x = 0; x < sizeX; x++) {
		int x1 = x - 1; reduceX(x1);
		int x2 = x - p.t3; reduceX(x2);
		effect[x] = translateZ(parity[x1], p.t1) ^ translateZ(parity[x2], p.t2);
	}
    for(unsigned int x=0; x<sizeX; x++) {
        lanes[3*x] ^= effect[x];
        lanes[3*x+1] ^= effect[x];
        lanes[3*x+2] ^= effect[x];
	}
}


void Xoodoo::inverseTheta(XoodooState& state) const
{
	vector<LaneValue>& lanes = state.getLanes();
	vector<LaneValue> parity(sizeX);
	for (unsigned int x = 0; x<sizeX; x++) parity[x] = lanes[3 * x] ^ lanes[1 + 3 * x] ^ lanes[2 + 3 * x];
	unsigned int exponent = thetaOrder - 1;
	unsigned int powerTwo = 1;
	vector<LaneValue> effect = parity;
	do {
		if ((exponent&powerTwo) != 0) {
			vector<LaneValue> temp = effect;
			for (unsigned int x = 0; x < sizeX; x++) {
				int x1 = x - powerTwo; reduceX(x1);
				int x2 = x - p.t3*powerTwo; reduceX(x2);
				effect[x] = temp[x] ^ translateZ(temp[x1], p.t1*powerTwo) ^ translateZ(temp[x2], p.t2*powerTwo);
			}
		}
		powerTwo <<= 1;
	} while (powerTwo <= exponent);
	for (unsigned int x = 0; x<sizeX; x++) effect[x] ^= parity[x];
	for (unsigned int x = 0; x<sizeX; x++) {
		lanes[3 * x] ^= effect[x];
		lanes[3 * x + 1] ^= effect[x];
		lanes[3 * x + 2] ^= effect[x];
	}
}

void Xoodoo::thetaTransposed(XoodooState& state) const
{
    vector<LaneValue>& lanes = state.getLanes();
    vector<LaneValue> parity(sizeX);
    vector<LaneValue> effect(sizeX);
    for(unsigned int x=0; x<sizeX; x++) parity[x] = lanes[3*x] ^ lanes[1+3*x] ^ lanes[2+3*x];
	for (unsigned int x = 0; x < sizeX; x++) {
		int x1 = x + 1; reduceX(x1);
		int x2 = x + p.t3; reduceX(x2);
		effect[x] = translateZ(parity[x1], -p.t1) ^ translateZ(parity[x2], -p.t2);
	}
    for(unsigned int x=0; x<sizeX; x++) {
        lanes[3*x] ^= effect[x];
        lanes[3*x+1] ^= effect[x];
        lanes[3*x+2] ^= effect[x];
	}
}


void Xoodoo::inverseThetaTransposed(XoodooState& state) const
{
    vector<LaneValue>& lanes = state.getLanes();
    vector<LaneValue> parity(sizeX);
    for(unsigned int x=0; x<sizeX; x++) parity[x] = lanes[3*x] ^ lanes[1+3*x] ^ lanes[2+3*x];
	unsigned int exponent = thetaOrder - 1;
	unsigned int powerTwo = 1;
    vector<LaneValue> effect = parity;
	do {
		if( (exponent&powerTwo) != 0 ) {
            vector<LaneValue> temp = effect;
			for (unsigned int x = 0; x < sizeX; x++) {
				int x1 = x + powerTwo; reduceX(x1);
				int x2 = x + p.t3*powerTwo; reduceX(x2);
				effect[x] = temp[x] ^ translateZ(temp[x1], -p.t1*powerTwo) ^ translateZ(temp[x2], -p.t2*powerTwo);
			}
		}
		powerTwo <<= 1;
	} while ( powerTwo <= exponent );
    for(unsigned int x=0; x<sizeX; x++) effect[x] ^= parity[x];
    for(unsigned int x=0; x<sizeX; x++) {
        lanes[3*x] ^= effect[x];
        lanes[3*x+1] ^= effect[x];
        lanes[3*x+2] ^= effect[x];
	}
}

void Xoodoo::translateXZ(int& x, int& z, int dx, int dz) const
{
    x += dx;
    reduceX(x);
    z += dz;
    reduceZ(z);
}

void Xoodoo::reduceX(int& X) const
{
    X = ((X%(int)sizeX)+(int)sizeX)%(int)sizeX;
}

void Xoodoo::reduceY(int& Y) const
{
    Y = ((Y%(int)sizeY)+(int)sizeY)%(int)sizeY;
}

void Xoodoo::reduceZ(int& Z) const
{
    Z = ((Z%(int)sizeZ)+(int)sizeZ)%(int)sizeZ;
}

void Xoodoo::reduceXYZ(int& X, int& Y, int& Z) const
{
    reduceX(X);
    reduceY(Y);
    reduceZ(Z);
}

void Xoodoo::chi(XoodooState& state) const
{
    vector<LaneValue>& lanes = state.getLanes();
    for(unsigned int x=0; x<sizeX; x++) {
        LaneValue lane0 = lanes[0 + 3*x] ^ ((~lanes[1 + 3*x]) & lanes[2 + 3*x]);
        LaneValue lane1 = lanes[1 + 3*x] ^ ((~lanes[2 + 3*x]) & lanes[0 + 3*x]);
        LaneValue lane2 = lanes[2 + 3*x] ^ ((~lanes[0 + 3*x]) & lanes[1 + 3*x]);
        lanes[0 + 3*x] = lane0;
        lanes[1 + 3*x] = lane1;
        lanes[2 + 3*x] = lane2;
    }
}

void Xoodoo::inverseChi(XoodooState& state) const
{
    chi(state);
}

void XoodooLanes::clear()
{
    lanes.assign(lanes.size(), 0);
}

void XoodooLanes::invert()
{
    for(unsigned int i=0; i<lanes.size(); i++)
        lanes[i] = ~lanes[i];
}

XoodooLanes& XoodooLanes::operator^=(const XoodooLanes& other)
{
    for(unsigned int i=0; i<lanes.size(); i++)
        lanes[i] ^= other.lanes[i];
    return *this;
}

XoodooLanes& XoodooLanes::operator&=(const XoodooLanes& other)
{
    for(unsigned int i=0; i<lanes.size(); i++)
        lanes[i] &= other.lanes[i];
    return *this;
}

XoodooLanes& XoodooLanes::operator|=(const XoodooLanes& other)
{
    for(unsigned int i=0; i<lanes.size(); i++)
        lanes[i] |= other.lanes[i];
    return *this;
}

bool XoodooLanes::isZero() const
{
    for(unsigned int i=0; i<lanes.size(); i++) {
        if (lanes[i] != 0)
            return false;
    }
    return true;
}

XoodooState::XoodooState(const Xoodoo& anInstance)
    : XoodooLanes(anInstance.getSizeX()*Xoodoo::sizeY), instance(anInstance), sizeZ(anInstance.getSizeZ())
{
}

XoodooState& XoodooState::operator=(const XoodooState& other)
{
    lanes = other.lanes;
    return *this;
}

void XoodooState::translateXZ(int dx, int dz)
{
    const unsigned int sizeX = instance.getSizeX();
    instance.reduceX(dx);
    for(unsigned int y=0; y<Xoodoo::sizeY; y++) {
        vector<LaneValue> plane(sizeX);
        for(unsigned int x=0; x<sizeX; x++)
            plane[(x+dx) % sizeX] = instance.translateZ(lanes[y + Xoodoo::sizeY*x], dz);
        for(unsigned int x=0; x<sizeX; x++)
            lanes[y + Xoodoo::sizeY*x] = plane[x];
    }
}

string XoodooState::getDisplayString(unsigned int x, unsigned int pad) const
{
    const char character[8] = {'.', '1', '2', '3', '4', '5', '6', '7'};
    string result;
    for(unsigned int z=0; z<instance.getSizeZ(); z++) {
        ColumnValue value = getColumn(x, z);
        result += character[value];
    }
    for(unsigned int z=instance.getSizeZ(); z<pad; z++)
        result += ' ';
    return result;
}

int XoodooState::getBit(unsigned int index) const
{
    unsigned int z = index % sizeZ;
    index /= sizeZ;
    unsigned int x = index % instance.getSizeX();
    index /= instance.getSizeX();
    unsigned int y = index;
    return getBit(x, y, z);
}

void XoodooState::setBit(unsigned int index, int value)
{
    unsigned int z = index % sizeZ;
    index /= sizeZ;
    unsigned int x = index % instance.getSizeX();
    index /= instance.getSizeX();
    unsigned int y = index;
    if (value)
        setBitToOne(x, y, z);
    else
        setBitToZero(x, y, z);
}
