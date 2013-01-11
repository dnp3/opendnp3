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
#ifndef __CLASS_MASK_H_
#define __CLASS_MASK_H_

#include "PointClass.h"

namespace apl
{
namespace dnp
{

struct ClassMask {
	ClassMask(bool c1, bool c2, bool c3) :
		class1(c1),
		class2(c2),
		class3(c3)
	{}

	ClassMask() :
		class1(false),
		class2(false),
		class3(false)
	{}

	bool class1;
	bool class2;
	bool class3;

	static int GetMask(bool c1, bool c2, bool c3) {
		int m = 0;
		if(c1) m |= PC_CLASS_1;
		if(c2) m |= PC_CLASS_2;
		if(c3) m |= PC_CLASS_3;
		return m;
	}

	bool IsEnabled() {
		return class1 || class2 || class3;
	}
};

}
}

#endif
