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
#ifndef ASIODNP3_MEASUPDATE_H
#define ASIODNP3_MEASUPDATE_H

#include "IMeasUpdater.h"
#include "ChangeSet.h"

#include <openpal/util/Uncopyable.h>
#include <memory>


namespace asiodnp3
{
	
class MeasUpdate : private openpal::Uncopyable
{
public:
	MeasUpdate(IMeasUpdater& updater);

	~MeasUpdate();
	
	void Update(const opendnp3::Binary& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::DoubleBitBinary& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::Analog& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::Counter& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::FrozenCounter& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::BinaryOutputStatus& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::AnalogOutputStatus& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::TimeAndInterval& meas, uint16_t index);

	void Modify(const openpal::Function1<const opendnp3::Binary&, opendnp3::Binary>& modify, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Modify(const openpal::Function1<const opendnp3::DoubleBitBinary&, opendnp3::DoubleBitBinary>& modify, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Modify(const openpal::Function1<const opendnp3::Analog&, opendnp3::Analog>& modify, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Modify(const openpal::Function1<const opendnp3::Counter&, opendnp3::Counter>& modify, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Modify(const openpal::Function1<const opendnp3::FrozenCounter&, opendnp3::FrozenCounter>& modify, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Modify(const openpal::Function1<const opendnp3::BinaryOutputStatus&, opendnp3::BinaryOutputStatus>& modify, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Modify(const openpal::Function1<const opendnp3::AnalogOutputStatus&, opendnp3::AnalogOutputStatus>& modify, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Modify(const openpal::Function1<const opendnp3::TimeAndInterval&, opendnp3::TimeAndInterval>& modify, uint16_t index);
	
private:

	template <class T>
	void UpdateAny(const T& meas, uint16_t index, opendnp3::EventMode mode);

	template <class T>
	void ModifyAny(const openpal::Function1<const T&, T>& modify, uint16_t index, opendnp3::EventMode mode);

	IMeasUpdater* pUpdater;	
	ChangeSet* pChanges;
};

template <class T>
void MeasUpdate::UpdateAny(const T& meas, uint16_t index, opendnp3::EventMode mode)
{	
	auto update = [=](IDatabase& db) { db.Update(meas, index, mode); };
	pChanges->Add(update);
}

template <class T>
void MeasUpdate::ModifyAny(const openpal::Function1<const T&, T>& modify, uint16_t index, opendnp3::EventMode mode)
{
	auto update = [=](IDatabase& db){ db.Modify(modify, index, mode); };
	pChanges->Add(update);
}

}

#endif
