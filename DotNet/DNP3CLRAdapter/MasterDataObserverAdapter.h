//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __MASTER_DATA_OBSERVER_H_
#define __MASTER_DATA_OBSERVER_H_

using namespace System::Collections::ObjectModel;

#include <APL/DataInterfaces.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{	
namespace Adapter
{		
	public class MasterDataObserverAdapter : public apl::IDataObserver
	{
		public:

		MasterDataObserverAdapter(DNP3::Interface::IDataObserver^ proxy);

		private:

		gcroot<DNP3::Interface::IDataObserver^> proxy;

		protected:

		void _Start();
		void _Update(const apl::Binary& arPoint, size_t aIndex);
		void _Update(const apl::Analog& arPoint, size_t aIndex);
		void _Update(const apl::Counter& arPoint, size_t aIndex);
		void _Update(const apl::ControlStatus& arPoint, size_t aIndex);
		void _Update(const apl::SetpointStatus& arPoint, size_t aIndex);
		void _End();
	};

	public ref class MasterDataObserverAdapterWrapper
	{
		public:
			MasterDataObserverAdapterWrapper(DNP3::Interface::IDataObserver^ proxy) : 
			  pAdapter(new MasterDataObserverAdapter(proxy))
			{}

			~MasterDataObserverAdapterWrapper() 
			{ delete pAdapter; }

			apl::IDataObserver* GetDataObserver() { return pAdapter; }

		private:
			MasterDataObserverAdapter* pAdapter;
	};
}}

#endif
