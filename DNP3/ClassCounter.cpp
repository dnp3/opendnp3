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
#include "ClassCounter.h"


#include <assert.h>

namespace apl
{
namespace dnp
{

void ClassCounter::IncrCount(PointClass aClass)
{
	switch(aClass) {
	case(PC_CLASS_1):
		mNumClass1++;
		break;
	case(PC_CLASS_2):
		mNumClass2++;
		break;
	case(PC_CLASS_3):
		mNumClass3++;
		break;
	default:
		break;
	}
}

void ClassCounter::DecrCount(PointClass aClass)
{
	switch(aClass) {
	case(PC_CLASS_1):
		assert(mNumClass1 > 0);
		mNumClass1--;
		break;
	case(PC_CLASS_2):
		assert(mNumClass2 > 0);
		mNumClass2--;
		break;
	case(PC_CLASS_3):
		assert(mNumClass3 > 0);
		mNumClass3--;
		break;
	default:
		break;
	}
}

}
}

