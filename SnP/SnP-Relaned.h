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

#ifndef _SnP_Relaned_h_
#define _SnP_Relaned_h_

/** Function to XOR data given as bytes into the state.
  * The bits to modify are restricted to be consecutive and to be in the same lane.
  * The bit positions that are affected by this function are
  * from @a lanePosition*(lane size in bits) + @a offset*8
  * to @a lanePosition*(lane size in bits) + @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  lanePosition    Index of the lane to be modified (x+5*y,
  *                         or bit position divided by the lane size).
  * @param  data    Pointer to the input data.
  * @param  offset  Offset in bytes within the lane.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a lanePosition < 25
  * @pre    0 ≤ @a offset < (lane size in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (lane size in bytes)
  */
void SnP_XORBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length);

/** Function to XOR data given as bytes into the state.
  * The bits to modify are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * @param  state   Pointer to the state.
  * @param  data    Pointer to the input data.
  * @param  laneCount   The number of lanes, i.e., the length of the data
  *                     divided by the lane size.
  * @pre    0 ≤ @a laneCount ≤ 25
  */
void SnP_XORLanes(void *state, const unsigned char *data, unsigned int laneCount);

/** Function to overwrite data given as bytes into the state.
  * The bits to modify are restricted to be consecutive and to be in the same lane.
  * The bit positions that are affected by this function are
  * from @a lanePosition*(lane size in bits) + @a offset*8
  * to @a lanePosition*(lane size in bits) + @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  lanePosition    Index of the lane to be modified (x+5*y,
  *                         or bit position divided by the lane size).
  * @param  data    Pointer to the input data.
  * @param  offset  Offset in bytes within the lane.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a lanePosition < 25
  * @pre    0 ≤ @a offset < (lane size in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (lane size in bytes)
  */
void SnP_OverwriteBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length);

/** Function to overwrite data given as bytes into the state.
  * The bits to modify are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * @param  state   Pointer to the state.
  * @param  data    Pointer to the input data.
  * @param  laneCount   The number of lanes, i.e., the length of the data
  *                     divided by the lane size.
  * @pre    0 ≤ @a laneCount ≤ 25
  */
void SnP_OverwriteLanes(void *state, const unsigned char *data, unsigned int laneCount);

/** Function to retrieve data from the state into bytes.
  * The bits to output are restricted to be consecutive and to be in the same lane.
  * The bit positions that are retrieved by this function are
  * from @a lanePosition*(lane size in bits) + @a offset*8
  * to @a lanePosition*(lane size in bits) + @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  lanePosition    Index of the lane to be read (x+5*y,
  *                         or bit position divided by the lane size).
  * @param  data    Pointer to the area where to store output data.
  * @param  offset  Offset in byte within the lane.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a lanePosition < 25
  * @pre    0 ≤ @a offset < (lane size in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (lane size in bytes)
  */
void SnP_ExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length);

/** Function to retrieve data from the state into bytes.
  * The bits to output are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * @param  state   Pointer to the state.
  * @param  data    Pointer to the area where to store output data.
  * @param  laneCount   The number of lanes, i.e., the length of the data
  *                     divided by the lane size.
  * @pre    0 ≤ @a laneCount ≤ 25
  */
void SnP_ExtractLanes(const void *state, unsigned char *data, unsigned int laneCount);

/** Function to retrieve data from the state into bytes and
  * to XOR them into the output buffer.
  * The bits to output are restricted to be consecutive and to be in the same lane.
  * The bit positions that are retrieved by this function are
  * from @a lanePosition*(lane size in bits) + @a offset*8
  * to @a lanePosition*(lane size in bits) + @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  lanePosition    Index of the lane to be read (x+5*y,
  *                         or bit position divided by the lane size).
  * @param  data    Pointer to the area where to XOR output data.
  * @param  offset  Offset in byte within the lane.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a lanePosition < 25
  * @pre    0 ≤ @a offset < (lane size in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (lane size in bytes)
  */
void SnP_ExtractAndXORBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length);

/** Function to retrieve data from the state into bytes and
  * to XOR them into the output buffer.
  * The bits to output are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * @param  state   Pointer to the state.
  * @param  data    Pointer to the area where to XOR output data.
  * @param  laneCount   The number of lanes, i.e., the length of the data
  *                     divided by the lane size.
  * @pre    0 ≤ @a laneCount ≤ 25
  */
void SnP_ExtractAndXORLanes(const void *state, unsigned char *data, unsigned int laneCount);

