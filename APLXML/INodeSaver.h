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


#ifndef _XML_SAVE_FILE_H_
#define _XML_SAVE_FILE_H_

namespace apl
{

template <class RootNode>
class INodeSaver
{


public:
	virtual ~INodeSaver() {}

	bool LoadNode(RootNode&);
	bool SaveNode(RootNode&);

protected:
	virtual bool _LoadNode(RootNode&) = 0;
	virtual bool _SaveNode(RootNode&) = 0;

};
template <class RootNode>
bool INodeSaver<RootNode>:: LoadNode(RootNode& aNode)
{
	return _LoadNode(aNode);
}
template <class RootNode>
bool INodeSaver<RootNode> :: SaveNode(RootNode& aNode)
{
	return _SaveNode(aNode);
}


};

#endif
