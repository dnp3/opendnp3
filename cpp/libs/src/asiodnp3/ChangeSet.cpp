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

#include "asiodnp3/ChangeSet.h"

using namespace opendnp3;

namespace asiodnp3
{

void ChangeSet::Update(const opendnp3::Binary& meas, uint16_t index, opendnp3::EventMode mode)
{
	this->Add([meas, index, mode](opendnp3::IDatabase & db)
	{
		db.Update(meas, index, mode);
	});
}

void ChangeSet::Update(const opendnp3::DoubleBitBinary& meas, uint16_t index, opendnp3::EventMode mode)
{
	this->Add([meas, index, mode](opendnp3::IDatabase & db)
	{
		db.Update(meas, index, mode);
	});
}

void ChangeSet::Update(const opendnp3::Analog& meas, uint16_t index, opendnp3::EventMode mode)
{
	this->Add([meas, index, mode](opendnp3::IDatabase & db)
	{
		db.Update(meas, index, mode);
	});
}

void ChangeSet::Update(const opendnp3::Counter& meas, uint16_t index, opendnp3::EventMode mode)
{
	this->Add([meas, index, mode](opendnp3::IDatabase & db)
	{
		db.Update(meas, index, mode);
	});
}

void ChangeSet::Update(const opendnp3::FrozenCounter& meas, uint16_t index, opendnp3::EventMode mode)
{
	this->Add([meas, index, mode](opendnp3::IDatabase & db)
	{
		db.Update(meas, index, mode);
	});
}

void ChangeSet::Update(const opendnp3::BinaryOutputStatus& meas, uint16_t index, opendnp3::EventMode mode)
{
	this->Add([meas, index, mode](opendnp3::IDatabase & db)
	{
		db.Update(meas, index, mode);
	});
}

void ChangeSet::Update(const opendnp3::AnalogOutputStatus& meas, uint16_t index, opendnp3::EventMode mode)
{
	this->Add([meas, index, mode](opendnp3::IDatabase & db)
	{
		db.Update(meas, index, mode);
	});
}

void ChangeSet::Update(const opendnp3::TimeAndInterval& meas, uint16_t index)
{
	this->Add([meas, index](opendnp3::IDatabase & db)
	{
		db.Update(meas, index);
	});
}

void ChangeSet::Add(const update_func_t& fun)
{
	updates.push_back(fun);
}

void ChangeSet::Apply(opendnp3::IDatabase& db)
{
	for (auto& update : updates)
	{
		update(db);
	}
}

size_t ChangeSet::Size() const
{
	return updates.size();
}

bool ChangeSet::IsEmpty() const
{
	return updates.empty();
}

}
