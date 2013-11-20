
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __I_MEASUREMENT_HANDLER_H_
#define	__I_MEASUREMENT_HANDLER_H_

#include "IMeasurementUpdate.h"

#include <functional>
#include <iostream>

namespace opendnp3
{

class IMeasurementHandler
{
public:

	virtual ~IMeasurementHandler() {}	
	virtual void Load(const IMeasurementUpdate& arUpdate) = 0;
};

class ProxiedMeasurementHandler : public IMeasurementHandler
{
public:
	ProxiedMeasurementHandler(const std::function<void (const IMeasurementUpdate&)>& arProxy) : mProxy(arProxy)
	{}

	void Load(const IMeasurementUpdate& arUpdate)
	{
		mProxy(arUpdate);
	}

private:
	std::function<void (const IMeasurementUpdate&)> mProxy;
};

class NullMeasurementHandler : public ProxiedMeasurementHandler
{

public:
	static IMeasurementHandler* Inst() { return &msInstance; }

private:
	NullMeasurementHandler() : ProxiedMeasurementHandler([](const IMeasurementUpdate&){})
	{}
	
	static NullMeasurementHandler msInstance;
};

class PrintingMeasurementHandler : public ProxiedMeasurementHandler
{

public:
	static IMeasurementHandler* Inst() { return &msInstance; }

private:
	PrintingMeasurementHandler() : ProxiedMeasurementHandler(Print)
	{}

	static void Print(const IMeasurementUpdate& arUpdate);

	template<class T>
	static void PrintAny(const T&, uint32_t aIndex);
	
	static PrintingMeasurementHandler msInstance;
};

template<class T>
void PrintingMeasurementHandler::PrintAny(const T& arPoint, uint32_t aIndex)
{
	std::ostringstream oss;
	oss << arPoint.ToString() << " : " << aIndex;
	std::cout << oss.str() << std::endl;
}



}

#endif
