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
#include "ResponseLoaderTestObject.h"

#include "BufferHelpers.h"

#include <opendnp3/APDU.h>
#include <opendnp3/ResponseLoader.h>

#include <boost/test/unit_test.hpp>

#include <memory.h>

using namespace openpal;

namespace opendnp3
{

ResponseLoaderTestObject::ResponseLoaderTestObject() :
	fdo(),
	log(),
	mLogger(Logger(&log, LogLevel::Info, "rsp"))
{}

void ResponseLoaderTestObject::Load(const std::string& arAPDU)
{
	fdo.Clear();
	HexSequence hs(arAPDU);
	APDU f;
	f.Write(hs.ToReadOnly());
	f.Interpret();

	ResponseLoader rl(mLogger, [this](MeasurementUpdate& update){ fdo.Load(update); });
	for(HeaderReadIterator hdr = f.BeginRead(); !hdr.IsEnd(); ++hdr) {
		rl.Process(hdr);
	}
}

void ResponseLoaderTestObject::CheckBinaries(const std::string& arAPDU)
{
	this->Load(arAPDU);

	BOOST_REQUIRE_EQUAL(fdo.mBinaryMap.size(), 3);
	BOOST_REQUIRE_EQUAL(fdo.GetTotalCount(), 3);

	BOOST_REQUIRE(fdo.Check(false, BQ_ONLINE, 1, 0));
	BOOST_REQUIRE(fdo.Check(true, BQ_ONLINE, 2, 0));
	BOOST_REQUIRE(fdo.Check(false, BQ_ONLINE, 3, 0));
}

void ResponseLoaderTestObject::CheckCounters(const std::string& arAPDU)
{
	this->Load(arAPDU);

	BOOST_REQUIRE_EQUAL(fdo.mCounterMap.size(), 2);
	BOOST_REQUIRE_EQUAL(fdo.GetTotalCount(), 2);

	BOOST_REQUIRE(fdo.Check(4, CQ_ONLINE, 0, 0));
	BOOST_REQUIRE(fdo.Check(9, CQ_ONLINE, 1, 0));
}

void ResponseLoaderTestObject::CheckAnalogs(const std::string& arAPDU)
{
	this->Load(arAPDU);

	BOOST_REQUIRE_EQUAL(fdo.mAnalogMap.size(), 2);
	BOOST_REQUIRE_EQUAL(fdo.GetTotalCount(), 2);

	BOOST_REQUIRE(fdo.Check(4, AQ_ONLINE, 0, 0));
	BOOST_REQUIRE(fdo.Check(9, AQ_ONLINE, 1, 0));
}

void ResponseLoaderTestObject::CheckSetpointStatii(const std::string& arAPDU)
{
	this->Load(arAPDU);

	BOOST_REQUIRE_EQUAL(fdo.mSetpointStatusMap.size(), 2);
	BOOST_REQUIRE_EQUAL(fdo.GetTotalCount(), 2);

	BOOST_REQUIRE(fdo.Check(4, PQ_ONLINE, 0));
	BOOST_REQUIRE(fdo.Check(9, PQ_ONLINE, 1));
}

}

