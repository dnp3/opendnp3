
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
#include "TransportTestObject.h"

#include "BufferHelpers.h"

#include <opendnp3/ToHex.h>
#include <memory>
#include <sstream>


using namespace std;

namespace opendnp3
{

TransportTestObject::TransportTestObject(bool aOpenOnStart, FilterLevel aLevel, bool aImmediate) :
	LogTester(aImmediate),
	mpLogger(mLog.GetLogger(aLevel, "TransportTestObject")),
	transport(mpLogger),
	lower(mpLogger),
	upper(mpLogger)
{
	lower.SetUpperLayer(&transport);
	transport.SetUpperLayer(&upper);

	if(aOpenOnStart) lower.ThisLayerUp();
}

std::string TransportTestObject::GetData(const std::string& arHdr, uint8_t aSeed, size_t aLength)
{
	ByteStr buff(aLength);
	uint8_t val = aSeed;
	for(size_t i = 0; i < aLength; ++i) {
		buff[i] = val;
		++val;
	}

	ostringstream oss;
	if(arHdr.size() > 0) oss << arHdr << " ";
	oss << toHex(buff, buff.Size(), true);
	return oss.str();
}

std::string TransportTestObject::GeneratePacketSequence(vector< std::string >& arVec, size_t aNumPackets, size_t aLastPacketLength)
{
	ostringstream oss;
	for(size_t i = 0; i < aNumPackets; ++i) {
		bool fir = i == 0;
		bool fin = i == (aNumPackets - 1);
		int seq = static_cast<int>(i % 64);
		size_t len = fin ? aLastPacketLength : TL_MAX_TPDU_PAYLOAD;
		uint8_t hdr = TransportTx::GetHeader(fir, fin, seq);
		std::string data = this->GetData("", 0, len); //raw data with no header
		oss << ((i == 0) ? "" : " ") << data; //cache the data in the string stream
		arVec.push_back(toHex(&hdr, 1, true) + " " + data);
	}

	return oss.str();
}

}

