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
#include "asiodnp3/MeasUpdate.h"

#include "asiodnp3/ChangeSet.h"

#include <memory>

using namespace openpal;
using namespace opendnp3;



namespace asiodnp3
{

template <class T>
void MeasUpdate::UpdateAny(const T& meas, uint16_t index, opendnp3::EventMode mode)
{
	if (this->m_use_timestamp)
	{
		T copy(meas);
		copy.time = DNPTime(this->m_timestamp.msSinceEpoch);

		auto update = [ = ](opendnp3::IDatabase & db)
		{
			db.Update(copy, index, mode);
		};
		m_changes->Add(update);

	}
	else
	{
		auto update = [ = ](opendnp3::IDatabase & db)
		{
			db.Update(meas, index, mode);
		};
		m_changes->Add(update);
	}
}

template <class T>
void MeasUpdate::ModifyAny(const openpal::Function1<const T&, T>& modify, uint16_t index, opendnp3::EventMode mode)
{
	auto update = [ = ](opendnp3::IDatabase & db)
	{
		db.Modify(modify, index, mode);
	};
	m_changes->Add(update);
}

MeasUpdate::MeasUpdate(IOutstation* outstation, openpal::UTCTimestamp timestamp) :
	m_outstation(outstation),
	m_timestamp(timestamp),
	m_use_timestamp(true),
	m_changes(new ChangeSet())
{

}

MeasUpdate::MeasUpdate(IOutstation* outstation) :
	m_outstation(outstation),
	m_use_timestamp(false),
	m_changes(new ChangeSet())
{

}

MeasUpdate::~MeasUpdate()
{
	if (m_changes->IsEmpty())
	{
		// The user didn't add anything, just delete it here
		delete m_changes;
	}
	else
	{
		auto pChangeSet = m_changes;
		auto pOut = m_outstation;

		auto update = [pChangeSet, pOut]()
		{
			pChangeSet->ApplyAll(pOut->GetDatabase());
			delete pChangeSet;
			pOut->CheckForUpdates();
		};

		m_outstation->GetExecutor().PostLambda(update);
	}
}

void MeasUpdate::Update(const Binary& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasUpdate::Update(const DoubleBitBinary& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasUpdate::Update(const Analog& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasUpdate::Update(const Counter& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasUpdate::Update(const FrozenCounter& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasUpdate::Update(const BinaryOutputStatus& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasUpdate::Update(const AnalogOutputStatus& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasUpdate::Update(const TimeAndInterval& meas, uint16_t index)
{
	auto update = [ = ](IDatabase & db)
	{
		db.Update(meas, index);
	};
	m_changes->Add(update);
}

void MeasUpdate::Modify(const openpal::Function1<const Binary&, Binary>& modify, uint16_t index, EventMode mode)
{
	this->ModifyAny(modify, index, mode);
}

void MeasUpdate::Modify(const openpal::Function1<const DoubleBitBinary&, DoubleBitBinary>& modify, uint16_t index, EventMode mode)
{
	this->ModifyAny(modify, index, mode);
}

void MeasUpdate::Modify(const openpal::Function1<const Analog&, Analog>& modify, uint16_t index, EventMode mode)
{
	this->ModifyAny(modify, index, mode);
}

void MeasUpdate::Modify(const openpal::Function1<const Counter&, Counter>& modify, uint16_t index, EventMode mode)
{
	this->ModifyAny(modify, index, mode);
}

void MeasUpdate::Modify(const openpal::Function1<const FrozenCounter&, FrozenCounter>& modify, uint16_t index, EventMode mode)
{
	this->ModifyAny(modify, index, mode);
}

void MeasUpdate::Modify(const openpal::Function1<const BinaryOutputStatus&, BinaryOutputStatus>& modify, uint16_t index, EventMode mode)
{
	this->ModifyAny(modify, index, mode);
}

void MeasUpdate::Modify(const openpal::Function1<const AnalogOutputStatus&, AnalogOutputStatus>& modify, uint16_t index, EventMode mode)
{
	this->ModifyAny(modify, index, mode);
}

void MeasUpdate::Modify(const openpal::Function1<const TimeAndInterval&, TimeAndInterval>& modify, uint16_t index)
{
	auto update = [ = ](IDatabase & db)
	{
		db.Modify(modify, index);
	};
	m_changes->Add(update);
}
}
