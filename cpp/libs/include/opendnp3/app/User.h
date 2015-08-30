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

#ifndef OPENDNP3_USER_H
#define OPENDNP3_USER_H

#include <cstdint>

namespace opendnp3
{
/// A user in SA is just a uint16. This is just a type-safe wrapper with helpers
class User
{
public:

	User() : id(UNKNOWN_ID)
	{}

	explicit User(uint16_t id_) : id(id_)
	{}

	uint16_t GetId() const
	{
		return id;
	}

	bool IsDefault() const
	{
		return id == DEFAULT_ID;
	}

	bool IsUnknown() const
	{
		return id == UNKNOWN_ID;
	}

	static User Default()
	{
		return User(DEFAULT_ID);
	}

	static User Unknown()
	{
		return User(UNKNOWN_ID);
	}

	static const uint16_t UNKNOWN_ID;
	static const uint16_t DEFAULT_ID;

private:

	uint16_t id;
};

}

#endif
