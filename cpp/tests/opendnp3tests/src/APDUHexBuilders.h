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

#ifndef __APDU_HEX_BUILDERS_H_
#define __APDU_HEX_BUILDERS_H_

#include <string>
#include <cstdint>

#include <opendnp3/gen/PointClass.h>
#include <opendnp3/app/IINField.h>
#include <opendnp3/gen/FunctionCode.h>
#include <opendnp3/app/ClassField.h>

namespace hex
{
	// ----------- requests --------------

	std::string IntegrityPoll(uint8_t seq, const opendnp3::ClassField& field = opendnp3::ClassField::AllClasses());

	std::string ClassTask(opendnp3::FunctionCode fc, uint8_t seq, const opendnp3::ClassField& field = opendnp3::ClassField::AllClasses());

	std::string ClassPoll(uint8_t seq, opendnp3::PointClass pc);

	std::string EventPoll(uint8_t seq, const opendnp3::ClassField& field = opendnp3::ClassField::AllEventClasses());

	std::string ClearRestartIIN(uint8_t seq);

	std::string MeasureDelay(uint8_t seq);

	// ----------- responses --------------

	std::string EmptyResponse(uint8_t seq, const opendnp3::IINField& iin = opendnp3::IINField::Empty());

	std::string NullUnsolicited(uint8_t seq, const opendnp3::IINField& iin = opendnp3::IINField(opendnp3::IINBit::DEVICE_RESTART));

	// ----------- confirms --------------

	std::string SolicitedConfirm(uint8_t seq);

	std::string UnsolConfirm(uint8_t seq);

	std::string Confirm(uint8_t seq, bool unsol);
}

#endif

