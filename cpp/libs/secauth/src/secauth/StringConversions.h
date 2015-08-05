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
#ifndef SECAUTH_STRING_CONVERSIONS_H
#define SECAUTH_STRING_CONVERSIONS_H

#include <string>
#include <openpal/container/ReadBufferView.h>

namespace secauth
{
	// -- Routines for going from UTF-8 uint8_t* slices in memory to std::string and back again

	/// Makes a copy of the data and stores it in std::string
	std::string ToString(const openpal::ReadBufferView& rslice);

	/// Produces an unsigned byte view of the string w/o copying the source data
	openpal::ReadBufferView AsSlice(const std::string& str);
}

#endif

