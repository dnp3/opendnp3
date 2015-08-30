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

#include "asiodnp3/MeasUpdate.h"

using namespace openpal;
using namespace opendnp3;

#include <memory>

namespace asiodnp3
{
MeasUpdate::MeasUpdate(IOutstation* pOutstation_) :
	pOutstation(pOutstation_),
	pChanges(new ChangeSet())
{

}

MeasUpdate::~MeasUpdate()
{
	if (pChanges->IsEmpty())
	{
		// The user didn't add anything, just delete it here
		delete pChanges;
	}
	else
	{
		auto pChangeSet = pChanges;
		auto pOut = pOutstation;

		auto update = [pChangeSet, pOut]()
		{
			pChangeSet->ApplyAll(pOut->GetDatabase());
			delete pChangeSet;
			pOut->CheckForUpdates();
		};

		pOutstation->GetExecutor().PostLambda(update);
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
	pChanges->Add(update);
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
	pChanges->Add(update);
}
}
