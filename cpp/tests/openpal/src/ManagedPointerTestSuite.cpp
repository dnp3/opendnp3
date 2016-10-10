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
#include <catch.hpp>

#include <openpal/container/ManagedPtr.h>

using namespace openpal;

#include <vector>

#define SUITE(name) "ManagedPointerTestSuite - " name

struct Flags
{
	Flags(int x_, int y_) : x(x_), y(y_)
	{}

	int x;
	int y;
};

TEST_CASE(SUITE("ManagedPointersCanCreatedViaPoinerToStack"))
{
	Flags stack = { 4, 3 };
	auto pFlags = ManagedPtr<Flags>::WrapperOnly(&stack);

	REQUIRE(pFlags->x == 4);
	REQUIRE(pFlags->y == 3);

	pFlags->x = 10;
	pFlags->y = 20;

	REQUIRE(pFlags->x == 10);
	REQUIRE(pFlags->y == 20);
}

TEST_CASE(SUITE("ContainerTypesLikeVectorCanHoldAMixtureOfManagedAndUnmanaged"))
{
	std::vector<ManagedPtr<Flags>> container;

	Flags stack = { 4, 3 };
	container.push_back(ManagedPtr<Flags>::WrapperOnly(&stack));
	container.push_back(ManagedPtr<Flags>::WrapperOnly(&stack));
	container.push_back(ManagedPtr<Flags>::Deleted(new Flags { 10, 20 }));
	container.push_back(ManagedPtr<Flags>::Deleted(new Flags { 30, 40 }));
}


TEST_CASE(SUITE("ManagedPointersCanBeDereferenced"))
{
	auto pFlags = ManagedPtr<Flags>::Deleted(new Flags { 4, 3 });

	REQUIRE(pFlags->x == 4);
	REQUIRE(pFlags->y == 3);

	pFlags->x = 10;
	pFlags->y = 20;

	REQUIRE(pFlags->x == 10);
	REQUIRE(pFlags->y == 20);
}

TEST_CASE(SUITE("ManagedPointersCanBeMovementConstructed"))
{
	auto pInt = ManagedPtr<int>::Deleted(new int);
	ManagedPtr<int> copy(std::move(pInt));

	REQUIRE_FALSE(pInt.IsDefined());
	REQUIRE(copy.IsDefined());
}

TEST_CASE(SUITE("ManagedPointersCanBeMovementAssigned"))
{
	auto pInt = ManagedPtr<int>::Deleted(new int);
	auto pInt2 = std::move(pInt);

	REQUIRE_FALSE(pInt.IsDefined());
	REQUIRE(pInt2.IsDefined());
}




