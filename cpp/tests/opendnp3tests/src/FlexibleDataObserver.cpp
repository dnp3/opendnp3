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
#include "FlexibleDataObserver.h"

#include "MeasurementComparisons.h"

#include <iostream>

namespace opendnp3
{

FlexibleDataObserver::FlexibleDataObserver() : mNewData(false)
{}

void FlexibleDataObserver::Clear()
{
	Transaction t(this);
	mBinaryMap.clear();
	mAnalogMap.clear();
	mCounterMap.clear();
	mFrozenCounterMap.clear();
	mBinaryOutputStatusMap.clear();
	mAnalogOutputStatusMap.clear();
}

// The RHS is a strict subset of the LHS... i.e. everything in the RHS can be found in the LHS
bool FlexibleDataObserver::StrictEquality(const FlexibleDataObserver& arLHS, const FlexibleDataObserver& arRHS)
{
	if(! StrictEquality(arLHS.mBinaryMap, arRHS.mBinaryMap) ) return false;
	if(! StrictEquality(arLHS.mAnalogMap, arRHS.mAnalogMap) ) return false;
	if(! StrictEquality(arLHS.mCounterMap, arRHS.mCounterMap) ) return false;
	if(! StrictEquality(arLHS.mFrozenCounterMap, arRHS.mFrozenCounterMap) ) return false;
	if(! StrictEquality(arLHS.mBinaryOutputStatusMap, arRHS.mBinaryOutputStatusMap) ) return false;
	if(! StrictEquality(arLHS.mAnalogOutputStatusMap, arRHS.mAnalogOutputStatusMap) ) return false;

	return true;
}

// The RHS is a strict subset of the LHS... i.e. everything in the RHS can be found in the LHS
bool FlexibleDataObserver::IsSubsetOf(const FlexibleDataObserver& arLHS, const FlexibleDataObserver& arRHS)
{
	if(! IsSubsetOf(arLHS.mBinaryMap, arRHS.mBinaryMap) ) return false;
	if(! IsSubsetOf(arLHS.mAnalogMap, arRHS.mAnalogMap) ) return false;
	if(! IsSubsetOf(arLHS.mCounterMap, arRHS.mCounterMap) ) return false;
	if(! IsSubsetOf(arLHS.mFrozenCounterMap, arRHS.mFrozenCounterMap) ) return false;
	if(! IsSubsetOf(arLHS.mBinaryOutputStatusMap, arRHS.mBinaryOutputStatusMap) ) return false;
	if(! IsSubsetOf(arLHS.mAnalogOutputStatusMap, arRHS.mAnalogOutputStatusMap) ) return false;

	return true;
}

void FlexibleDataObserver::Print()
{
	Transaction tr(this);

	std::cout << "--- Binary ---" << std::endl;
	this->Print<Binary>(mBinaryMap);

	std::cout << "--- Analog ---" << std::endl;
	this->Print<Analog>(mAnalogMap);

	std::cout << "--- Counter ---" << std::endl;
	this->Print<Counter>(mCounterMap);

	std::cout << "--- Frozen Counter ---" << std::endl;
	this->Print<FrozenCounter>(mFrozenCounterMap);

	std::cout << "--- Control Status ---" << std::endl;
	this->Print<BinaryOutputStatus>(mBinaryOutputStatusMap);

	std::cout << "--- Setpoint Status ---" << std::endl;
	this->Print<AnalogOutputStatus>(mAnalogOutputStatusMap);

}

template<class T>
bool FlexibleDataObserver::StrictEquality(const T& arMap1, const T& arMap2)
{
	if(arMap1.size() != arMap2.size()) return false;
	return IsSubsetOf(arMap1, arMap2);
}

template<class T>
bool FlexibleDataObserver::IsSubsetOf(const T& arMap1, const T& arMap2)
{
	for(auto pair1 : arMap1)
	{
		typename T::const_iterator j = arMap2.find(pair1.first);
		if(j == arMap2.end()) return false;
		if(j->second != pair1.second) return false;
	}

	return true;
}

}

