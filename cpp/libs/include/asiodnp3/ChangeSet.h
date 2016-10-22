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

#include <vector>
#include <memory>
#include <functional>

namespace asiodnp3
{

class ChangeSet : public opendnp3::IUpdateHandler
{

public:

	void Lock();

	bool Update(const opendnp3::Binary& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	bool Update(const opendnp3::DoubleBitBinary& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	bool Update(const opendnp3::Analog& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	bool Update(const opendnp3::Counter& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	bool Update(const opendnp3::FrozenCounter& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	bool Update(const opendnp3::BinaryOutputStatus& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	bool Update(const opendnp3::AnalogOutputStatus& meas, uint16_t index, opendnp3::EventMode mode = opendnp3::EventMode::Detect);
	bool Update(const opendnp3::TimeAndInterval& meas, uint16_t index);
	bool Modify(opendnp3::FlagsType type, uint16_t start, uint16_t stop, uint8_t flags);

	void Apply(opendnp3::IUpdateHandler&) const;

	size_t Size() const;

	bool IsEmpty() const;

private:

	typedef std::function<void(opendnp3::IUpdateHandler&)> update_func_t;

	struct SharedData
	{
		std::vector<update_func_t> updates;
		bool is_locked = false;
	};

	bool Add(const update_func_t& fun);

	const std::shared_ptr<SharedData> shared = std::make_shared<SharedData>();
};

}

#endif
