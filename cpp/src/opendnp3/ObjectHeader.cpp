
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

#include "ObjectHeader.h"

#include <stddef.h>

namespace opendnp3
{

AllObjectsHeader AllObjectsHeader::mInstance;

#define MACRO_CASE_DEFINE(enm) case(enm): return enm;

QualifierCode IObjectHeader::ByteToQualifierCode(uint8_t aCode)
{
	switch(aCode) {
		MACRO_CASE_DEFINE(QC_1B_START_STOP)
		MACRO_CASE_DEFINE(QC_2B_START_STOP)
		MACRO_CASE_DEFINE(QC_4B_START_STOP)

		MACRO_CASE_DEFINE(QC_ALL_OBJ)

		MACRO_CASE_DEFINE(QC_1B_CNT)
		MACRO_CASE_DEFINE(QC_2B_CNT)
		MACRO_CASE_DEFINE(QC_4B_CNT)

		MACRO_CASE_DEFINE(QC_1B_CNT_1B_INDEX)
		MACRO_CASE_DEFINE(QC_2B_CNT_2B_INDEX)
		MACRO_CASE_DEFINE(QC_4B_CNT_4B_INDEX)

		MACRO_CASE_DEFINE(QC_1B_VCNT_1B_SIZE)
		MACRO_CASE_DEFINE(QC_1B_VCNT_2B_SIZE)
		MACRO_CASE_DEFINE(QC_1B_VCNT_4B_SIZE)

	default:
		return QC_UNDEFINED;
	}
}


void IObjectHeader::Get(const uint8_t* apStart, ObjectHeaderField& arData) const
{
	arData.Group = *(apStart);
	arData.Variation = *(++apStart);
	arData.Qualifier = IObjectHeader::ByteToQualifierCode(*(++apStart));
}

void IObjectHeader::Set(uint8_t* apStart, uint8_t aGrp, uint8_t aVar, QualifierCode aQual) const
{
	*(apStart) = aGrp;
	*(++apStart) = aVar;
	*(++apStart) = aQual;
}

}

