#ifndef __LINK_LAYER_CONSTANTS_
#define __LINK_LAYER_CONSTANTS_

namespace opendnp3
{

/// Sizes of various part of link frames
enum LinkSizes {
	LS_MIN_LENGTH = 5,
	LS_MAX_LENGTH = 255,
	LS_HEADER_SIZE = 10,
	LS_DATA_BLOCK_SIZE = 16,
	LS_CRC_SIZE = 2,
	LS_DATA_PLUS_CRC_SIZE = 18,
	LS_MAX_USER_DATA_SIZE = 250,
	LS_MAX_FRAME_SIZE = 292,	//10(header) + 250 (user data) + 32 (block CRC's) = 292 frame bytes

};

/// Indices for use with buffers containing link headers
enum LinkHeaderIndex {
	LI_START_05 = 0,
	LI_START_64 = 1,
	LI_LENGTH = 2,
	LI_CONTROL = 3,
	LI_DESTINATION = 4,
	LI_SOURCE = 6,
	LI_CRC = 8
};

/// Masks for use with the CONTROL byte
enum ControlMask {
	MASK_DIR = 0x80,
	MASK_PRM = 0x40,
	MASK_FCB = 0x20,
	MASK_FCV = 0x10,
	MASK_FUNC = 0x0F,
	MASK_FUNC_OR_PRM = MASK_PRM | MASK_FUNC
};

}

#endif

