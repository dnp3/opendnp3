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
#include <boost/test/unit_test.hpp>

#include "TestHelpers.h"
#include "BufferHelpers.h"

#include <opendnp3/APDU.h>
#include <opendnp3/ObjectHeader.h>
#include <opendnp3/ObjectReadIterator.h>

#include <opendnp3/DNPConstants.h>
#include <opendnp3/DataTypes.h>

#include <queue>
#include <set>
#include <sstream>

using namespace std;
using namespace openpal;
using namespace opendnp3;

BOOST_AUTO_TEST_SUITE(APDUReading)
BOOST_AUTO_TEST_CASE(WriteTooMuch)
{
	APDU frag(100);
	CopyableBuffer buff(101);	

	BOOST_REQUIRE_THROW(frag.Write(buff.ToReadOnly()), ArgumentException);
}

BOOST_AUTO_TEST_CASE(FunctionCodeToStringNamesAreUnique)
{
	const size_t NUM_CODES = 34;
	FunctionCode codes[NUM_CODES] = {
		FunctionCode::CONFIRM,
		FunctionCode::READ,
		FunctionCode::WRITE,
		FunctionCode::SELECT,
		FunctionCode::OPERATE,
		FunctionCode::DIRECT_OPERATE,
		FunctionCode::DIRECT_OPERATE_NO_ACK,
		FunctionCode::FREEZE,
		FunctionCode::FREEZE_NO_ACK,
		FunctionCode::FREEZE_CLEAR,
		FunctionCode::FREEZE_CLEAR_NO_ACK,
		FunctionCode::FREEZE_AT_TIME,
		FunctionCode::FREEZE_AT_TIME_NO_ACK,
		FunctionCode::COLD_RESTART,
		FunctionCode::WARM_RESTART,
		FunctionCode::INITIALIZE_DATA,
		FunctionCode::INITIALIZE_APPLICATION,
		FunctionCode::START_APPLICATION,
		FunctionCode::STOP_APPLICATION,
		FunctionCode::SAVE_CONFIGURATION,
		FunctionCode::ENABLE_UNSOLICITED,
		FunctionCode::DISABLE_UNSOLICITED,
		FunctionCode::ASSIGN_CLASS,
		FunctionCode::DELAY_MEASURE,
		FunctionCode::RECORD_TIME,
		FunctionCode::FILE_OPEN,
		FunctionCode::FILE_CLOSE,
		FunctionCode::FILE_DELETE,
		FunctionCode::FILE_INFO,
		FunctionCode::FILE_AUTHENTICATE,
		FunctionCode::FILE_ABORT,
		FunctionCode::RESPONSE,
		FunctionCode::UNSOLICITED_RESPONSE,
		FunctionCode::UNKNOWN
	};

	set<string> strings;
	for(int i = 0; i < NUM_CODES; ++i) {
		FunctionCode code = static_cast<FunctionCode>(codes[i]);
		string text = FunctionCodeToString(code);
		if(strings.find(text) != strings.end()) {
			ostringstream oss;
			oss << "The string " << text << " was encountered 2x with int " << i << " and function code " << FunctionCodeToString(code);
			oss << " when there were " << strings.size() << " strings in the set";
			BOOST_FAIL(oss.str());
		}
		strings.insert(text);
	}
}


BOOST_AUTO_TEST_CASE(APDUToString)
{
	APDU frag;
	HexSequence hs("C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06");
	frag.Write(hs);
	frag.Interpret();

	std::string expected = "FIR: 1, FIN: 1, CON: 0, UNS: 0, SEQ: 3, Func: READ HdrCount: 4, Header: (Grp: 60, Var: 2, Qual: ALL_OBJECTS) Header: (Grp: 60, Var: 3, Qual: ALL_OBJECTS) Header: (Grp: 60, Var: 4, Qual: ALL_OBJECTS) Header: (Grp: 60, Var: 1, Qual: ALL_OBJECTS), Size: 14";
	std::string interpreted = frag.ToString();	

	BOOST_REQUIRE_EQUAL(expected, interpreted);
}

