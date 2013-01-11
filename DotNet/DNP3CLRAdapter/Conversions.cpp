
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
#include <msclr\marshal_cppstd.h>
#include "Conversions.h"

using namespace DNP3::Interface;

namespace DNP3
{	
namespace Adapter
{

	System::Exception^ Conversions::convertException(const apl::Exception& ex)
	{		
		return gcnew System::Exception(convertString(ex.GetErrorString()));
	}	

	std::string Conversions::convertString(System::String^ s)
	{
		return msclr::interop::marshal_as<std::string>(s);
	}

	System::String^ Conversions::convertString(const std::string& s)
	{
		return gcnew System::String(s.c_str());
	}

	apl::FilterLevel Conversions::convertFilterLevel(FilterLevel level)
	{
		switch(level)
		{
			case (FilterLevel::LEV_EVENT):
				return apl::LEV_EVENT;
			case (FilterLevel::LEV_ERROR):
				return apl::LEV_ERROR;
			case (FilterLevel::LEV_WARNING):
				return apl::LEV_WARNING;
			case (FilterLevel::LEV_INFO):
				return apl::LEV_INFO;
			case (FilterLevel::LEV_INTERPRET):
				return apl::LEV_INTERPRET;
			case (FilterLevel::LEV_COMM):
				return apl::LEV_COMM;
			case (FilterLevel::LEV_DEBUG):
				return apl::LEV_DEBUG;
			default:
				return apl::LEV_DEBUG;
		}
	}

	FilterLevel Conversions::convertFilterLevel(apl::FilterLevel level)
	{
		switch(level)
		{
			case (apl::LEV_EVENT):
				return FilterLevel::LEV_EVENT;
			case (apl::LEV_ERROR):
				return FilterLevel::LEV_ERROR;
			case (apl::LEV_WARNING):
				return FilterLevel::LEV_WARNING;
			case (apl::LEV_INFO):
				return FilterLevel::LEV_INFO;
			case (apl::LEV_INTERPRET):
				return FilterLevel::LEV_INTERPRET;
			case (apl::LEV_COMM):
				return FilterLevel::LEV_COMM;
			case (apl::LEV_DEBUG):
				return FilterLevel::LEV_DEBUG;
			default:
				return FilterLevel::LEV_DEBUG;
		}
	}

	CommandStatus Conversions::convertCommandStatus(apl::CommandStatus status)
	{
		switch(status)
		{
			case(apl::CS_SUCCESS):
				return CommandStatus::CS_SUCCESS;
			case(apl::CS_TIMEOUT):
				return CommandStatus::CS_TIMEOUT;
			case(apl::CS_NO_SELECT):
				return CommandStatus::CS_NO_SELECT;
			case(apl::CS_FORMAT_ERROR):
				return CommandStatus::CS_FORMAT_ERROR;
			case(apl::CS_NOT_SUPPORTED):
				return CommandStatus::CS_NOT_SUPPORTED;
			case(apl::CS_ALREADY_ACTIVE):
				return CommandStatus::CS_ALREADY_ACTIVE;
			case(apl::CS_HARDWARE_ERROR):
				return CommandStatus::CS_HARDWARE_ERROR;
			case(apl::CS_LOCAL):
				return CommandStatus::CS_LOCAL;
			case(apl::CS_TOO_MANY_OPS):
				return CommandStatus::CS_TOO_MANY_OPS;
			case(apl::CS_NOT_AUTHORIZED):
				return CommandStatus::CS_NOT_AUTHORIZED;

			default:
				return CommandStatus::CS_UNDEFINED;
		}
	}

