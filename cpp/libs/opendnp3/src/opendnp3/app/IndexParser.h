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
#ifndef OPENDNP3_INDEXPARSER_H
#define OPENDNP3_INDEXPARSER_H

#include <openpal/container/ReadOnlyBuffer.h>
#include <openpal/serialization/Serialization.h>

namespace opendnp3
{

// an index parser is an abstraction for reading index prefixes
class IndexParser
{
public:

	/**
		* Reads the index and possibly widens the type to 32bit depending on the underlying implementation
		* Advances the buffer past the index
		*
		* @return The value of the index
		*/
	virtual uint32_t ReadIndex(openpal::ReadOnlyBuffer& buffer) = 0;

	/**
	* @return The size of the index prefix in bytes (e.g. 1,2,4)
	*/
	virtual uint32_t IndexSize() = 0;

};

template <class ParserType>
class TypedIndexParser : public IndexParser
{
public:

	static IndexParser*	Inst()
	{
		return &mInstance;
	}

	virtual uint32_t ReadIndex(openpal::ReadOnlyBuffer& buffer) override
	{
		return ParserType::ReadBuffer(buffer);
	}

	virtual uint32_t IndexSize() override
	{
		return ParserType::Size;
	}

private:

	static TypedIndexParser mInstance;

	TypedIndexParser() {}
	TypedIndexParser(const TypedIndexParser&);
};

template <class ParserType>
TypedIndexParser<ParserType> TypedIndexParser<ParserType>::mInstance;

}

#endif