BOOST_AUTO_TEST_CASE(ClassPollRequest)
{
	APDU frag;
	HexSequence hs("C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06");
	frag.Write(hs.ToReadOnly());
	frag.Interpret();

	//Test the Application header
	AppControlField control = frag.GetControl();

	BOOST_REQUIRE(frag.GetFunction() == FunctionCode::READ);
	BOOST_REQUIRE(control.FIR);
	BOOST_REQUIRE(control.FIN);
	BOOST_REQUIRE(!control.CON);
	BOOST_REQUIRE(!control.UNS);
	BOOST_REQUIRE_EQUAL(control.SEQ, 3);

	bool except = false;
	try {
		IINField f = frag.GetIIN();
	}
	catch(Exception) {
		except = true;
	}
	BOOST_REQUIRE(except);

	queue< pair<int, int> > mObjVar;
	mObjVar.push( pair<int, int>(60, 2) );
	mObjVar.push( pair<int, int>(60, 3) );
	mObjVar.push( pair<int, int>(60, 4) );
	mObjVar.push( pair<int, int>(60, 1) );

	HeaderReadIterator hdrs = frag.BeginRead();

	BOOST_REQUIRE_EQUAL(hdrs.Count(), 4); //assert that there are 4 headers

	for(; !hdrs.IsEnd(); ++hdrs) {
		ObjectReadIterator objs = hdrs.BeginRead();
		BOOST_REQUIRE_EQUAL(objs.Count(), 0);

		BOOST_REQUIRE(mObjVar.size() > 0);
		BOOST_REQUIRE_EQUAL(hdrs->GetGroup(), mObjVar.front().first);
		BOOST_REQUIRE_EQUAL(hdrs->GetVariation(), mObjVar.front().second);
		mObjVar.pop();

		//Check that the headers indicates it's at the end of iteration
		BOOST_REQUIRE(objs.IsEnd());
	}

	BOOST_REQUIRE_EQUAL(mObjVar.size(), 0);
}

BOOST_AUTO_TEST_CASE(ResponseWithDataAndFlags)
{
	APDU frag;
	HexSequence hs("E3 81 96 00 02 01 28 01 00 00 00 01 02 01 28 01 00 01 00 01 02 01 28 01 00 02 00 01 02 01 28 01 00 03 00 01 20 02 28 01 00 00 00 01 00 00 20 02 28 01 00 01 00 01 00 00 01 01 01 00 00 03 00 00 1E 02 01 00 00 01 00 01 00 00 01 00 00");
	frag.Write(hs);
	frag.Interpret();

	//Test the Application header
	AppControlField control = frag.GetControl();
	BOOST_REQUIRE(frag.GetFunction() == FunctionCode::RESPONSE);
	BOOST_REQUIRE(control.FIR);
	BOOST_REQUIRE(control.FIN);
	BOOST_REQUIRE(control.CON);
	BOOST_REQUIRE(!control.UNS);
	BOOST_REQUIRE_EQUAL(control.SEQ, 3);
	
	size_t hdr_count = 0;

	// 4 binary event headers (1 event), 2 analog event headers (1 event), 1 binary input header (1 Object,4 values), 1 analog input header (2) values = 8 headers
	HeaderReadIterator i = frag.BeginRead();
	BOOST_REQUIRE_EQUAL(i.Count(), 8);

	for(; !i.IsEnd(); i++) {
		ObjectReadIterator j = i.BeginRead();

		switch(hdr_count) {
		case(0):
		case(1):
		case(2):
		case(3): {
				BOOST_REQUIRE_EQUAL(i->GetGroup(), 2);
				BOOST_REQUIRE_EQUAL(i->GetVariation(), 1);

				BOOST_REQUIRE_EQUAL(j.Count(), 1);
				BOOST_REQUIRE_FALSE(j.IsEnd());
				BOOST_REQUIRE_EQUAL(j->Index(), hdr_count);

				const Group2Var1* pObj = static_cast<const Group2Var1*>(i->GetBaseObject());
				BOOST_REQUIRE_EQUAL(pObj->mFlag.Get(*j), 0x01);

				break;
			}
		case(4): { // The analog events have indices 0 and 1
				BOOST_REQUIRE_EQUAL(j.Count(), 1);
				BOOST_REQUIRE_FALSE(j.IsEnd());
				BOOST_REQUIRE_EQUAL(j->Index(), 0);

				break;
			}
		case(5): {
				BOOST_REQUIRE_EQUAL(j.Count(), 1);
				BOOST_REQUIRE_FALSE(j.IsEnd());
				BOOST_REQUIRE_EQUAL(j->Index(), 1);

				break;
			}
		case(6): { //The binary status is collective, so the index should be the starting index which is 0
				BOOST_REQUIRE_EQUAL(j.Count(), 4); //four objects in bitfield obj

				BOOST_REQUIRE_EQUAL(i->GetObjectType(), OT_BITFIELD);
				BOOST_REQUIRE_EQUAL(i->GetGroup(), 1);
				BOOST_REQUIRE_EQUAL(i->GetVariation(), 1);

				const Group1Var1* pObj = static_cast<const Group1Var1*>(i->GetBaseObject());

				for(size_t k = 0; k < 4; k++) {
					BOOST_REQUIRE_FALSE(j.IsEnd());
					BOOST_REQUIRE_EQUAL(j->Index(), k);
					BOOST_REQUIRE_EQUAL(j->Start(), 0);
					BOOST_REQUIRE_EQUAL( pObj->Read(*j, j->Start(), j->Index()), false);
					j++;
				}

				BOOST_REQUIRE(j.IsEnd());//no more values

				break;
			}
		case(7): {
				BOOST_REQUIRE_EQUAL(j.Count(), 2);
				BOOST_REQUIRE_FALSE(j.IsEnd());
				BOOST_REQUIRE_EQUAL(j->Index(), 0);
				j++;
				BOOST_REQUIRE_FALSE(j.IsEnd());
				BOOST_REQUIRE_EQUAL(j->Index(), 1);
				j++;
				BOOST_REQUIRE(j.IsEnd());
				break;
			}
		}

		++hdr_count;
	}

	BOOST_REQUIRE_EQUAL(hdr_count, 8);
}

