
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "VtoRouterManager.h"

#include "AlwaysOpeningVtoRouter.h"
#include "EnhancedVtoRouter.h"
#include "VtoRouterSettings.h"

#include <APL/Exception.h>
#include <APL/IPhysicalLayerSource.h>
#include <APL/IPhysicalLayerAsync.h>
#include <APL/Logger.h>
#include <APL/Location.h>

#include <memory>


#include <sstream>

namespace apl
{
namespace dnp
{

RouterRecord::RouterRecord(const std::string& arPortName, std::shared_ptr<VtoRouter> apRouter, IVtoWriter* apWriter, uint8_t aVtoChannelId) :
	mPortName(arPortName),
	mpRouter(apRouter),
	mpWriter(apWriter),
	mVtoChannelId(aVtoChannelId)
{

}

VtoRouterManager::VtoRouterManager(Logger* apLogger, IPhysicalLayerSource* apPhysSrc) :
	Loggable(apLogger),	
	mpPhysSource(apPhysSrc)
{	
	assert(apPhysSrc != NULL);
}

VtoRouter* VtoRouterManager::StartRouter(
    const std::string& arPortName,
    const VtoRouterSettings& arSettings,
    IVtoWriter* apWriter)
{
	IPhysicalLayerAsync* pPhys = mpPhysSource->AcquireLayer(arPortName);
	Logger* pLogger = this->GetSubLogger(arPortName, arSettings.CHANNEL_ID);

	std::shared_ptr<VtoRouter> pRouter;
	if(arSettings.DISABLE_EXTENSIONS) {
		pRouter.reset(new AlwaysOpeningVtoRouter(arSettings, pLogger, apWriter, pPhys));
	}
	else {
		if(arSettings.START_LOCAL) {
			pRouter.reset(new ServerSocketVtoRouter(arSettings, pLogger, apWriter, pPhys));
		}
		else {
			pRouter.reset(new ClientSocketVtoRouter(arSettings, pLogger, apWriter, pPhys));
		}
	}

	RouterRecord record(arPortName, pRouter, apWriter, arSettings.CHANNEL_ID);

	this->mRecords.push_back(record);

	return pRouter.get();
}

std::vector<RouterRecord> VtoRouterManager::GetAllRouters()
{
	std::vector<RouterRecord> ret;
	for(size_t i = 0; i < mRecords.size(); ++i) ret.push_back(mRecords[i]);
	return ret;
}

void VtoRouterManager::StopRouter(IVtoWriter* apWriter, uint8_t aVtoChannelId)
{
	VtoRouter* pRouter = this->GetRouterOnWriter(apWriter, aVtoChannelId).mpRouter.get();
	this->StopRouter(pRouter, apWriter);
}

void VtoRouterManager::StopAllRoutersOnWriter(IVtoWriter* apWriter)
{
	RouterRecordVector recs = this->GetAllRoutersOnWriter(apWriter);

	for(auto record: recs) {
		this->StopRouter(record.mpRouter.get(), apWriter);
	}
}

std::vector<RouterRecord> VtoRouterManager::GetAllRoutersOnWriter(IVtoWriter* apWriter)
{
	std::vector< RouterRecord > ret;
	for(auto record: mRecords) {
		if(record.mpWriter == apWriter) ret.push_back(record);
	}
	return ret;
}

RouterRecord VtoRouterManager::GetRouterOnWriter(IVtoWriter* apWriter, uint8_t aVtoChannelId)
{
	for(auto rec: mRecords) {	
		if(rec.mpWriter == apWriter && rec.mVtoChannelId == aVtoChannelId) return rec;
	}

	throw ArgumentException(LOCATION, "Router not found for writer on channel");
}


RouterRecordVector::iterator VtoRouterManager::Find(IVtoWriter* apWriter, uint8_t aVtoChannelId)
{
	RouterRecordVector::iterator i = this->mRecords.begin();

	for(; i != mRecords.end(); ++i) {
		if(i->mpWriter == apWriter && i->mVtoChannelId == aVtoChannelId) return i;
	}

	return i;
}

RouterRecordVector::iterator VtoRouterManager::Find(IVtoWriter* apWriter)
{
	RouterRecordVector::iterator i = this->mRecords.begin();

	for(; i != mRecords.end(); ++i) {
		if(i->mpWriter == apWriter) return i;
	}

	return i;
}

void VtoRouterManager::StopRouter(VtoRouter* apRouter, IVtoWriter* apWriter)
{
	for(RouterRecordVector::iterator i = mRecords.begin(); i != mRecords.end(); ++i) {
		if(i->mpRouter.get() == apRouter) {

			apRouter->GetExecutor()->Synchronize([apWriter, apRouter, i](){
				apWriter->RemoveVtoCallback(apRouter);
				i->mpRouter->Shutdown();
			});

			i->mpRouter->WaitForShutdown();			  // blocking, when it returns we're done for good
			mpPhysSource->ReleaseLayer(i->mPortName); // release the physical layer
			mRecords.erase(i);						  // erasing from the vector will cause the shared_ptr to delete the VtoRouter*
			return;
		}
	}

	throw ArgumentException(LOCATION, "Router could not be found in vector");
}

Logger* VtoRouterManager::GetSubLogger(const std::string& arId, uint8_t aVtoChannelId)
{
	std::ostringstream oss;
	oss << arId << "-VtoRouterChannel-" << ((int)aVtoChannelId);
	return mpLogger->GetSubLogger(oss.str());
}

}
}
