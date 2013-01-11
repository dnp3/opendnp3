
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
#ifndef __DOT_NET_STACK_MANAGER_H_
#define __DOT_NET_STACK_MANAGER_H_

using namespace System;
using namespace DNP3::Interface;

namespace apl { namespace dnp {
	class StackManager;
}}

namespace DNP3
{	
namespace Adapter
{
	public ref class StackManager
	{
		public:
			StackManager();
			
			void AddTCPClient(System::String^ name, FilterLevel level, System::UInt64 retryMs, System::String^ address, System::UInt16 port);					
			void AddTCPServer(System::String^ name, FilterLevel level, System::UInt64 retryMs, System::String^ endpoint, System::UInt16 port);
			void AddSerial(System::String^ name, FilterLevel level, System::UInt64 retryMs, SerialSettings^ settings);
						
			ICommandAcceptor^ AddMaster(	System::String^ portName,
											System::String^ stackName,	                            
											FilterLevel level,
											IDataObserver^ publisher,
											MasterStackConfig^ config);

			IDataObserver^	 AddSlave(	System::String^ portName,
										System::String^ stackName,
										FilterLevel level,
										ICommandAcceptor^ cmdAcceptor, 
										DNP3::Interface::SlaveStackConfig^ config);

			void RemovePort(System::String^ portName);

			void RemoveStack(System::String^ stackName);

			void AddLogHandler(ILogHandler^ logHandler);

		private:
			apl::dnp::StackManager* pMgr;
	};

}}

#endif
