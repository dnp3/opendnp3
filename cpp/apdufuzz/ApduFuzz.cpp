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

#include "ResultSet.h"

using namespace std;
using namespace openpal;
using namespace opendnp3;

class Handler : public IAPDUHeaderHandler
{
	public:

		virtual void AllObjects(GroupVariation gv) override
		{}

		virtual void OnRange(const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Binary>>& meas) override
		{
			meas.Foreach([](const IndexedValue<Binary>& v){});
		}

		virtual void OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const LazyIterable<IndexedValue<Binary>>& meas) override
		{
			meas.Foreach([](const IndexedValue<Binary>& v){});
		}
};

class Fuzzer
{
	public:

	Fuzzer(size_t iter, size_t s) : seed(s), iterations(iter)
	{}

	static const size_t MAX_SIZE = 100;

	void Run()
	{
		Handler h;
		std::mt19937 gen;
		gen.seed(seed);
		std::uniform_int_distribution<size_t> size(1, MAX_SIZE);
		std::uniform_int_distribution<uint8_t> value(0x00, 0xFF);

		for(size_t i = 0; i<iterations; ++i)
		{
			size_t count  = size(gen);
			for(size_t i = 0; i < count; ++i) buffer[i] = value(gen);
			ReadOnlyBuffer rb(buffer, count);
			APDUParser::Result result = APDUParser::ParseHeaders(rb, h);
			results.Update(result);
		}
	}

	ResultSet results;

	private:

	size_t seed;
	size_t iterations;
	uint8_t buffer[MAX_SIZE];
};

int main(int argc, char* argv[])
{
	size_t iterations = 10*1000*1000;
	size_t concurrency = std::thread::hardware_concurrency();

	std::vector<Fuzzer> fuzzers;
	for(size_t i = 0;  i <  concurrency; ++i) fuzzers.push_back(Fuzzer(iterations, i+1));

	std::vector<std::thread*> threads;
	for(auto& f: fuzzers) {
		auto pThread = new std::thread([&]() { f.Run(); });
		threads.push_back(pThread);
	}

	for(auto pThread: threads) pThread->join();
	for(auto pThread: threads) delete pThread;

	ResultSet rs;
	for(auto& f: fuzzers) rs.Merge(f.results);

	size_t expected = iterations * concurrency;
	size_t sum = rs.Sum();

	cout << "complete:  [ " << sum << " / " << expected << " ] - " << (expected == sum) << endl;
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
