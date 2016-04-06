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
#include "asiodnp3/MeasurementUpdate.h"

#include "asiodnp3/ChangeSet.h"

#include <memory>

using namespace openpal;
using namespace opendnp3;



namespace asiodnp3
{
no_value_change_tag no_value_change;


template <class T>
void MeasurementUpdate::UpdateAny(const T& meas, uint16_t index, opendnp3::EventMode mode)
{
	if (!m_changes)
	{
		m_changes = new ChangeSet;
	}
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

MeasurementUpdate::MeasurementUpdate(IOutstation* outstation, openpal::UTCTimestamp timestamp) :
	m_outstation(outstation),
	m_timestamp(timestamp),
	m_use_timestamp(true),
	m_changes(new ChangeSet)
{

}

MeasurementUpdate::MeasurementUpdate(IOutstation* outstation) :
	m_outstation(outstation),
	m_use_timestamp(false),
	m_changes(new ChangeSet)
{

}

MeasurementUpdate::~MeasurementUpdate()
{
	delete m_changes;
}

void MeasurementUpdate::Update(const Binary& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasurementUpdate::Update(const DoubleBitBinary& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasurementUpdate::Update(const Analog& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasurementUpdate::Update(const Counter& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasurementUpdate::Update(const FrozenCounter& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasurementUpdate::Update(const BinaryOutputStatus& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasurementUpdate::Update(const AnalogOutputStatus& meas, uint16_t index, EventMode mode)
{
	this->UpdateAny(meas, index, mode);
}

void MeasurementUpdate::Update(const TimeAndInterval& meas, uint16_t index)
{
	auto update = [ = ](IDatabase & db)
	{
		db.Update(meas, index);
	};
	m_changes->Add(update);
}

void MeasurementUpdate::commit()
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
	m_changes = nullptr;
}
}
