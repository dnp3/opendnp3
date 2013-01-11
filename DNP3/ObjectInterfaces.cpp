//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
// contributor license agreements. See the NOTICE file distributed with this
// work for additional information regarding copyright ownership.  Green Enery
// Corp licenses this file to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance with the
// License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
//
#include "ObjectInterfaces.h"

#include "Objects.h"
#include <APL/DataTypes.h>

#include <memory>

/**
 * Creates a single number representation of a group ID and variation ID for
 * comparison purposes.
 */
#define RADIX(group,var)		(group << 4) | var

/**
 * Creates a 'case' statement for use in a 'switch' block to test against
 * RADIX(group,var) and return Group<group>Var<var>::Inst() if a match is
 * found.
 */
#define MACRO_RADIX_CASE(group, var) \
		case (RADIX(group, var)): \
		{ \
			return Group##group##Var##var::Inst(); \
		}

/**
 * Creates a 'case' statement for use in a 'switch' block to test against
 * the group identifier and return Group<group>Var0::Inst() if a match is
 * found.
 */
#define MACRO_GROUP_CASE(group) \
 		case (group): \
		{ \
			return Group##group##Var0::Inst(); \
		}

namespace apl
{
namespace dnp
{

ObjectBase* ObjectBase::Get(int aGroup, int aVariation)
{
	switch (RADIX(aGroup, aVariation)) {
		// Binary Input
		MACRO_RADIX_CASE(1, 0);
		MACRO_RADIX_CASE(1, 1);
		MACRO_RADIX_CASE(1, 2);

		// Binary Input Events
		MACRO_RADIX_CASE(2, 0);
		MACRO_RADIX_CASE(2, 1);
		MACRO_RADIX_CASE(2, 2);
		MACRO_RADIX_CASE(2, 3);

		// Binary Output
		MACRO_RADIX_CASE(10, 0);
		MACRO_RADIX_CASE(10, 1);
		MACRO_RADIX_CASE(10, 2);

		// Control blocks
		MACRO_RADIX_CASE(12, 1);
		MACRO_RADIX_CASE(12, 2);
		MACRO_RADIX_CASE(12, 3);

		// Counters
		MACRO_RADIX_CASE(20, 0);
		MACRO_RADIX_CASE(20, 1);
		MACRO_RADIX_CASE(20, 2);
		MACRO_RADIX_CASE(20, 3);
		MACRO_RADIX_CASE(20, 4);
		MACRO_RADIX_CASE(20, 5);
		MACRO_RADIX_CASE(20, 6);
		MACRO_RADIX_CASE(20, 7);
		MACRO_RADIX_CASE(20, 8);

		// Frozen Counters
		MACRO_RADIX_CASE(21, 0);
		MACRO_RADIX_CASE(21, 1);
		MACRO_RADIX_CASE(21, 2);
		MACRO_RADIX_CASE(21, 3);
		MACRO_RADIX_CASE(21, 4);
		MACRO_RADIX_CASE(21, 5);
		MACRO_RADIX_CASE(21, 6);
		MACRO_RADIX_CASE(21, 7);
		MACRO_RADIX_CASE(21, 8);
		MACRO_RADIX_CASE(21, 9);
		MACRO_RADIX_CASE(21, 10);
		MACRO_RADIX_CASE(21, 11);
		MACRO_RADIX_CASE(21, 12);

		// Counter Event
		MACRO_RADIX_CASE(22, 0);
		MACRO_RADIX_CASE(22, 1);
		MACRO_RADIX_CASE(22, 2);
		MACRO_RADIX_CASE(22, 3);
		MACRO_RADIX_CASE(22, 4);
		MACRO_RADIX_CASE(22, 5);
		MACRO_RADIX_CASE(22, 6);
		MACRO_RADIX_CASE(22, 7);
		MACRO_RADIX_CASE(22, 8);

		// Frozen Counter Event
		MACRO_RADIX_CASE(23, 0);
		MACRO_RADIX_CASE(23, 1);
		MACRO_RADIX_CASE(23, 2);
		MACRO_RADIX_CASE(23, 3);
		MACRO_RADIX_CASE(23, 4);
		MACRO_RADIX_CASE(23, 5);
		MACRO_RADIX_CASE(23, 6);
		MACRO_RADIX_CASE(23, 7);
		MACRO_RADIX_CASE(23, 8);

		// Analog
		MACRO_RADIX_CASE(30, 0);
		MACRO_RADIX_CASE(30, 1);
		MACRO_RADIX_CASE(30, 2);
		MACRO_RADIX_CASE(30, 3);
		MACRO_RADIX_CASE(30, 4);
		MACRO_RADIX_CASE(30, 5);
		MACRO_RADIX_CASE(30, 6);

		// Frozen Analogs
		MACRO_RADIX_CASE(31, 0);
		MACRO_RADIX_CASE(31, 1);
		MACRO_RADIX_CASE(31, 2);
		MACRO_RADIX_CASE(31, 3);
		MACRO_RADIX_CASE(31, 4);
		MACRO_RADIX_CASE(31, 5);
		MACRO_RADIX_CASE(31, 6);

		// Analog events
		MACRO_RADIX_CASE(32, 0);
		MACRO_RADIX_CASE(32, 1);
		MACRO_RADIX_CASE(32, 2);
		MACRO_RADIX_CASE(32, 3);
		MACRO_RADIX_CASE(32, 4);
		MACRO_RADIX_CASE(32, 5);
		MACRO_RADIX_CASE(32, 6);
		MACRO_RADIX_CASE(32, 7);
		MACRO_RADIX_CASE(32, 8);

		// Frozen Analog events
		MACRO_RADIX_CASE(33, 0);
		MACRO_RADIX_CASE(33, 1);
		MACRO_RADIX_CASE(33, 2);
		MACRO_RADIX_CASE(33, 3);
		MACRO_RADIX_CASE(33, 4);
		MACRO_RADIX_CASE(33, 5);
		MACRO_RADIX_CASE(33, 6);
		MACRO_RADIX_CASE(33, 7);
		MACRO_RADIX_CASE(33, 8);

		// Analog Output
		MACRO_RADIX_CASE(40, 0);
		MACRO_RADIX_CASE(40, 1);
		MACRO_RADIX_CASE(40, 2);
		MACRO_RADIX_CASE(40, 3);
		MACRO_RADIX_CASE(40, 4);

		// Analog Input
		MACRO_RADIX_CASE(41, 1);
		MACRO_RADIX_CASE(41, 2);
		MACRO_RADIX_CASE(41, 3);
		MACRO_RADIX_CASE(41, 4);

		// Time Objects
		MACRO_RADIX_CASE(50, 1);
		MACRO_RADIX_CASE(50, 2);
		MACRO_RADIX_CASE(51, 1);
		MACRO_RADIX_CASE(51, 2);
		MACRO_RADIX_CASE(52, 1);
		MACRO_RADIX_CASE(52, 2);

		// Class Objects
		MACRO_RADIX_CASE(60, 1);
		MACRO_RADIX_CASE(60, 2);
		MACRO_RADIX_CASE(60, 3);
		MACRO_RADIX_CASE(60, 4);

		// Device Objects
		MACRO_RADIX_CASE(80, 1);
	}

	switch (aGroup) {
		/* Octet String Objects */
		MACRO_GROUP_CASE(110);
		MACRO_GROUP_CASE(111);

		/* Virtual Terminal Objects */
		MACRO_GROUP_CASE(112);
		MACRO_GROUP_CASE(113);
	}

	/* Nothing matched if we reach this point */
	return NULL;
}

}
}

/* vim: set ts=4 sw=4: */
