/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "LogAdapter.h"

#include "Conversions.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			LogAdapter::LogAdapter(Automatak::DNP3::Interface::ILogHandler^ proxy) : proxy(proxy)
			{}

			std::shared_ptr<openpal::ILogHandler> LogAdapter::Create(Automatak::DNP3::Interface::ILogHandler^ proxy)
			{
				return std::shared_ptr<openpal::ILogHandler>(new LogAdapter(proxy));
			}

			// logging error messages, etc
			void LogAdapter::Log(const openpal::LogEntry& entry)
			{
				System::String^ alias = Conversions::ConvertString(entry.loggerid);
				System::String^ location = Conversions::ConvertString(entry.location);
				System::String^ message = Conversions::ConvertString(entry.message);

				Automatak::DNP3::Interface::LogEntry^ le = gcnew Automatak::DNP3::Interface::LogEntry(entry.filters.GetBitfield(), alias, location, message);

				proxy->Log(le);
			}

		}
	}
}
