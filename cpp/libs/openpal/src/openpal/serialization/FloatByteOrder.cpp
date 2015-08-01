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

#include "FloatByteOrder.h"


namespace openpal
{

const FloatByteOrder::Value FloatByteOrder::ORDER(GetByteOrder());

FloatByteOrder::Value FloatByteOrder::GetByteOrder()
{
	/// pick a really simple test vector. This should exactly equal -20 for floating representation on x86
	uint8_t bytes[8] = { 0x00, 0x00, 0xA0, 0xC1 };

	float f = *reinterpret_cast<float*>(bytes);

	if (f == -20.0f)
	{
		return FloatByteOrder::Value::NORMAL;
	}

	return FloatByteOrder::Value::UNSUPPORTED;
}


}