BOOST_AUTO_TEST_CASE(Confirm)
{
	APDU frag;
	HexSequence hs("C3 00");
	frag.Write(hs);
	frag.Interpret();

	AppControlField control = frag.GetControl();
	BOOST_REQUIRE(frag.GetFunction() == FunctionCode::CONFIRM);
	BOOST_REQUIRE(control.FIR);
	BOOST_REQUIRE(control.FIN);
	BOOST_REQUIRE(!control.CON);
	BOOST_REQUIRE(!control.UNS);
	BOOST_REQUIRE_EQUAL(control.SEQ, 3);
}

BOOST_AUTO_TEST_CASE(ClearIIN)
{
	APDU frag;
	HexSequence hs("C4 02 50 01 00 07 07 00");
	frag.Write(hs);
	frag.Interpret();

	AppControlField control = frag.GetControl();

	BOOST_REQUIRE(frag.GetFunction() == FunctionCode::WRITE);

	HeaderReadIterator hdrs = frag.BeginRead();
	BOOST_REQUIRE_EQUAL(hdrs.Count(), 1);

	ObjectReadIterator objs = hdrs.BeginRead();
	BOOST_REQUIRE_EQUAL(objs.Count(), 1);
	BOOST_REQUIRE_EQUAL(hdrs->GetGroup(), 80);
	BOOST_REQUIRE_EQUAL(hdrs->GetVariation(), 1);

	BOOST_REQUIRE_EQUAL(objs->Index(), 7);
	BOOST_REQUIRE_EQUAL(objs->Start(), 7);

	const BitfieldObject* pObj = static_cast<const BitfieldObject*>(hdrs->GetBaseObject());

	BOOST_REQUIRE(pObj->Read(*objs, objs->Start(), objs->Index()) == false);
}

BOOST_AUTO_TEST_CASE(ReadIndices)
{
	APDU frag;
	HexSequence hs("C0 01 01 02 17 03 01 03 05");
	frag.Write(hs);
	frag.Interpret();

	HeaderReadIterator i = frag.BeginRead();
	BOOST_REQUIRE_EQUAL(i.Count(), 1);

	ObjectReadIterator j = i.BeginRead();
	BOOST_REQUIRE_EQUAL(j.Count(), 3);
	BOOST_REQUIRE(!j.HasData());

	BOOST_REQUIRE_EQUAL(j->Index(), 1);
	++j;
	BOOST_REQUIRE_EQUAL(j->Index(), 3);
	++j;
	BOOST_REQUIRE_EQUAL(j->Index(), 5);
	++j;
	BOOST_REQUIRE( j.IsEnd() );
}

BOOST_AUTO_TEST_CASE(InsufficientDataForFragment)
{
	APDU frag;
	HexSequence hs("C4");
	frag.Write(hs);

	int code = -1;
	try {
		frag.Interpret();
	}
	catch(Exception ex) {
		code = ex.ErrorCode();
	}
	BOOST_REQUIRE_EQUAL(code, ALERR_INSUFFICIENT_DATA_FOR_FRAG);
}

BOOST_AUTO_TEST_CASE(InsufficientDataForResponse)
{
	APDU frag;
	HexSequence hs("C4 81 00");
	frag.Write(hs);

	int code = -1;
	try {
		frag.Interpret();
	}
	catch(Exception ex) {
		code = ex.ErrorCode();
	}
	BOOST_REQUIRE_EQUAL(code, ALERR_INSUFFICIENT_DATA_FOR_RESPONSE);
}