	apl::CommandStatus Conversions::convertCommandStatus(CommandStatus status)
	{
		switch(status)
		{
			case(CommandStatus::CS_SUCCESS):
				return apl::CS_SUCCESS;
			case(CommandStatus::CS_TIMEOUT):
				return apl::CS_TIMEOUT;
			case(CommandStatus::CS_NO_SELECT):
				return apl::CS_NO_SELECT;
			case(CommandStatus::CS_FORMAT_ERROR):
				return apl::CS_FORMAT_ERROR;
			case(CommandStatus::CS_NOT_SUPPORTED):
				return apl::CS_NOT_SUPPORTED;
			case(CommandStatus::CS_ALREADY_ACTIVE):
				return apl::CS_ALREADY_ACTIVE;
			case(CommandStatus::CS_HARDWARE_ERROR):
				return apl::CS_HARDWARE_ERROR;
			case(CommandStatus::CS_LOCAL):
				return apl::CS_LOCAL;
			case(CommandStatus::CS_TOO_MANY_OPS):
				return apl::CS_TOO_MANY_OPS;
			case(CommandStatus::CS_NOT_AUTHORIZED):
				return apl::CS_NOT_AUTHORIZED;

			default:
				return apl::CS_UNDEFINED;
		}
	}

	ControlCode Conversions::convertControlCode(apl::ControlCode code)
	{
		switch(code)
		{
			case (apl::CC_NULL):
				return ControlCode::CC_NULL;
			case (apl::CC_PULSE):
				return ControlCode::CC_PULSE;
			case (apl::CC_LATCH_ON):
				return ControlCode::CC_LATCH_ON;
			case (apl::CC_LATCH_OFF):
				return ControlCode::CC_LATCH_OFF;
			case (apl::CC_PULSE_CLOSE):
				return ControlCode::CC_PULSE_CLOSE;
			case (apl::CC_PULSE_TRIP):
				return ControlCode::CC_PULSE_TRIP;			
			default:
				return ControlCode::CC_UNDEFINED;
		}
	}

	apl::ControlCode Conversions::convertControlCode(ControlCode code)
	{
		switch(code)
		{
			case (ControlCode::CC_NULL):
				return apl::CC_NULL;
			case (ControlCode::CC_PULSE):
				return apl::CC_PULSE;
			case (ControlCode::CC_LATCH_ON):
				return apl::CC_LATCH_ON;
			case (ControlCode::CC_LATCH_OFF):
				return apl::CC_LATCH_OFF;
			case (ControlCode::CC_PULSE_CLOSE):
				return apl::CC_PULSE_CLOSE;
			case (ControlCode::CC_PULSE_TRIP):
				return apl::CC_PULSE_TRIP;			
			default:
				return apl::CC_UNDEFINED;
		}
	}

	BinaryOutput^ Conversions::convertBO(const apl::BinaryOutput& bo)
	{
		return gcnew BinaryOutput(convertControlCode(bo.GetCode()), bo.mCount, bo.mOnTimeMS, bo.mOffTimeMS);
	}

	apl::BinaryOutput Conversions::convertBO(BinaryOutput^ bo)
	{
		return apl::BinaryOutput(convertControlCode(bo->code), bo->count, bo->onTime, bo->offTime);
	}


	SetpointEncodingType Conversions::convertSetpointEncoding(apl::SetpointEncodingType encoding)
	{
		switch(encoding) {
			case (apl::SPET_INT16):
				return SetpointEncodingType::SPET_INT16;
			case (apl::SPET_INT32):
				return SetpointEncodingType::SPET_INT32;
			case (apl::SPET_FLOAT):
				return SetpointEncodingType::SPET_FLOAT;
			case (apl::SPET_DOUBLE):
				return SetpointEncodingType::SPET_DOUBLE;
			case (apl::SPET_AUTO_INT):
				return SetpointEncodingType::SPET_AUTO_INT;
			case (apl::SPET_AUTO_DOUBLE):
				return SetpointEncodingType::SPET_AUTO_DOUBLE;
			default:
				return SetpointEncodingType::SPET_UNSET;
		}
	}

	apl::SetpointEncodingType Conversions::convertSetpointEncoding(SetpointEncodingType encoding)
	{
		switch(encoding) {
			case (SetpointEncodingType::SPET_INT16):
				return apl::SPET_INT16;
			case (SetpointEncodingType::SPET_INT32):
				return apl::SPET_INT32;
			case (SetpointEncodingType::SPET_FLOAT):
				return apl::SPET_FLOAT;
			case (SetpointEncodingType::SPET_DOUBLE):
				return apl::SPET_DOUBLE;
			case (SetpointEncodingType::SPET_AUTO_INT):
				return apl::SPET_AUTO_INT;
			case (SetpointEncodingType::SPET_AUTO_DOUBLE):
				return apl::SPET_AUTO_DOUBLE;
			default:
				return apl::SPET_UNSET;
		}
	}

