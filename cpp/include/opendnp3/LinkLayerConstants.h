
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __LINK_LAYER_CONSTANTS_
#define __LINK_LAYER_CONSTANTS_

#include <cstdint>

#include <string>

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

/** These func codes are a little different then those defined in the specification for the following reason.
	For simplicity the PRM bit is included to make the Pri-To-Sec and Sec-To-Pri codes non-overlapping
	This is much simplier and for all intents and purposes you don't have to deal with the PRM bit anymore.

	No implementation of function code 1:

	2004-03-29_DNP3_Doc_Library.pdf: Page 526.
	DNP3 does not define any action associated with the Data Link Layer Reset User Process request function
	code. Support of this function is not required. Function code 1 is now obsolete.
	*/
enum FuncCodes {
	// Pri-To-Sec functions, add 0x40 to each to for the PRM bit
	FC_PRI_RESET_LINK_STATES = 0x40,
	FC_PRI_TEST_LINK_STATES = 0x42,
	FC_PRI_CONFIRMED_USER_DATA = 0x43,
	FC_PRI_UNCONFIRMED_USER_DATA = 0x44,
	FC_PRI_REQUEST_LINK_STATUS = 0x49,
	// Sec-to-Pri functions, just as in the specification
	FC_SEC_ACK = 0x00,
	FC_SEC_NACK = 0x01,
	FC_SEC_LINK_STATUS = 0x0B,
	FC_SEC_NOT_SUPPORTED = 0x0F,

	// Used as an error code for return values
	FC_INVALID = 0xFF
};

/** @param aByte Any byte
	@return Returns FC_INVALID if byte is not a valid function code */
FuncCodes ByteToFuncCode(uint8_t aByte);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
/** @param aCode Any function code
	@return Returns string representation */
std::string FuncCodeToString(FuncCodes aCode);
#endif

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

