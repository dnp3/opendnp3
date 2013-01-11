
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
#ifndef _SINGLE_NODE_SAVER_H_
#define _SINGLE_NODE_SAVER_H_

#include "INodeSaver.h"
#include "tinybinding.h"

#include <APL/Loggable.h>
#include <APL/LoggableMacros.h>

namespace apl
{

/** Implementation of INodeSaver that takes an XML Node and
saves or loads it to a file.
*/
template <class RootNode>
class SingleNodeSaver : public INodeSaver<RootNode>, public Loggable
{
public:
	SingleNodeSaver(std::string aFileName, Logger* apLogger)
		: Loggable(apLogger), mFileName(aFileName)
	{	}

protected:
	bool _LoadNode(RootNode&);
	bool _SaveNode(RootNode&);

private:
	std::string mFileName;
};

template <class RootNode>
bool SingleNodeSaver<RootNode> :: _LoadNode(RootNode& aeNode)
{
	try {
		loadXmlInto(mFileName, &aeNode);
		return true;
	}
	catch(Exception& e) {
		//treat file not found as a warning (its expect on a clean boot)
		FilterLevel level = e.ErrorCode() == IXMLDataBound::ERR_XML_NO_FILE ? LEV_WARNING : LEV_ERROR;
		LOG_BLOCK(level, "Error Loading XML: " << e.what());
		return false;
	}
}

template <class RootNode>
bool SingleNodeSaver<RootNode> :: _SaveNode(RootNode& aeNode)
{
	try {
		TiXmlDocument doc2;
		aeNode.toXml(&doc2, true, true);
		return doc2.SaveFile(mFileName);
	}
	catch(Exception& e) {
		LOG_BLOCK(LEV_ERROR, "Error Saving XML: " << e.what());
		return false;
	}
}
}

#endif
