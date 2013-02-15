
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
#ifndef __SIMPLE_DATA_OBSERVER_H_
#define	__SIMPLE_DATA_OBSERVER_H_

#include "IDataObserver.h"
#include "Singleton.h"

#include <functional>
#include <iostream>

namespace opendnp3
{

class SimpleDataObserver : public IDataObserver
{
public: 
	SimpleDataObserver(std::function<void (const std::string&)> aOutputFunc);
			
protected:

	//concrete class will implement these
	void _Start();
	void _Update(const Binary& arPoint, size_t);
	void _Update(const Analog& arPoint, size_t);
	void _Update(const Counter& arPoint, size_t);
	void _Update(const ControlStatus& arPoint, size_t);
	void _Update(const SetpointStatus& arPoint, size_t);
	void _End();

private:
	std::function<void (std::string)> mOutputFunc;

};

class NullDataObserver : public SimpleDataObserver
{
	private:
		static NullDataObserver mInstance;
	protected:
		NullDataObserver() : SimpleDataObserver([](const std::string&){})
		{}
	public:
		static NullDataObserver* Inst(){ return &mInstance; }	
};

class PrintingDataObserver : public SimpleDataObserver
{
	private:
		static PrintingDataObserver mInstance;
	protected:
		PrintingDataObserver() : SimpleDataObserver([](const std::string& arStr){ std::cout << arStr << std::endl;})
		{}
	public:
		static PrintingDataObserver* Inst(){ return &mInstance; }
};

}

#endif
