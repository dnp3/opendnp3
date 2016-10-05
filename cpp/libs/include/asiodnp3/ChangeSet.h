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
#ifndef ASIODNP3_CHANGESET_H
#define ASIODNP3_CHANGESET_H

#include <opendnp3/outstation/IUpdateHandler.h>

#include <openpal/util/Uncopyable.h>

#include <vector>
#include <functional>

namespace asiodnp3
{

class ChangeSet : private openpal::Uncopyable
{

public:

	ChangeSet() {}

	ChangeSet(ChangeSet&& other) : updates(std::move(other.updates))
	{

	}

	void Update(const opendnp3::Binary& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::DoubleBitBinary& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::Analog& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::Counter& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::FrozenCounter& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::BinaryOutputStatus& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::AnalogOutputStatus& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	void Update(const opendnp3::TimeAndInterval& meas, uint16_t index);	

	void Apply(opendnp3::IUpdateHandler&);

	size_t Size() const;

	bool IsEmpty() const;

private:

	typedef std::function<void(opendnp3::IUpdateHandler&)> update_func_t;

	void Add(const update_func_t& fun);

	std::vector<update_func_t> updates;
};

}

#endif
