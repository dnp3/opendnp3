
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
#ifndef __MASTER_DATA_OBSERVER_H_
#define __MASTER_DATA_OBSERVER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/IDataObserver.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{	
namespace Adapter
{		
	private class MasterDataObserverAdapter : public opendnp3::IDataObserver
	{
		public:

		MasterDataObserverAdapter(DNP3::Interface::IDataObserver^ proxy);

		private:

		gcroot<DNP3::Interface::IDataObserver^> proxy;

		protected:

		void _Start();
		void _Update(const opendnp3::Binary& arPoint, size_t aIndex);
		void _Update(const opendnp3::Analog& arPoint, size_t aIndex);
		void _Update(const opendnp3::Counter& arPoint, size_t aIndex);
		void _Update(const opendnp3::ControlStatus& arPoint, size_t aIndex);
		void _Update(const opendnp3::SetpointStatus& arPoint, size_t aIndex);
		void _End();
	};

	public ref class MasterDataObserverWrapper
	{
		public:

		MasterDataObserverWrapper(DNP3::Interface::IDataObserver^ proxy) :
			mpAdapter(new MasterDataObserverAdapter(proxy))
		{}

		~MasterDataObserverWrapper()
		{ 
			delete mpAdapter;
		}

		opendnp3::IDataObserver* Get() { return mpAdapter; }
		
		private:
		MasterDataObserverAdapter* mpAdapter;
	};
	
}}

#endif
