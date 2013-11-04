
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
// you under the terms of the License.
//

#include <msclr\marshal_cppstd.h>
#include "Conversions.h"

using namespace DNP3::Interface;

#ifdef ERROR
#undef ERROR
#endif

namespace DNP3
{	
namespace Adapter
{

	System::Exception^ Conversions::convertException(const opendnp3::Exception& ex)
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

	openpal::FilterLevel Conversions::convertFilterLevel(LogLevel level)
	{
		switch(level)
		{
			case (LogLevel::EVENT):
				return openpal::LEV_EVENT;
			case (LogLevel::ERROR):
				return openpal::LEV_ERROR;
			case (LogLevel::WARNING):
				return openpal::LEV_WARNING;
			case (LogLevel::INFO):
				return openpal::LEV_INFO;
			case (LogLevel::INTERPRET):
				return openpal::LEV_INTERPRET;
			case (LogLevel::COMM):
				return openpal::LEV_COMM;
			case (LogLevel::DEBUG):
				return openpal::LEV_DEBUG;
			default:
				return openpal::LEV_DEBUG;
		}
	}

	LogLevel Conversions::convertFilterLevel(openpal::FilterLevel level)
	{
		switch(level)
		{
			case (openpal::LEV_EVENT):
				return LogLevel::EVENT;
			case (openpal::LEV_ERROR):
				return LogLevel::ERROR;
			case (openpal::LEV_WARNING):
				return LogLevel::WARNING;
			case (openpal::LEV_INFO):
				return LogLevel::INFO;
			case (openpal::LEV_INTERPRET):
				return LogLevel::INTERPRET;
			case (openpal::LEV_COMM):
				return LogLevel::COMM;
			case (openpal::LEV_DEBUG):
				return LogLevel::DEBUG;
			default:
				return LogLevel::DEBUG;
		}
	}

	ChannelState Conversions::convertChannelState(opendnp3::ChannelState aState)
	{
		switch(aState)
		{
			case(opendnp3::ChannelState::CS_CLOSED):
				return ChannelState::CLOSED;
			case(opendnp3::ChannelState::CS_OPEN):
				return ChannelState::OPEN;
			case(opendnp3::ChannelState::CS_OPENING):
				return ChannelState::OPENING;
			case(opendnp3::ChannelState::CS_SHUTDOWN):
				return ChannelState::SHUTDOWN;
			case(opendnp3::ChannelState::CS_WAITING):
				return ChannelState::WAITING;
			default:
				return ChannelState::CLOSED;
		}
	}

	
	StackState  Conversions::convertStackState(opendnp3::StackState aState)
	{
		switch(aState)
		{
			case(opendnp3::StackState::SS_COMMS_UP): 
				return StackState::COMMS_UP;
			case(opendnp3::StackState::SS_COMMS_DOWN):
				return StackState::COMMS_DOWN;
			default:
				return StackState::UNKNOWN;
		}
	}

	CommandStatus Conversions::convertCommandStatus(opendnp3::CommandStatus status)
	{
		switch(status)
		{
			case(opendnp3::CS_SUCCESS):
				return CommandStatus::SUCCESS;
			case(opendnp3::CS_TIMEOUT):
				return CommandStatus::TIMEOUT;
			case(opendnp3::CS_NO_SELECT):
				return CommandStatus::NO_SELECT;
			case(opendnp3::CS_FORMAT_ERROR):
				return CommandStatus::FORMAT_ERROR;
			case(opendnp3::CS_NOT_SUPPORTED):
				return CommandStatus::NOT_SUPPORTED;
			case(opendnp3::CS_ALREADY_ACTIVE):
				return CommandStatus::ALREADY_ACTIVE;
			case(opendnp3::CS_HARDWARE_ERROR):
				return CommandStatus::HARDWARE_ERROR;
			case(opendnp3::CS_LOCAL):
				return CommandStatus::LOCAL;
			case(opendnp3::CS_TOO_MANY_OPS):
				return CommandStatus::TOO_MANY_OPS;
			case(opendnp3::CS_NOT_AUTHORIZED):
				return CommandStatus::NOT_AUTHORIZED;

			default:
				return CommandStatus::UNDEFINED;
		}
	}

