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
#include "XML_TestSet.h"

#include <APLXML/XML_APL.h>
#include <DNP3XML/XML_DNP3.h>

#include <XMLBindings/APLXML_Base.h>
#include <XMLBindings/APLXML_DNP.h>


#include <XMLBindings/APLXML_MTS.h>
#include <XMLBindings/APLXML_STS.h>

using namespace apl::dnp;

namespace apl
{

void XML_TestSet::Configure(APLXML_MTS::MasterTestSet_t& arMasterTest, bool aUseUnsol)
{
	arMasterTest.PhysicalLayer = "tcpclient";
	arMasterTest.LogFile = "master_testset.log";
	arMasterTest.Log.Filter = APLXML_Base::LOG_WARNING;
	XML_DNP3::Configure(arMasterTest.Master, aUseUnsol);

	xml::XML_APL::AddSerial(arMasterTest.PhysicalLayerList, "serial", "COM1");
	xml::XML_APL::AddTCPClient(arMasterTest.PhysicalLayerList, "tcpclient", "127.0.0.1", 20000);

	xml::XML_APL::AddTCPServer(arMasterTest.PhysicalLayerList, "vtotunnel", "0.0.0.0", 20001);

	XML_DNP3::AddVtoPort(arMasterTest.Master.VtoPorts, "vtotunnel", 0, true);
}

void XML_TestSet::Configure(APLXML_STS::SlaveTestSet_t& arSlaveTest)
{
	arSlaveTest.PhysicalLayer = "tcpserver";
	arSlaveTest.LogFile = "slave_testset.log";
	arSlaveTest.Log.Filter = APLXML_Base::LOG_WARNING;
	XML_DNP3::Configure(arSlaveTest.Slave);

	xml::XML_APL::AddSerial(arSlaveTest.PhysicalLayerList, "serial", "COM1");
	xml::XML_APL::AddTCPServer(arSlaveTest.PhysicalLayerList, "tcpserver", "0.0.0.0", 20000);

	xml::XML_APL::AddTCPClient(arSlaveTest.PhysicalLayerList, "vtotunnel", "127.0.0.1", 22);

	XML_DNP3::AddVtoPort(arSlaveTest.Slave.VtoPorts, "vtotunnel", 0, false);

	arSlaveTest.Remote = false;
	arSlaveTest.RemotePort = 4999;
	arSlaveTest.LinkCommandStatus = false;
	arSlaveTest.StartOnline = false;
}


}
