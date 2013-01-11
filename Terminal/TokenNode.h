
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
#ifndef __TOKEN_NODE_H_
#define __TOKEN_NODE_H_

#include <map>
#include <vector>
#include <string>
#include <deque>
#include <assert.h>
#include <sstream>

namespace apl
{
template <class T>
class TokenNode
{
	typedef std::map<std::string, TokenNode<T>*> TokenMap;

public:

	TokenNode(T aValue)
		:
		mHasDefault(true),
		mValue(aValue) {

	}

	TokenNode()
		:
		mHasDefault(false) {

	}

	~TokenNode() {
		for(auto pair: mMap) delete pair.second;
	}

	T* GetValue() {
		return &mValue;
	}


	// returns the correct handler for the tokens
	// and modifies the Tokens vector appropriately for the call
	TokenNode* FindNode(std::vector<std::string>& arTokens) {
		if(arTokens.size() == 0) return this;

		typename TokenMap::iterator i = mMap.find(arTokens[0]);

		if(i == mMap.end()) return this; // this node with arguments

		//strip and argument and call the next node
		arTokens.erase(arTokens.begin());
		return (i->second)->FindNode(arTokens);
	}


	TokenNode* AddToken(const std::string& arToken, T aValue) {
		assert(mMap.find(arToken) == mMap.end());
		TokenNode<T>* pToken = new TokenNode<T>(aValue);
		mMap[arToken] = pToken;
		return pToken;
	}


	void SetValue(T aValue) {
		mHasDefault = true;
		mValue = aValue;
	}

	void GetSubNodeNames(std::vector<std::string>& arNames) {
		for(auto pair: mMap) arNames.push_back(pair.first);
	}

	void GetSubNodesWithOptions(std::vector<std::string>& arNames) {		
		for(auto pair: mMap) {
			std::vector<std::string> subs;
			pair.second->GetSubNodeNames(subs);

			std::ostringstream oss;
			oss << pair.first;
			if ( subs.size() > 0 ) {
				oss << " [";
				bool first = true;
				for(auto c: subs) {				
					if ( !first ) oss << "|";
					first = false;
					oss << c;
				}
				oss << "]";
			}
			arNames.push_back(oss.str());
		}
	}


private:

	bool mHasDefault;

	T mValue;

	TokenMap mMap;


};
}

#endif