	opendnp3::CommandStatus Conversions::convertCommandStatus(CommandStatus status)
	{
		switch(status)
		{
			case(CommandStatus::SUCCESS):
				return opendnp3::CS_SUCCESS;
			case(CommandStatus::TIMEOUT):
				return opendnp3::CS_TIMEOUT;
			case(CommandStatus::NO_SELECT):
				return opendnp3::CS_NO_SELECT;
			case(CommandStatus::FORMAT_ERROR):
				return opendnp3::CS_FORMAT_ERROR;
			case(CommandStatus::NOT_SUPPORTED):
				return opendnp3::CS_NOT_SUPPORTED;
			case(CommandStatus::ALREADY_ACTIVE):
				return opendnp3::CS_ALREADY_ACTIVE;
			case(CommandStatus::HARDWARE_ERROR):
				return opendnp3::CS_HARDWARE_ERROR;
			case(CommandStatus::LOCAL):
				return opendnp3::CS_LOCAL;
			case(CommandStatus::TOO_MANY_OPS):
				return opendnp3::CS_TOO_MANY_OPS;
			case(CommandStatus::NOT_AUTHORIZED):
				return opendnp3::CS_NOT_AUTHORIZED;

			default:
				return opendnp3::CS_UNDEFINED;
		}
	}

	ControlCode Conversions::convertControlCode(opendnp3::ControlCode code)
	{
		switch(code)
		{
			case (opendnp3::CC_NULL):
				return ControlCode::CC_NULL;
			case (opendnp3::CC_PULSE):
				return ControlCode::CC_PULSE;
			case (opendnp3::CC_LATCH_ON):
				return ControlCode::CC_LATCH_ON;
			case (opendnp3::CC_LATCH_OFF):
				return ControlCode::CC_LATCH_OFF;
			case (opendnp3::CC_PULSE_CLOSE):
				return ControlCode::CC_PULSE_CLOSE;
			case (opendnp3::CC_PULSE_TRIP):
				return ControlCode::CC_PULSE_TRIP;			
			default:
				return ControlCode::CC_UNDEFINED;
		}
	}

	opendnp3::ControlCode Conversions::convertControlCode(ControlCode code)
	{
		switch(code)
		{
			case (ControlCode::CC_NULL):
				return opendnp3::CC_NULL;
			case (ControlCode::CC_PULSE):
				return opendnp3::CC_PULSE;
			case (ControlCode::CC_LATCH_ON):
				return opendnp3::CC_LATCH_ON;
			case (ControlCode::CC_LATCH_OFF):
				return opendnp3::CC_LATCH_OFF;
			case (ControlCode::CC_PULSE_CLOSE):
				return opendnp3::CC_PULSE_CLOSE;
			case (ControlCode::CC_PULSE_TRIP):
				return opendnp3::CC_PULSE_TRIP;			
			default:
				return opendnp3::CC_UNDEFINED;
		}
	}

	ControlRelayOutputBlock^ Conversions::convertCommand(const opendnp3::ControlRelayOutputBlock& bo)
	{
		return gcnew ControlRelayOutputBlock(convertControlCode(bo.GetCode()), bo.mCount, bo.mOnTimeMS, bo.mOffTimeMS);
	}

	opendnp3::ControlRelayOutputBlock Conversions::convertCommand(ControlRelayOutputBlock^ bo)
	{			
		return opendnp3::ControlRelayOutputBlock(convertControlCode(bo->code), bo->count, bo->onTime, bo->offTime);
	}
				
	opendnp3::AnalogOutputInt32 Conversions::convertCommand(AnalogOutputInt32^ sp)
	{
		return opendnp3::AnalogOutputInt32(sp->value);
	}

	AnalogOutputInt32^ Conversions::convertCommand(const opendnp3::AnalogOutputInt32& sp)
	{
		return gcnew AnalogOutputInt32(sp.GetValue());
	}

	opendnp3::AnalogOutputInt16 Conversions::convertCommand(AnalogOutputInt16^ sp)
	{
		return opendnp3::AnalogOutputInt16(sp->value);
	}
	
	AnalogOutputInt16^ Conversions::convertCommand(const opendnp3::AnalogOutputInt16& sp)
	{
		return gcnew AnalogOutputInt16(sp.GetValue());
	}

