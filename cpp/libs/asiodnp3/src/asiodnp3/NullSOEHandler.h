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
#ifndef ASIODNP3_NULL_SOE_HANDLER_H
#define ASIODNP3_NULL_SOE_HANDLER_H

#include <opendnp3/master/ISOEHandler.h>

namespace opendnp3
{

class NullSOEHandler final : public ISOEHandler
{

public:

	static ISOEHandler& Instance()
	{
		return instance;
	}

	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<OctetString>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<TimeAndInterval>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryCommandEvent>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogCommandEvent>>& values) override final  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<SecurityStat>>& values) override final  {}
	

protected:

	void Start() override {}
	void End() override {}

private:	



	NullSOEHandler()
	{}

	static NullSOEHandler instance;
};

}

#endif
