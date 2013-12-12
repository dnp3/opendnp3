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

#include <opendnp3/APDUParser.h>

#include <random>
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <chrono>

#include <openpal/ToHex.h>

#include "ResultSet.h"

using namespace std;
using namespace openpal;
using namespace opendnp3;

class Handler : public IAPDUHeaderHandler
{
	public:

	virtual void AllObjects(GroupVariation gv, const openpal::ReadOnlyBuffer& header) override
	{}

	virtual void OnIIN(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<bool>>& bits) override
	{
		bits.Foreach([&](const IndexedValue<bool>& v) {});
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Binary>>& meas) override
	{
		meas.Foreach([](const IndexedValue<Binary>& v){});
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Binary>>& meas) override
	{
		meas.Foreach([](const IndexedValue<Binary>& v){});
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<ControlStatus>>& meas) override
	{
		meas.Foreach([](const IndexedValue<ControlStatus>& v){});
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Counter>>& meas) override
	{
		meas.Foreach([](const IndexedValue<Counter>& v){});
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Counter>>& meas) override
	{
		meas.Foreach([](const IndexedValue<Counter>& v){});
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Analog>>& meas) override
	{
		meas.Foreach([](const IndexedValue<Analog>& v){});
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Analog>>& meas) override
	{
		meas.Foreach([](const IndexedValue<Analog>& v){});
	}

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<SetpointStatus>>& meas) override
	{
		meas.Foreach([](const IndexedValue<SetpointStatus>& v){});
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<ControlRelayOutputBlock>>& meas) override
	{
		meas.Foreach([](const IndexedValue<ControlRelayOutputBlock>& v){});
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt16>>& meas) override
	{
		meas.Foreach([](const IndexedValue<AnalogOutputInt16>& v){});
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputInt32>>& meas) override
	{
		meas.Foreach([](const IndexedValue<AnalogOutputInt32>& v){});
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputFloat32>>& meas) override
	{
		meas.Foreach([](const IndexedValue<AnalogOutputFloat32>& v){});
	}

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<AnalogOutputDouble64>>& meas) override
	{
		meas.Foreach([](const IndexedValue<AnalogOutputDouble64>& v){});
	}
};

class Fuzzer
{
	public:

	void Run(uint64_t iter, size_t maxSize, size_t seed)
	{
		uint8_t* buffer = new uint8_t[maxSize];
		std::mt19937 gen;
		gen.seed(seed);
		std::uniform_int_distribution<size_t> size(1, maxSize);
		std::uniform_int_distribution<uint8_t> value(0x00, 0xFF);

		for(uint64_t i = 0; i<iter; ++i)
		{
			size_t count  = size(gen);
			for(size_t j = 0; j < count; ++j) buffer[j] = value(gen);
			ReadOnlyBuffer rb(buffer, count);
			APDUParser::Result result = APDUParser::ParseHeaders(rb, h);
			results.Update(result);
			if(result == APDUParser::Result::OK) {
				auto percent = static_cast<double>(i)/static_cast<double>(iter)*100.0;
				cout << "iter<" << i << "> - " << percent << "% - " << openpal::toHex(rb) << endl;
			}
		}
		delete[] buffer;
	}

	Handler h;
	ResultSet results;
};

int main(int argc, char* argv[])
{
	if(argc < 3) {
		std::cout << "iteration & size arguments required" << std::endl;
		return -1;
	}

	uint64_t iterations = std::stoull(argv[1]);
	uint32_t maxApduSize = std::stoul(argv[2]);
	size_t concurrency = std::thread::hardware_concurrency();
	uint64_t totalCases = iterations*static_cast<uint64_t>(concurrency);

	std::cout << "Running " << iterations << " iterations with a concurrency of " << concurrency << std::endl;

	std::vector<Fuzzer> fuzzers(concurrency, Fuzzer());

	auto start = std::chrono::high_resolution_clock::now();
	std::vector<std::unique_ptr<std::thread>> threads;
	size_t seed = 1;	
	for(auto& f: fuzzers) {
		std::unique_ptr<std::thread> pThread(new std::thread([&]() { f.Run(iterations, maxApduSize, seed); }));
		threads.push_back(std::move(pThread));
		++seed;
	}
	for(auto& pThread: threads) pThread->join();
	auto stop = std::chrono::high_resolution_clock::now();

	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

	auto casesPerSec = 1000.0 * (static_cast<double>(totalCases) / static_cast<double>(ms));

	ResultSet rs;
	for(auto& f: fuzzers) rs.Merge(f.results);
	
	uint64_t sum = rs.Sum();

	cout << "complete:  [ " << sum << " / " << totalCases << " ] - " << (totalCases == sum) << endl;
	cout << "fuzz tests per second: " << casesPerSec <<  " in " << ms << " milliseconds" << endl;
	cout << endl;
	cout << "OK: " << rs.numOK << endl;
	cout << "NotEnoughDataForHeader: " << rs.numNotEnoughDataForHeader << endl;
	cout << "NotEnoughDataForRange: " << rs.numNotEnoughDataForRange << endl;
	cout << "NotEnoughDataForObjects: " << rs.numNotEnoughDataForObjects << endl;
	cout << "UnReasonableObjectCount: " << rs.numUnReasonableObjectCount << endl;
	cout << "UnknownObject: " << rs.numUnknownObject << endl;
	cout << "UnknownQualifier: " << rs.numUnknownQualifier << endl;
	cout << "IllegalObjectQualifier: " << rs.numIllegalObjectQualifier << endl;
	cout << "BadStartStop: " << rs.numBadStartStop << endl;

	return 0;
}
