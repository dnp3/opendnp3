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
#ifndef OPENDNP3_NUMPARSER_H
#define OPENDNP3_NUMPARSER_H

#include <openpal/logging/Logger.h>
#include <openpal/container/ReadBufferView.h>


#include "opendnp3/app/ParseResult.h"

namespace opendnp3
{

// A one or two byte unsigned integer parser
class NumParser
{
	// a function that consumes bytes from a buffer and returns a uint16_t count
	typedef uint16_t(*ReadFun)(openpal::ReadBufferView& buffer);

public:
	
	uint8_t NumBytes() const;

	ParseResult ParseCount(openpal::ReadBufferView& buffer, uint16_t& count, openpal::Logger* pLogger) const;
	
	static NumParser OneByte();
	static NumParser TwoByte();

private:

	// read the number, consuming from the buffer
	// return true if there is enough bytes, false otherwise
	bool Read(uint16_t& count, openpal::ReadBufferView& buffer) const;

	static uint16_t ReadByte(openpal::ReadBufferView& buffer);

	NumParser(ReadFun pReadFun, uint8_t size);

	ReadFun pReadFun;
	uint8_t size;

	NumParser() = delete;
};

}

#endif