#define SnP_XORBytes(state, data, offset, length) \
    { \
        if ((offset) == 0) { \
            SnP_XORLanes(state, data, (length)/SnP_laneLengthInBytes); \
            SnP_XORBytesInLane(state, \
                (length)/SnP_laneLengthInBytes, \
                (data)+((length)/SnP_laneLengthInBytes)*SnP_laneLengthInBytes, \
                0, \
                (length)%SnP_laneLengthInBytes); \
        } \
        else { \
            unsigned int _sizeLeft = (length); \
            unsigned int _lanePosition = (offset)/SnP_laneLengthInBytes; \
            unsigned int _offsetInLane = (offset)%SnP_laneLengthInBytes; \
            const unsigned char *_curData = (data); \
            while(_sizeLeft > 0) { \
                unsigned int _bytesInLane = SnP_laneLengthInBytes - _offsetInLane; \
                if (_bytesInLane > _sizeLeft) \
                    _bytesInLane = _sizeLeft; \
                SnP_XORBytesInLane(state, _lanePosition, _curData, _offsetInLane, _bytesInLane); \
                _sizeLeft -= _bytesInLane; \
                _lanePosition++; \
                _offsetInLane = 0; \
                _curData += _bytesInLane; \
            } \
        } \
    }

#define SnP_OverwriteBytes(state, data, offset, length) \
    { \
        if ((offset) == 0) { \
            SnP_OverwriteLanes(state, data, (length)/SnP_laneLengthInBytes); \
            SnP_OverwriteBytesInLane(state, \
                (length)/SnP_laneLengthInBytes, \
                (data)+((length)/SnP_laneLengthInBytes)*SnP_laneLengthInBytes, \
                0, \
                (length)%SnP_laneLengthInBytes); \
        } \
        else { \
            unsigned int _sizeLeft = (length); \
            unsigned int _lanePosition = (offset)/SnP_laneLengthInBytes; \
            unsigned int _offsetInLane = (offset)%SnP_laneLengthInBytes; \
            const unsigned char *_curData = (data); \
            while(_sizeLeft > 0) { \
                unsigned int _bytesInLane = SnP_laneLengthInBytes - _offsetInLane; \
                if (_bytesInLane > _sizeLeft) \
                    _bytesInLane = _sizeLeft; \
                SnP_OverwriteBytesInLane(state, _lanePosition, _curData, _offsetInLane, _bytesInLane); \
                _sizeLeft -= _bytesInLane; \
                _lanePosition++; \
                _offsetInLane = 0; \
                _curData += _bytesInLane; \
            } \
        } \
    }

#define SnP_ExtractBytes(state, data, offset, length) \
    { \
        if ((offset) == 0) { \
            SnP_ExtractLanes(state, data, (length)/SnP_laneLengthInBytes); \
            SnP_ExtractBytesInLane(state, \
                (length)/SnP_laneLengthInBytes, \
                (data)+((length)/SnP_laneLengthInBytes)*SnP_laneLengthInBytes, \
                0, \
                (length)%SnP_laneLengthInBytes); \
        } \
        else { \
            unsigned int _sizeLeft = (length); \
            unsigned int _lanePosition = (offset)/SnP_laneLengthInBytes; \
            unsigned int _offsetInLane = (offset)%SnP_laneLengthInBytes; \
            unsigned char *_curData = (data); \
            while(_sizeLeft > 0) { \
                unsigned int _bytesInLane = SnP_laneLengthInBytes - _offsetInLane; \
                if (_bytesInLane > _sizeLeft) \
                    _bytesInLane = _sizeLeft; \
                SnP_ExtractBytesInLane(state, _lanePosition, _curData, _offsetInLane, _bytesInLane); \
                _sizeLeft -= _bytesInLane; \
                _lanePosition++; \
                _offsetInLane = 0; \
                _curData += _bytesInLane; \
            } \
        } \
    }

#define SnP_ExtractAndXORBytes(state, data, offset, length) \
    { \
        if ((offset) == 0) { \
            SnP_ExtractAndXORLanes(state, data, (length)/SnP_laneLengthInBytes); \
            SnP_ExtractAndXORBytesInLane(state, \
                (length)/SnP_laneLengthInBytes, \
                (data)+((length)/SnP_laneLengthInBytes)*SnP_laneLengthInBytes, \
                0, \
                (length)%SnP_laneLengthInBytes); \
        } \
        else { \
            unsigned int _sizeLeft = (length); \
            unsigned int _lanePosition = (offset)/SnP_laneLengthInBytes; \
            unsigned int _offsetInLane = (offset)%SnP_laneLengthInBytes; \
            unsigned char *_curData = (data); \
            while(_sizeLeft > 0) { \
                unsigned int _bytesInLane = SnP_laneLengthInBytes - _offsetInLane; \
                if (_bytesInLane > _sizeLeft) \
                    _bytesInLane = _sizeLeft; \
                SnP_ExtractAndXORBytesInLane(state, _lanePosition, _curData, _offsetInLane, _bytesInLane); \
                _sizeLeft -= _bytesInLane; \
                _lanePosition++; \
                _offsetInLane = 0; \
                _curData += _bytesInLane; \
            } \
        } \
    }

#endif
