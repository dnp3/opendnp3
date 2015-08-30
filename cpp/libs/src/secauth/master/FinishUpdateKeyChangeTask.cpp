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

#include "FinishUpdateKeyChangeTask.h"

#include <opendnp3/LogLevels.h>
#include <opendnp3/app/parsing/APDUParser.h>
#include <opendnp3/app/parsing/ObjectHeaderParser.h>

#include <openpal/logging/LogMacros.h>

#include <openpal/crypto/SecureCompare.h>

#include "secauth/KeyChangeConfirmationHMAC.h"
#include "secauth/SingleObjectHandlers.h"

using namespace openpal;
using namespace opendnp3;

namespace secauth
{


FinishUpdateKeyChangeTask::FinishUpdateKeyChangeTask(
    const FinishUpdateKeyChangeArgs& args,
    IMasterApplication& application,
    openpal::IHMACAlgo& algorithm,
    openpal::Logger logger,
    const opendnp3::TaskConfig& config,
    const ChangeUpdateKeyCallbackT& callback
) :
	IMasterTask(application, MonotonicTimestamp::Min(), logger, config),
	m_args(args),
	m_algorithm(&algorithm),
	m_callback(callback)
{}


bool FinishUpdateKeyChangeTask::BuildRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	KeyChangeConfirmationHMAC calc(*m_algorithm);

	std::error_code ec;

	auto hmac = calc.Compute(
	                m_args.updateKey.GetView().data,
	                KeyChangeHMACData(
	                    m_args.outstationName,
	                    m_args.masterChallengeData.ToRSlice(),
	                    m_args.outstationChallengeData.ToRSlice(),
	                    m_args.keyChangeSequenceNum,
	                    m_args.user
	                ),
	                ec
	            );

	if (ec)
	{
		FORMAT_LOG_BLOCK(logger, flags::ERR, "Error calculating key change hmac: %s", ec.message().c_str());
		return false;
	}

	request.SetFunction(FunctionCode::AUTH_REQUEST);
	request.SetControl(AppControlField::Request(seq));

	auto writer = request.GetWriter();

	Group120Var13 updateKeyChange(
	    m_args.keyChangeSequenceNum,
	    m_args.user.GetId(),
	    m_args.encryptedKeyData.ToRSlice()
	);

	if (!(writer.WriteFreeFormat(updateKeyChange) && writer.WriteFreeFormat(Group120Var15(hmac))))
	{
		return false;
	}

	return true;
}

IMasterTask::ResponseResult FinishUpdateKeyChangeTask::ProcessResponse(const opendnp3::APDUResponseHeader& header, const openpal::RSlice& objects)
{
	if (!(header.function == FunctionCode::AUTH_RESPONSE && ValidateSingleResponse(header) && ValidateInternalIndications(header)))
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	// Do a look ahead in the ASDU and determine how to interpret it
	GroupVariation gv = GroupVariation::UNKNOWN;
	if (!ObjectHeaderParser::ReadFirstGroupVariation(objects, gv))
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Response contains empty or malformed object data");
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	switch (gv)
	{
	case(GroupVariation::Group120Var7) :
		return ProcessErrorResponse(objects);
	case(GroupVariation::Group120Var15) :
		return ProcessConfirmationResponse(objects);
	default:
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Unsupported object header in response: %s", GroupVariationToString(gv));
		return ResponseResult::ERROR_BAD_RESPONSE;
	}
}

IMasterTask::ResponseResult FinishUpdateKeyChangeTask::ProcessErrorResponse(const openpal::RSlice& objects)
{
	ErrorHandler handler;
	if (APDUParser::Parse(objects, handler, &logger) == ParseResult::OK)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Outstation returned auth error code: %s", AuthErrorCodeToString(handler.value.errorCode));
	}

	return ResponseResult::ERROR_BAD_RESPONSE;
}

IMasterTask::ResponseResult FinishUpdateKeyChangeTask::ProcessConfirmationResponse(const openpal::RSlice& objects)
{
	KeyChangeConfirmationHandler handler;
	if (APDUParser::Parse(objects, handler, &logger) != ParseResult::OK)
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

	std::error_code ec;

	KeyChangeConfirmationHMAC::ComputeAndCompare(
	    m_args.updateKey.GetView().data,
	    KeyChangeHMACData(
	        m_args.username,
	        m_args.outstationChallengeData.ToRSlice(),
	        m_args.masterChallengeData.ToRSlice(),
	        m_args.keyChangeSequenceNum,
	        m_args.user
	    ),
	    *m_algorithm,
	    handler.value.hmacValue,
	    ec
	);

	if (ec)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "Error verifying outstation HMAC: %s", ec.message().c_str());
		return ResponseResult::ERROR_INTERNAL_FAILURE;
	}

	// make the specified callback
	m_callback(m_args.username, m_args.user, m_args.updateKey);

	return ResponseResult::OK_FINAL;
}


} //end ns