	opendnp3::AnalogOutputFloat32 Conversions::convertCommand(AnalogOutputFloat32^ sp)
	{
		return opendnp3::AnalogOutputFloat32(sp->value);
	}

	AnalogOutputFloat32^ Conversions::convertCommand(const opendnp3::AnalogOutputFloat32& sp)
	{
		return gcnew AnalogOutputFloat32(sp.GetValue());
	}

	opendnp3::AnalogOutputDouble64 Conversions::convertCommand(AnalogOutputDouble64^ sp)
	{
		return opendnp3::AnalogOutputDouble64(sp->value);
	}

	AnalogOutputDouble64^ Conversions::convertCommand(const opendnp3::AnalogOutputDouble64& sp)
	{
		return gcnew AnalogOutputDouble64(sp.GetValue());
	}

	Binary^ Conversions::convertMeas(opendnp3::Binary meas)
	{
		return gcnew Binary(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	Analog^ Conversions::convertMeas(opendnp3::Analog meas)
	{
		return gcnew Analog(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	Counter^ Conversions::convertMeas(opendnp3::Counter meas)
	{
		return gcnew Counter(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	SetpointStatus^ Conversions::convertMeas(opendnp3::SetpointStatus meas)
	{
		return gcnew SetpointStatus(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	ControlStatus^ Conversions::convertMeas(opendnp3::ControlStatus meas)
	{
		return gcnew ControlStatus(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}

	opendnp3::Binary Conversions::convertMeas(Binary^ meas)
	{
		opendnp3::Binary m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}
	
	opendnp3::Analog Conversions::convertMeas(Analog^ meas)
	{
		opendnp3::Analog m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}
	
	opendnp3::Counter Conversions::convertMeas(Counter^ meas)
	{
		opendnp3::Counter m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}
	
	opendnp3::SetpointStatus Conversions::convertMeas(SetpointStatus^ meas)
	{
		opendnp3::SetpointStatus m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}
	
	opendnp3::ControlStatus Conversions::convertMeas(ControlStatus^ meas)
	{
		opendnp3::ControlStatus m(meas->value, meas->quality);
		m.SetTime(TimeStamp::Convert(meas->time));
		return m;
	}

	opendnp3::SerialSettings Conversions::convertSerialSettings(SerialSettings^ settings)
	{
		opendnp3::SerialSettings s;
		s.mDevice = convertString(settings->port);
		s.mBaud = settings->baud;
		s.mDataBits = settings->dataBits;
		s.mStopBits = settings->stopBits;
		s.mParity = (opendnp3::ParityType) settings->parity;
		s.mFlowType = (opendnp3::FlowType) settings->flowControl;
		return s;
	}

	opendnp3::LinkConfig Conversions::convertConfig(LinkConfig^ config)
	{
		return opendnp3::LinkConfig(config->isMaster, config->useConfirms, config->numRetry, config->localAddr, config->remoteAddr, config->timeout);
	}

	opendnp3::AppConfig Conversions::convertConfig(AppConfig^ config)
	{
		return opendnp3::AppConfig(config->rspTimeout, config->numRetry, config->fragSize);
	}

	opendnp3::ClassMask Conversions::convertClassMask(ClassMask^ cm)
	{
		return opendnp3::ClassMask(cm->class1, cm->class2, cm->class3);
	}

	opendnp3::EventMaxConfig Conversions::convertEventMaxConfig(EventMaxConfig^ cm)
	{
		return opendnp3::EventMaxConfig(cm->maxBinaryEvents, cm->maxAnalogEvents, cm->maxCounterEvents, 0);
	}	

	opendnp3::StaticBinaryResponse Conversions::convert(StaticBinaryResponse rsp)
	{
		switch(rsp)
		{
			case(StaticBinaryResponse::GROUP1_VAR2):
				return opendnp3::SBR_GROUP1_VAR2;
			default:
				throw gcnew System::ArgumentException(rsp.ToString());
		}
	}

	opendnp3::StaticAnalogResponse Conversions::convert(StaticAnalogResponse rsp)
	{
		switch(rsp)
		{
			case(StaticAnalogResponse::GROUP30_VAR1):
				return opendnp3::SAR_GROUP30_VAR1;
			case(StaticAnalogResponse::GROUP30_VAR2):
				return opendnp3::SAR_GROUP30_VAR2;
			case(StaticAnalogResponse::GROUP30_VAR3):
				return opendnp3::SAR_GROUP30_VAR3;
			case(StaticAnalogResponse::GROUP30_VAR4):
				return opendnp3::SAR_GROUP30_VAR4;
			case(StaticAnalogResponse::GROUP30_VAR5):
				return opendnp3::SAR_GROUP30_VAR5;
			case(StaticAnalogResponse::GROUP30_VAR6):
				return opendnp3::SAR_GROUP30_VAR6;
			default:
				throw gcnew System::ArgumentException(rsp.ToString());
		}
	}

	opendnp3::StaticCounterResponse Conversions::convert(StaticCounterResponse rsp)
	{
		switch(rsp)
		{
			case(StaticCounterResponse::GROUP20_VAR1):
				return opendnp3::SCR_GROUP20_VAR1;
			case(StaticCounterResponse::GROUP20_VAR2):
				return opendnp3::SCR_GROUP20_VAR2;
			case(StaticCounterResponse::GROUP20_VAR5):
				return opendnp3::SCR_GROUP20_VAR5;
			case(StaticCounterResponse::GROUP20_VAR6):
				return opendnp3::SCR_GROUP20_VAR6;
			default:
				throw gcnew System::ArgumentException(rsp.ToString());
		}
	}

	opendnp3::StaticSetpointStatusResponse Conversions::convert(StaticSetpointStatusResponse rsp)
	{
		switch(rsp)
		{
			case(StaticSetpointStatusResponse::GROUP40_VAR1):
				return opendnp3::SSSR_GROUP40_VAR1;
			case(StaticSetpointStatusResponse::GROUP40_VAR2):
				return opendnp3::SSSR_GROUP40_VAR2;
			case(StaticSetpointStatusResponse::GROUP40_VAR3):
				return opendnp3::SSSR_GROUP40_VAR3;
			case(StaticSetpointStatusResponse::GROUP40_VAR4):
				return opendnp3::SSSR_GROUP40_VAR4;
			default:
				throw gcnew System::ArgumentException(rsp.ToString());
		}
	}

	opendnp3::EventBinaryResponse Conversions::convert(EventBinaryResponse rsp)
	{
		switch(rsp)
		{
			case(EventBinaryResponse::GROUP2_VAR1):
				return opendnp3::EBR_GROUP2_VAR1;
			case(EventBinaryResponse::GROUP2_VAR2):
				return opendnp3::EBR_GROUP2_VAR2;
			default:
				throw gcnew System::ArgumentException(rsp.ToString());
		}
	}
	
	opendnp3::EventAnalogResponse Conversions::convert(EventAnalogResponse rsp)
	{
		switch(rsp)
		{
			case(EventAnalogResponse::GROUP32_VAR1):
				return opendnp3::EAR_GROUP32_VAR1;
			case(EventAnalogResponse::GROUP32_VAR2):
				return opendnp3::EAR_GROUP32_VAR2;
			case(EventAnalogResponse::GROUP32_VAR3):
				return opendnp3::EAR_GROUP32_VAR3;
			case(EventAnalogResponse::GROUP32_VAR4):
				return opendnp3::EAR_GROUP32_VAR4;
			case(EventAnalogResponse::GROUP32_VAR5):
				return opendnp3::EAR_GROUP32_VAR5;
			case(EventAnalogResponse::GROUP32_VAR6):
				return opendnp3::EAR_GROUP32_VAR6;
			case(EventAnalogResponse::GROUP32_VAR7):
				return opendnp3::EAR_GROUP32_VAR7;
			case(EventAnalogResponse::GROUP32_VAR8):
				return opendnp3::EAR_GROUP32_VAR8;
			default:
				throw gcnew System::ArgumentException(rsp.ToString());
		}
	}
	
	opendnp3::EventCounterResponse Conversions::convert(EventCounterResponse rsp)
	{
		switch(rsp)
		{
			case(EventCounterResponse::GROUP22_VAR1):
				return opendnp3::ECR_GROUP22_VAR1;
			case(EventCounterResponse::GROUP22_VAR2):
				return opendnp3::ECR_GROUP22_VAR2;
			case(EventCounterResponse::GROUP22_VAR5):
				return opendnp3::ECR_GROUP22_VAR5;
			case(EventCounterResponse::GROUP22_VAR6):
				return opendnp3::ECR_GROUP22_VAR6;
			default:
				throw gcnew System::ArgumentException(rsp.ToString());
		}
	}

	opendnp3::SlaveConfig Conversions::convertConfig(SlaveConfig^ config)
	{
		opendnp3::SlaveConfig sc;

		sc.mDisableUnsol = config->disableUnsol;
		sc.mMaxControls = config->maxControls;
		sc.mUnsolMask = convertClassMask(config->unsolMask);		
		sc.mAllowTimeSync = config->allowTimeSync;
		sc.mTimeSyncPeriod = config->timeSyncPeriod;
		sc.mUnsolPackDelay = config->unsolPackDelay;
		sc.mUnsolRetryDelay = config->unsolRetryDelay;
		sc.mSelectTimeout = config->selectTimeout;
		sc.mMaxFragSize = config->maxFragSize;
		sc.mEventMaxConfig = convertEventMaxConfig(config->eventMaxConfig);
		sc.mStaticBinary = convert(config->staticBinary);
		sc.mStaticAnalog = convert(config->staticAnalog);
		sc.mStaticCounter = convert(config->staticCounter);
		sc.mStaticSetpointStatus = convert(config->staticSetpointStatus);
		sc.mEventBinary = convert(config->eventBinary);
		sc.mEventAnalog = convert(config->eventAnalog);
		sc.mEventCounter = convert(config->eventCounter);
		
		return sc;
	}

	opendnp3::PointRecord Conversions::convertRecord(PointRecord^ epr)
	{
		return opendnp3::PointRecord();
	}
				
	opendnp3::EventPointRecord Conversions::convertRecord(EventPointRecord^ epr)
	{
		return opendnp3::EventPointRecord(static_cast<opendnp3::PointClass>(epr->pointClass));
	}

	opendnp3::DeadbandPointRecord Conversions::convertRecord(DeadbandEventPointRecord^ epr)
	{
		return opendnp3::DeadbandPointRecord(static_cast<opendnp3::PointClass>(epr->pointClass), epr->deadband);
	}

	opendnp3::DeviceTemplate Conversions::convertConfig(DeviceTemplate^ config)
	{
		opendnp3::DeviceTemplate dev(	config->binaries->Count, 
										config->analogs->Count,
										config->counters->Count,
										config->controlStatii->Count,
										config->setpointStatii->Count);

		for(int i=0; i<config->binaries->Count; ++i) dev.mBinary[i] = convertRecord(config->binaries[i]);
		for(int i=0; i<config->analogs->Count; ++i) dev.mAnalog[i] = convertRecord(config->analogs[i]);
		for(int i=0; i<config->counters->Count; ++i) dev.mCounter[i] = convertRecord(config->counters[i]);
		for(int i=0; i<config->controlStatii->Count; ++i) dev.mControlStatus[i] = convertRecord(config->controlStatii[i]);
		for(int i=0; i<config->setpointStatii->Count; ++i) dev.mSetpointStatus[i] = convertRecord(config->setpointStatii[i]);		

		return dev;
	}

	opendnp3::MasterConfig Conversions::convertConfig(MasterConfig^ config)
	{
		opendnp3::MasterConfig mc;
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

	opendnp3::MasterStackConfig Conversions::convertConfig(MasterStackConfig^ config)
	{
		opendnp3::MasterStackConfig cfg;
		cfg.master = convertConfig(config->master);
		cfg.app = convertConfig(config->app);
		cfg.link = convertConfig(config->link);
		return cfg;
	}

	opendnp3::SlaveStackConfig Conversions::convertConfig(SlaveStackConfig^ config)
	{
		opendnp3::SlaveStackConfig cfg;
		cfg.slave = convertConfig(config->slave);
		cfg.device = convertConfig(config->device);
		cfg.app = convertConfig(config->app);
		cfg.link = convertConfig(config->link);
		return cfg;
	}
}}
