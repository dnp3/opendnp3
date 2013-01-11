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
#ifndef __SLAVE_COMMAND_ACCEPTOR_ADAPTER_H_
#define __SLAVE_COMMAND_ACCEPTOR_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <APL/CommandInterfaces.h>

using namespace DNP3::Interface;

namespace DNP3
{	
namespace Adapter
{				
	//this object goes into the stack
	class SlaveCommandAcceptorAdapter : public apl::ICommandAcceptor
	{
		public:
			SlaveCommandAcceptorAdapter(DNP3::Interface::ICommandAcceptor^ proxy);

			void AcceptCommand(const apl::BinaryOutput&, size_t, int aSequence, apl::IResponseAcceptor* apRspAcceptor);
			void AcceptCommand(const apl::Setpoint&, size_t, int aSequence, apl::IResponseAcceptor* apRspAcceptor);

		private:
			gcroot<DNP3::Interface::ICommandAcceptor^> proxy;
	};

	public ref class SlaveCommandAcceptorAdapterWrapper
	{		
		public:

		SlaveCommandAcceptorAdapterWrapper(ICommandAcceptor^ proxy);
		~SlaveCommandAcceptorAdapterWrapper();

		apl::ICommandAcceptor* GetCommandAcceptor() { return pAdapter; }

		private:		
		SlaveCommandAcceptorAdapter* pAdapter;
	};

	public ref class ResponseDelegateAdapter
	{
		public: 
			ResponseDelegateAdapter(apl::IResponseAcceptor* apRspAcceptor, int aSequence);

			void OnResult(CommandStatus status);

		private:
			
			apl::IResponseAcceptor* mpRspAcceptor;
			int mSequence;			
	};
	
}}

#endif
