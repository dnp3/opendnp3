/*
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
#ifndef OPENPAL_UINT48TYPE_H
#define OPENPAL_UINT48TYPE_H

#include <cstdint>

namespace openpal
{

class UInt48Type
{

public:

	explicit UInt48Type(uint64_t value) : representation(value)
	{}

	UInt48Type() : representation(0)
	{}

	inline operator uint64_t() const
	{
		return representation;
	}

	uint64_t Get() const
	{
		return representation;
	}

protected:

	uint64_t representation;

};

}

#endif
