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
#include "tinybinding.h"

#include <APL/Location.h>

#include <vector>

using namespace std;
using namespace apl;


IXMLDataBound::IXMLDataBound(): valid(false)
{

}

IXMLDataBound::~IXMLDataBound()
{

}
bool IXMLDataBound::msExceptOnFailure = true;

const char* IXMLDataBound::GetValue(const char* aNodeName, TiXmlNode* apNode, TiXmlNode* apParent)
{
	if(apNode != NULL) {
		TiXmlElement* node = apNode->ToElement();
		if(node != NULL) {
			return node->GetText();
		}
	}
	std::ostringstream oss;
	oss << "ERROR: Missing node: " << aNodeName << " at " << apParent->Row() << ":" << apParent->Column();
	if(msExceptOnFailure) {
		throw apl::Exception(LOCATION, oss.str());
	}
	else {
		std::cout << oss.str() << std::endl;
		return "";
	}
}

void loadXmlInto(const std::string& arFileName, IXMLDataBound* sps)
{
	TiXmlDocument doc(arFileName);

	if(!doc.LoadFile()) {
		std::ostringstream oss;
		oss << "Load file failed on: " << arFileName << " - " << doc.ErrorDesc() << " at " << doc.ErrorRow() << ":" << doc.ErrorCol();
		throw Exception("loadXmlInto", oss.str(),  IXMLDataBound::ERR_XML_NO_FILE);
	}
	if(!doc.Type() == TiXmlNode::DOCUMENT)
		throw Exception("loadXmlInto", "Document is not of type TiXmlNode::DOCUMENT");

	TiXmlNode* node = doc.FirstChild();
	TiXmlNode* lastChild = doc.LastChild();
	while(node->Type() != TiXmlNode::ELEMENT && node != lastChild) {
		node = node->NextSibling();
	}
	sps->fromXml(node);
};

void WriteXmlToFile(IXMLDataBound* aXml, std::string aFileName)
{
	TiXmlDocument doc;
	aXml->toXml(&doc, true, true);
	if(!doc.SaveFile(aFileName)) throw Exception(LOCATION, "Unspecified error while saving");
}



template <>
void collectedType<string>::toXml(TiXmlNode* pParent, bool /*aCreateNode*/, bool /*aIgnoreValid*/)
{	
	for(auto pString: collection) {		
		TiXmlElement* pEm = new TiXmlElement(name);
		TiXmlText* pText = new TiXmlText(ToString_string(*pString));
		pEm->LinkEndChild(pText);
		pParent->LinkEndChild(pEm);		
	}
};

template <>
void collectedType<string>::fromXml(TiXmlNode* pNode)
{
	TiXmlElement* node = pNode->FirstChildElement(name);
	for( ; node != 0 ; node = node->NextSiblingElement(name)) {
		string* item = new string();
		*item = FromString_string(pNode, GetValue(name.c_str(), node, pNode));
		collection.push_back(item);
	}
};