BOOST_AUTO_TEST_CASE(InsufficientDataForObjectHeader)
{
	APDU frag;
	HexSequence hs("C4 81 00 00 00");
	frag.Write(hs);

	int code = -1;
	try {
		frag.Interpret();
	}
	catch(Exception ex) {
		code = ex.ErrorCode();
	}

	BOOST_REQUIRE_EQUAL(code, ALERR_INSUFFICIENT_DATA_FOR_HEADER);
}

BOOST_AUTO_TEST_CASE(UnknownGroupVar)
{
	APDU frag;
	HexSequence hs("C4 81 00 00 FF FF 06");
	frag.Write(hs);

	bool gotIt = false;
	try {
		frag.Interpret();
	}
	catch(ObjectException ex) {
		gotIt = true;
	}

	BOOST_REQUIRE(gotIt);
}

BOOST_AUTO_TEST_CASE(StartStopMismach)
{
	APDU frag;
	HexSequence hs("C4 81 00 00 01 01 00 02 00 01 00");  //obj 1 var 1 with 1 octet start = 0x02, stop = 0x01
	frag.Write(hs);

	int code = -1;
	try {
		frag.Interpret();
	}
	catch(Exception ex) {
		code = ex.ErrorCode();
	}

	BOOST_REQUIRE_EQUAL(code, ALERR_START_STOP_MISMATCH);
}

BOOST_AUTO_TEST_CASE(NonstaticObjectWithIndexPrefix)
{
	APDU frag;
	HexSequence hs("C4 81 00 00 01 01 17 00 00");  //obj 1 var 1 with 1 octet index prefix and 1 octet count
	frag.Write(hs);

	int code = -1;
	try {
		frag.Interpret();
	}
	catch(Exception ex) {
		code = ex.ErrorCode();
	}

	BOOST_REQUIRE_EQUAL(code, ALERR_ILLEGAL_QUALIFIER_AND_OBJECT);
}

BOOST_AUTO_TEST_CASE(UnknownQualifer)
{
	APDU frag;
	HexSequence hs("C4 81 00 00 01 02 10 00 00");  //obj 1 var 2 with index 1octet prefix and 1octet start stop
	frag.Write(hs);

	int code = -1;
	try {
		frag.Interpret();
	}
	catch(Exception ex) {
		code = ex.ErrorCode();
	}

	BOOST_REQUIRE_EQUAL(code, ALERR_UNKNOWN_QUALIFIER);
}

BOOST_AUTO_TEST_CASE(VirtualTerminalObjectWrite)
{
	APDU frag;

	// Application Control (AC):
	//      First Fragment (FIR) = 1
	//      Final Fragment (FIN) = 1
	//      Confirmation Required (CON) = 0
	//      Unsolicited Response (UNS) = 0
	//      Sequence (SEQ) = 0x2
	//
	// Function Control (FC):
	//      Request / WRITE = 0x02
	//
	// Group (GP):
	//      Object Group 112 = 0x70
	//
	// Variation (VR):
	//      Length = 0x05
	//
	// Qualified Field (QF):
	//      RES = 0
	//      Object Prefix Code = 0x1 (prefixed with a 1-octet index)
	//      Range Specifier Code = 0x7 (1-octet count of objects)
	//
	// Range Field (RF):
	//      Count = 0x01
	//
	// Object Prefix Index (IX):
	//      Prefix = 0x00
	//
	// Data Field:
	//      h = 0x68
	//      e = 0x65
	//      l = 0x6C
	//      l = 0x6C
	//      o = 0x6F
	//
	//              AC FC GP VR QF RF IX h  e  l  l  o
	HexSequence hs("C2 02 70 05 17 01 00 68 65 6C 6C 6F");

	frag.Write(hs);
	frag.Interpret();

	HeaderReadIterator i = frag.BeginRead();
	BOOST_REQUIRE_EQUAL(i.Count(), 1);

	BOOST_REQUIRE_EQUAL(i->GetGroup(), 112);
	BOOST_REQUIRE_EQUAL(i->GetVariation(), 5);

	ObjectReadIterator j = i.BeginRead();
	BOOST_REQUIRE_EQUAL(j.Count(), 1);
	BOOST_REQUIRE(j.HasData());

	const SizeByVariationObject* pObj = static_cast<const SizeByVariationObject*>(i->GetBaseObject());

	uint8_t buff[100];
	pObj->Read(*j, i->GetVariation(), buff);

	BOOST_REQUIRE_EQUAL("hello", std::string(reinterpret_cast<const char*>(buff), 5));
}

