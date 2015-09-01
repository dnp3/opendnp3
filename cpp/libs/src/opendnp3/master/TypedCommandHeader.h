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
#ifndef OPENDNP3_TYPED_COMMAND_HEADER_H
#define OPENDNP3_TYPED_COMMAND_HEADER_H

#include "opendnp3/master/ICommandHeader.h"
#include "opendnp3/master/CommandResponse.h"
#include "opendnp3/master/ICommandCollection.h"

#include "opendnp3/app/HeaderWriter.h"
#include "opendnp3/app/parsing/ICollection.h"
#include "opendnp3/app/Indexed.h"


#include <vector>
#include <algorithm>

namespace opendnp3
{

enum class SelectState : uint8_t
{
	INIT,
	FAIL,
	SUCCESS
};

template <class T>
class TypedCommandHeader final : public ICommandHeader, public ICollection<Indexed<CommandResponse>>, public ICommandCollection<T>
{	
	struct Record
	{
		Record(const Indexed<T>& command_) : 
			command(command_),
			response(TaskCompletion::FAILURE_BAD_RESPONSE),
			state(SelectState::INIT)
		{}

		Indexed<T> command;
		CommandResponse response;	
		SelectState state;
	};

public:

	TypedCommandHeader(const DNP3Serializer<T>& serializer) : m_serializer(serializer)
	{}

	// --- Implement ICommandCollection ---

	virtual void Add(const T& command, uint16_t index) override;
	
	// --- Implement ICommandHeader ----
	
	virtual bool Write(HeaderWriter&) override;

	virtual bool VerifySelect(const HeaderInfo& info, const ICollection<Indexed<T>>& commands) override;	

	virtual bool VerifyOperate(const HeaderInfo& info, const ICollection<Indexed<T>>& commands) override;

	// --- Implement ICollection<Indexed<CommandResponse>> ----

	virtual uint32_t Count() const override;

	virtual void Foreach(IVisitor<Indexed<CommandResponse>>& visitor) const override;

private:

	DNP3Serializer<T> m_serializer;
	std::vector<Record> m_records;
};


template <class T>
void TypedCommandHeader<T>::Add(const T& command, uint16_t index)
{
	m_records.push_back(WithIndex(command, index));
}

template <class T>
bool TypedCommandHeader<T>::Write(HeaderWriter& writer)
{	
	if (m_records.empty())
	{
		return false;
	}

	auto iter = writer.IterateOverCountWithPrefix<openpal::UInt16, T>(QualifierCode::UINT16_CNT_UINT16_INDEX, m_serializer);

	for(auto& rec: m_records)
	{
		if (!iter.Write(rec.command.value, rec.command.index))
		{
			return false;
		}
	}
	
	return iter.IsValid();
}

template <class T>
bool TypedCommandHeader<T>::VerifySelect(const HeaderInfo& info, const ICollection<Indexed<T>>& commands)
{
	if (commands.Count() > m_records.size())
	{
		return false;
	}
	
	uint32_t index = 0;

	auto visit = [&](const Indexed<T> item) 
	{
		auto& rec = m_records[index];
		++index;

		if (item.index != rec.command.index)
		{
			return;
		}

		if (!item.value.ValuesEqual(rec.command.value))
		{
			rec.state = SelectState::FAIL;
		}

		if (item.value.status != CommandStatus::SUCCESS)
		{
			rec.state = SelectState::FAIL;
		}

		if (rec.state == SelectState::INIT)
		{
			rec.state = SelectState::SUCCESS;
		}		
	}; 

	commands.ForeachItem(visit);

	auto isSuccess = [](const Record& rec) -> bool { return rec.state == SelectState::SUCCESS; };

	return std::all_of(m_records.begin(), m_records.end(), isSuccess);
}

template <class T>
bool TypedCommandHeader<T>::VerifyOperate(const HeaderInfo& info, const ICollection<Indexed<T>>& commands)
{	
	if (commands.Count() > m_records.size())
	{
		return false;
	}

	uint32_t index = 0;

	auto visit = [&](const Indexed<T> item)
	{		
		auto& rec = m_records[index];
		++index; 

		if (item.index != rec.command.index)
		{
			return;
		}

		if (item.value.ValuesEqual(rec.command.value))
		{
			rec.response = CommandResponse::CommandResponse(TaskCompletion::FAILURE_BAD_RESPONSE);
			return;
		}

		rec.response = CommandResponse::OK(item.value.status);		
	};

	commands.ForeachItem(visit);

	auto isSuccess = [](const Record& rec) -> bool { return rec.response.GetResult() == TaskCompletion::SUCCESS; };

	return std::all_of(m_records.begin(), m_records.end(), isSuccess);
}

template <class T>
uint32_t TypedCommandHeader<T>::Count() const
{
	return m_records.size();
}

template <class T>
void TypedCommandHeader<T>::Foreach(IVisitor<Indexed<CommandResponse>>& visitor) const
{
	for(auto& rec : m_records)
	{		
		visitor.OnValue(WithIndex(rec.response, rec.command.index));
	}
}

}

#endif
