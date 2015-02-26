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

#include "Group120Var9.h"

namespace opendnp3 {
	
	Group120Var9::Group120Var9(const openpal::ReadBufferView& hmacValue_) : hmacValue(hmacValue_) {}	
	
	bool Group120Var9::Read(const openpal::ReadBufferView& buffer, Group120Var9& output)
	{
		output.hmacValue = buffer;
		return true;
	}

	bool Group120Var9::Write(const Group120Var9& output, openpal::WriteBufferView& buffer)
	{
		if (buffer.Size() < output.Size())
		{
			return false;
		}
		else
		{
			output.hmacValue.CopyTo(buffer);
			return true;
		}
	}
}


