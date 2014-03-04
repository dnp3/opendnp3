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
#ifndef __BUFFER_WRAPPER_H_
#define __BUFFER_WRAPPER_H_

#include "HasSize.h"

#include <cstddef>
#include <cstdint>

namespace openpal
{
	
class ReadOnlyBuffer : public HasSize<uint32_t>
{

public:

	ReadOnlyBuffer();
	ReadOnlyBuffer(uint8_t const* apBuffer, uint32_t aSize);

	void CopyTo(uint8_t* apDest) const;

	ReadOnlyBuffer Truncate(uint32_t aSize) const;

	void ZeroSize();

	bool Equals(const ReadOnlyBuffer& rhs) const;

	void Advance(uint32_t aNum);

	operator uint8_t const *() const { return mpBuffer; };

private:
	uint8_t const* mpBuffer;
	
};

class WriteBuffer : public HasSize<uint32_t>
{
	public:

	static WriteBuffer Empty();

    WriteBuffer();
	WriteBuffer(const WriteBuffer& copy);
	WriteBuffer(uint8_t* apBuffer, uint32_t aSize);

	void Clear();

	void Advance(uint32_t aNum);

	WriteBuffer Truncate(uint32_t aNum) const;

	ReadOnlyBuffer ToReadOnly() const;

	operator uint8_t *() { return mpBuffer; };

	operator uint8_t const *() const { return mpBuffer; };

	private:

	uint8_t* mpBuffer;
};


}

#endif
