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

#ifndef SECAUTH_OUTSTATION_ERROR_CODES_H
#define SECAUTH_OUTSTATION_ERROR_CODES_H

#include <system_error>

#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

namespace secauth
{
enum class OutstationError : int
{
    BAD_UNWRAPPPED_UPDATE_KEY_DATA_SIZE,
    DECRYPTED_USERNAME_MISMATCH,
    CHALLENGE_DATA_MISMATCH,
    KEY_CHANGE_CONFIRMATION_HMAC_MISMATCH
};

class OutstationErrorCategory final : public std::error_category
{
public:

	static const std::error_category& Instance()
	{
		return instance;
	}

	virtual const char* name() const NOEXCEPT
	{
		return "Outstation Errors";
	}
	virtual std::string message(int ev) const;

private:

	OutstationErrorCategory() {}
	OutstationErrorCategory(const OutstationErrorCategory&) = delete;

	static OutstationErrorCategory instance;
};

std::error_code make_error_code(secauth::OutstationError err);
}

namespace std
{
template <>
struct is_error_code_enum<secauth::OutstationError> : public true_type {};
}




#endif
