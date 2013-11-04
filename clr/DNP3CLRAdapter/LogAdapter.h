
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
// you under the terms of the License.
//

#ifndef __LOG_ADAPTER_H_
#define __LOG_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>
#include <openpal/LogBase.h>

using namespace DNP3::Interface;

namespace DNP3
{	
namespace Adapter
{
	private class LogAdapter : public openpal::ILogBase
	{
		public:

		LogAdapter(ILogHandler^ proxy);

		// logging error messages, etc
		void Log( const openpal::LogEntry& arEntry );		

		private:		
		gcroot<ILogHandler^> proxy;
	};
	
	private ref class LogAdapterWrapper
	{
		public:
		LogAdapterWrapper(ILogHandler^ proxy) : mpAdapter(new LogAdapter(proxy))
		{}

		openpal::ILogBase* GetLogAdapter() { return mpAdapter; }

		~LogAdapterWrapper()
		{ delete mpAdapter; }

		private:
		LogAdapter* mpAdapter;
	};
}}

#endif