	apl::Setpoint Conversions::convertSP(Setpoint^ sp)
	{
		apl::Setpoint ret(sp->value);
		ret.SetEncodingType(convertSetpointEncoding(sp->encodingType));
		return ret;
	}

	Setpoint^ Conversions::convertSP(const apl::Setpoint& sp)
	{
		Setpoint^ ret = gcnew Setpoint(sp.GetValue());
		ret->encodingType = convertSetpointEncoding(sp.GetEncodingType());
		return ret;
	}

	Binary^ Conversions::convertMeas(apl::Binary meas)
	{
		return gcnew Binary(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	Analog^ Conversions::convertMeas(apl::Analog meas)
	{
		return gcnew Analog(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	Counter^ Conversions::convertMeas(apl::Counter meas)
	{
		return gcnew Counter(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	SetpointStatus^ Conversions::convertMeas(apl::SetpointStatus meas)
	{
		return gcnew SetpointStatus(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	ControlStatus^ Conversions::convertMeas(apl::ControlStatus meas)
	{
		return gcnew ControlStatus(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	apl::Binary Conversions::convertMeas(Binary^ meas)
	{
		apl::Binary m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}
	
	apl::Analog Conversions::convertMeas(Analog^ meas)
	{
		apl::Analog m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}
	
	apl::Counter Conversions::convertMeas(Counter^ meas)
	{
		apl::Counter m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}
	
	apl::SetpointStatus Conversions::convertMeas(SetpointStatus^ meas)
	{
		apl::SetpointStatus m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}
	
	apl::ControlStatus Conversions::convertMeas(ControlStatus^ meas)
	{
		apl::ControlStatus m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}

	apl::SerialSettings Conversions::convertSerialSettings(SerialSettings^ settings)
	{
		apl::SerialSettings s;
		s.mDevice = convertString(settings->port);
		s.mBaud = settings->baud;
		s.mDataBits = settings->dataBits;
		s.mStopBits = settings->stopBits;
		s.mParity = (apl::ParityType) settings->parity;
		s.mFlowType = (apl::FlowType) settings->flowControl;
		return s;
	}

	apl::dnp::LinkConfig Conversions::convertConfig(LinkConfig^ config)
	{
		return apl::dnp::LinkConfig(config->isMaster, config->useConfirms, config->numRetry, config->localAddr, config->remoteAddr, config->timeout);
	}

	apl::dnp::AppConfig Conversions::convertConfig(AppConfig^ config)
	{
		return apl::dnp::AppConfig(config->rspTimeout, config->numRetry, config->fragSize);
	}

	apl::dnp::ClassMask Conversions::convertClassMask(ClassMask^ cm)
	{
		return apl::dnp::ClassMask(cm->class1, cm->class2, cm->class3);
	}

	apl::dnp::EventMaxConfig Conversions::convertEventMaxConfig(EventMaxConfig^ cm)
	{
		return apl::dnp::EventMaxConfig(cm->maxBinaryEvents, cm->maxAnalogEvents, cm->maxCounterEvents, 0);
	}

	apl::dnp::GrpVar Conversions::convertGrpVar(GrpVar^ gv)
	{
		return apl::dnp::GrpVar(gv->grp, gv->var);
	}

	apl::dnp::SlaveConfig Conversions::convertConfig(SlaveConfig^ config)
	{
		apl::dnp::SlaveConfig sc;

		sc.mMaxControls = config->maxControls;
		sc.mUnsolMask = convertClassMask(config->unsolMask);		
		sc.mAllowTimeSync = config->allowTimeSync;
		sc.mTimeSyncPeriod = config->timeSyncPeriod;
		sc.mUnsolPackDelay = config->unsolPackDelay;
		sc.mUnsolRetryDelay = config->unsolRetryDelay;
		sc.mMaxFragSize = config->maxFragSize;
		sc.mEventMaxConfig = convertEventMaxConfig(config->eventMaxConfig);
		sc.mStaticBinary = convertGrpVar(config->staticBinary);
		sc.mStaticAnalog = convertGrpVar(config->staticAnalog);
		sc.mStaticCounter = convertGrpVar(config->staticCounter);
		sc.mStaticSetpointStatus = convertGrpVar(config->staticSetpointStatus);
		sc.mEventBinary = convertGrpVar(config->eventBinary);
		sc.mEventAnalog = convertGrpVar(config->eventAnalog);
		sc.mEventCounter = convertGrpVar(config->eventCounter);
		
		return sc;
	}

	apl::dnp::PointRecord Conversions::convertRecord(PointRecord^ epr)
	{
		return apl::dnp::PointRecord("");
	}

	apl::dnp::ControlRecord Conversions::convertRecord(ControlRecord^ epr)
	{
		return apl::dnp::ControlRecord("", static_cast<apl::CommandModes>(epr->mode), epr->selectTimeoutMs); 
	}
				
	apl::dnp::EventPointRecord Conversions::convertRecord(EventPointRecord^ epr)
	{
		return apl::dnp::EventPointRecord("", static_cast<apl::dnp::PointClass>(epr->pointClass));
	}

	apl::dnp::DeadbandPointRecord Conversions::convertRecord(DeadbandEventPointRecord^ epr)
	{
		return apl::dnp::DeadbandPointRecord("", static_cast<apl::dnp::PointClass>(epr->pointClass), epr->deadband);
	}

	apl::dnp::DeviceTemplate Conversions::convertConfig(DeviceTemplate^ config)
	{
		apl::dnp::DeviceTemplate dev(	config->binaries->Count, 
										config->analogs->Count,
										config->counters->Count,
										config->controlStatii->Count,
										config->setpointStatii->Count,
										config->controls->Count,
										config->setpoints->Count);

		for(int i=0; i<config->binaries->Count; ++i) dev.mBinary[i] = convertRecord(config->binaries[i]);
		for(int i=0; i<config->analogs->Count; ++i) dev.mAnalog[i] = convertRecord(config->analogs[i]);
		for(int i=0; i<config->counters->Count; ++i) dev.mCounter[i] = convertRecord(config->counters[i]);
		for(int i=0; i<config->controlStatii->Count; ++i) dev.mControlStatus[i] = convertRecord(config->controlStatii[i]);
		for(int i=0; i<config->setpointStatii->Count; ++i) dev.mSetpointStatus[i] = convertRecord(config->setpointStatii[i]);

		for(int i=0; i<config->controls->Count; ++i) dev.mControls[i] = convertRecord(config->controls[i]);
		for(int i=0; i<config->setpoints->Count; ++i) dev.mSetpoints[i] = convertRecord(config->setpoints[i]);

		return dev;
	}

	apl::dnp::MasterConfig Conversions::convertConfig(MasterConfig^ config)
	{
		apl::dnp::MasterConfig mc;
		mc.FragSize = config->fragSize;
		mc.AllowTimeSync = config->allowTimeSync;
		mc.DoUnsolOnStartup = config->doUnsolOnStartup;
		mc.EnableUnsol = config->enableUnsol;
		mc.UnsolClassMask = config->unsolClassMask;
		mc.IntegrityRate = config->integrityRate;
		mc.TaskRetryRate = config->taskRetryRate;

		for each(ExceptionScan^ es in config->scans)
		{
			mc.AddExceptionScan(es->classMask, es->scanRateMs);
		}

		return mc;
	}

	apl::dnp::MasterStackConfig Conversions::convertConfig(MasterStackConfig^ config)
	{
		apl::dnp::MasterStackConfig cfg;
		cfg.master = convertConfig(config->master);
		cfg.app = convertConfig(config->app);
		cfg.link = convertConfig(config->link);
		return cfg;
	}

	apl::dnp::SlaveStackConfig Conversions::convertConfig(SlaveStackConfig^ config)
	{
		apl::dnp::SlaveStackConfig cfg;
		cfg.slave = convertConfig(config->slave);
		cfg.device = convertConfig(config->device);
		cfg.app = convertConfig(config->app);
		cfg.link = convertConfig(config->link);
		return cfg;
	}
}}
