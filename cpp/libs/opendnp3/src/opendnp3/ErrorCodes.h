/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef OPENDNP3_ERRORCODES_H
#define OPENDNP3_ERRORCODES_H

namespace opendnp3
{

/// Error codes returned in log messages
enum ErrorCodes
{  
	
    // Application Layer Parsing       
    ALERR_INSUFFICIENT_DATA_FOR_HEADER,			// not enough to read object header
    ALERR_INSUFFICIENT_DATA_FOR_OBJECTS,		// not enough to read the indicated objects
	ALERR_UNKNOWN_GROUP_VAR,					// encounter an unknown group var while parsing
	ALERR_ILLEGAL_QUALIFIER_AND_OBJECT,			// The qualifier/object combination is invalid
	ALERR_COUNT_OF_ZERO,						// count header with ZERO	
	ALERR_START_STOP_MISMATCH,					// start > stop in a ranged header
	ALERR_UNKNOWN_QUALIFIER,					// An unknown qualifer was received

    // Transport Layer
	TLERR_NO_HEADER,							// TPDU with no header (empty)	
	TLERR_BUFFER_FULL,							// Transport reassembly buffer full before complete fragment received
	TLERR_NEW_FIR_MID_SEQUENCE,					// Received a new FIR in the middle of previous fragment
	TLERR_MESSAGE_WITHOUT_FIR,					// Received a NON-FIR TPDU without a previous FIR
	TLERR_BAD_SEQUENCE,							// Received a bad transport sequence number
	
	// Link Layer state machine

	DLERR_WRONG_MASTER_BIT,
	DLERR_UNKNOWN_ROUTE,
	DLERR_UNKNOWN_DESTINATION,
	DLERR_UNKNOWN_SOURCE,
	DLERR_UNEXPECTED_LPDU,
	
    // Link Layer parsing / validation

    DLERR_CRC,				//	CRC failure in header or data payload
	DLERR_INVALID_LENGTH,	//	length parameter outside range [0,255]
	DLERR_UNEXPECTED_DATA,	//  length > 5 for non-user data func code
	DLERR_NO_DATA,			//  length == 5 but func code indicates data
	DLERR_UNKNOWN_FUNC,		//  unknown function code
	DLERR_UNEXPECTED_FCV,	//	FCV bit set unexpectedly, ie set on wrong func code
	DLERR_UNEXPECTED_FCB,	//  FCB set unexpectedly
	
};

}


#endif