BOOST_AUTO_TEST_CASE(VtoObjectWriteMultipleIndices)
{
	APDU frag;

	// Application Control (AC):
	//      First Fragment (FIR) = 1
	//      Final Fragment (FIN) = 1
	//      Confirmation Required (CON) = 0
	//      Unsolicited Response (UNS) = 0
	//      Sequence (SEQ) = 0x2
	//
	// Function Control (FC):
	//      Request / WRITE = 0x02
	//
	// Group (GP):
	//      Object Group 112 = 0x70
	//
	// Variation (VR):
	//      Length = 0x05
	//
	// Qualified Field (QF):
	//      RES = 0
	//      Object Prefix Code = 0x1 (prefixed with a 1-octet index)
	//      Range Specifier Code = 0x7 (1-octet count of objects)
	//
	// Range Field (RF):
	//      Count = 0x01
	//
	// Object Prefix Index (IX):
	//      Prefix = 0x00
	//
	// Data Field:
	//      h = 0x68
	//      e = 0x65
	//      l = 0x6C
	//      l = 0x6C
	//      o = 0x6F
	//
	// Group (GP):
	//      Object Group 112 = 0x70
	//
	// Variation (VR):
	//      Length = 0x05
	//
	// Qualified Field (QF):
	//      RES = 0
	//      Object Prefix Code = 0x1 (prefixed with a 1-octet index)
	//      Range Specifier Code = 0x7 (1-octet count of objects)
	//
	// Range Field (RF):
	//      Count = 0x43
	//
	// Object Prefix Index (IX):
	//      Prefix = 0x00
	//
	// Data Field:
	//      w = 0x
	//      o = 0x
	//      r = 0x
	//      l = 0x6C
	//      d = 0x
	//
	//              AC FC GP VR QF RF IX h  e  l  l  o  GP VR QF RF IX w  o  r  l  d
	HexSequence hs("C2 02 70 05 17 01 00 68 65 6C 6C 6F 70 05 17 43 00 77 6F 72 6C 64");

	frag.Write(hs);
	frag.Interpret();

	/* First object */
	HeaderReadIterator i = frag.BeginRead();
	BOOST_REQUIRE_EQUAL(i.Count(), 2);

	BOOST_REQUIRE_EQUAL(i->GetGroup(), 112);
	BOOST_REQUIRE_EQUAL(i->GetVariation(), 5);

	ObjectReadIterator j = i.BeginRead();
	BOOST_REQUIRE_EQUAL(j.Count(), 1);
	BOOST_REQUIRE(j.HasData());

	const SizeByVariationObject* pObj = static_cast<const SizeByVariationObject*>(i->GetBaseObject());

	uint8_t buff[100];
	pObj->Read(*j, i->GetVariation(), buff);

	BOOST_REQUIRE_EQUAL("hello", std::string(reinterpret_cast<const char*>(buff), 5));

	/* Second object */
	++i;
	BOOST_REQUIRE_EQUAL(i.Count(), 2);

	BOOST_REQUIRE_EQUAL(i->GetGroup(), 112);
	BOOST_REQUIRE_EQUAL(i->GetVariation(), 5);

	j = i.BeginRead();
	BOOST_REQUIRE_EQUAL(j.Count(), 67);
	BOOST_REQUIRE(j.HasData());

	pObj = static_cast<const SizeByVariationObject*>(i->GetBaseObject());

	pObj->Read(*j, i->GetVariation(), buff);

	BOOST_REQUIRE_EQUAL("world", std::string(reinterpret_cast<const char*>(buff), 5));
}

BOOST_AUTO_TEST_CASE(OperatorEquals)
{
	APDU frag1;
	APDU frag2;
	APDU frag3;

	HexSequence hs("C3 01 3C 02 06 3C 03 06 3C 04 06 3C 01 06");
	frag1.Write(hs);
	frag2.Write(hs);

	BOOST_REQUIRE(frag1 == frag2);
	BOOST_REQUIRE(frag2 != frag3);
}

BOOST_AUTO_TEST_CASE(SetControlNegativeTC)
{
	APDU frag;
	BOOST_REQUIRE_THROW(frag.SetControl(true, true, true, true, -1), ArgumentException);
	BOOST_REQUIRE_THROW(frag.SetControl(true, true, true, true, 16), ArgumentException);
}
BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
