/*
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
#ifndef SECAUTH_SINGLE_VALUE_HANDLER_H
#define SECAUTH_SINGLE_VALUE_HANDLER_H

#include <opendnp3/app/parsing/IAPDUHandler.h>

namespace secauth
{

template <class ValueType, opendnp3::GroupVariation GV, opendnp3::QualifierCode QC>
class SingleValueHandler: public opendnp3::IAPDUHandler, private openpal::Uncopyable
{
public:

	SingleValueHandler() : m_valid(false)
	{}

	ValueType value;

	bool IsValid() const
	{
		return m_valid;
	}

	virtual bool IsAllowed(uint32_t headerCount, opendnp3::GroupVariation gv, opendnp3::QualifierCode qc) override final
	{
		return (headerCount == 0) && (gv == GV) && (qc == QC);
	}

protected:
	bool m_valid;

};


}

#endif

