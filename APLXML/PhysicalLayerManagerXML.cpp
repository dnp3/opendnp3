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
#include "PhysicalLayerManagerXML.h"

#include <XMLBindings/APLXML_Base.h>
#include "PhysicalLayerXMLFactory.h"
#include "XMLConversion.h"

#include <APL/Log.h>

namespace apl
{
class EventLog;
}

using namespace APLXML_Base;

namespace apl
{
namespace xml
{


PhysicalLayerManagerXML::PhysicalLayerManagerXML(Logger* apLogger, boost::asio::io_service* apService)  :
	PhysicalLayerManager(apLogger, apService)
{}

PhysicalLayerManagerXML::PhysicalLayerManagerXML(Logger* apLogger, boost::asio::io_service* apService, const APLXML_Base::PhysicalLayerList_t* apList, FilterLevel aLevel) :
	PhysicalLayerManager(apLogger, apService)
{
	this->AddList(apList, aLevel);
}

void PhysicalLayerManagerXML :: AddList(const APLXML_Base::PhysicalLayerList_t* apList, FilterLevel aLevel)
{
	AddStandalones<Serial_t>(apList->SerialVector, aLevel);
	AddStandalones<TCPClient_t>(apList->TCPClientVector, aLevel);
	AddStandalones<TCPServer_t>(apList->TCPServerVector, aLevel);
}

// Created helper function to remove ugly loops.
template<class T>
void PhysicalLayerManagerXML :: AddStandalones(const std::vector<T*>& arVector, FilterLevel aLevel)
{
	for(size_t i = 0; i < arVector.size(); ++i) {
		AddPhysLayer<T>(arVector[i]->Name, aLevel, arVector[i]);
	}
}

template <class T>
void PhysicalLayerManagerXML :: AddPhysLayer(const std::string& aName, FilterLevel aLevel, const T* apConfig)
{
	PhysLayerSettings s(aLevel, apConfig->OpenRetryMS);
	IPhysicalLayerAsyncFactory fac = PhysicalLayerXMLFactory::GetFactoryAsync(apConfig);
	PhysLayerInstance i(fac);
	this->AddLayer(aName, s, i);
}


}
}



