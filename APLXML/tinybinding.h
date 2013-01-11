
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
#ifndef _TINY_BINDING_H_
#define _TINY_BINDING_H_

#include <tinyxml/tinyxml.h>

#include <APL/Exception.h>
#include <APL/Uncopyable.h>
#include <APL/Parsing.h>
#include <APL/Location.h>

#include <string>
#include <sstream>
#include <vector>
#include <exception>
using namespace std;

//these are defined in some obscure windows .h file and we need to turn them off.
#undef DATABITS_7
#undef DATABITS_8

#define XML_CHECK(location, check) if(!(check)) throw apl::Exception(location, #check);

class IXMLDataBound
{
public:
	IXMLDataBound();
	virtual ~IXMLDataBound();
	virtual void fromXml(TiXmlNode* pNode) = 0;
	virtual void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid) = 0;
	bool valid;

	static const char* GetValue(const char* aNodeName, TiXmlNode* apNode, TiXmlNode* apParent);

	template <typename T>
	static T FromString_T(TiXmlNode* apParent, const char* aStr, const char* type, T aDefault);

	template <typename T>
	static std::string ToString_T(T aValue);

	static bool msExceptOnFailure;

	inline static int FromString_int(TiXmlNode* apParent, const char* aStr);
	inline static double FromString_double(TiXmlNode* apParent, const char* aStr);
	inline static bool FromString_bool(TiXmlNode* apParent, const char* aStr);
	inline static std::string FromString_string(TiXmlNode* apParent, const char* aStr);
	inline static float FromString_float(TiXmlNode* apParent, const char* aStr);

	inline static std::string ToString_int(int aValue);
	inline static std::string ToString_double(double aValue);
	inline static std::string ToString_bool(bool aValue);
	inline static std::string ToString_string(std::string aValue);
	inline static std::string ToString_float(float aValue);

	enum ErrorCode {
	    ERR_XML_NO_FILE
	};
};

template <typename T>
T IXMLDataBound::FromString_T(TiXmlNode* apParent, const char* aStr, const char* type, T aDefault)
{
	T ret;
	if(aStr != NULL && apl::Parsing::Get(std::string(aStr), ret)) return ret;
	else {
		std::ostringstream oss;
		oss << "ERROR: Couldn't convert ";
		if(aStr == NULL) oss << "NULL";
		else oss << aStr;
		oss << " to " << type << " at " << apParent->Row() << ":" << apParent->Column();
		if(msExceptOnFailure) throw apl::Exception(LOCATION, oss.str());
		else {
			std::cout << oss.str() << std::endl;
			return aDefault;
		}
	}
}

template <typename T>
std::string IXMLDataBound::ToString_T(T aValue)
{
	std::ostringstream oss;
	oss << boolalpha;
	oss << aValue;
	return oss.str();
}

inline int IXMLDataBound::FromString_int(TiXmlNode* apParent, const char* aStr)
{
	return FromString_T<int>(apParent, aStr, "int", 0);
}
inline double IXMLDataBound::FromString_double(TiXmlNode* apParent, const char* aStr)
{
	return FromString_T<double>(apParent, aStr, "double", 0);
}
inline bool IXMLDataBound::FromString_bool(TiXmlNode* apParent, const char* aStr)
{
	return FromString_T<bool>(apParent, aStr, "bool", false);
}
inline std::string IXMLDataBound::FromString_string(TiXmlNode* /*apParent*/, const char* aStr)
{
	if(aStr == NULL || strlen(aStr) == 0) return "";
	return string(aStr);
}
inline float IXMLDataBound::FromString_float(TiXmlNode* apParent, const char* aStr)
{
	return FromString_T<float>(apParent, aStr, "float", 0);
}

inline std::string IXMLDataBound::ToString_int(int aValue)
{
	return ToString_T<int>(aValue);
}
inline std::string IXMLDataBound::ToString_double(double aValue)
{
	return ToString_T<double>(aValue);
}
inline std::string IXMLDataBound::ToString_bool(bool aValue)
{
	return ToString_T<bool>(aValue);
}
inline std::string IXMLDataBound::ToString_string(std::string aValue)
{
	return aValue;
}
inline std::string IXMLDataBound::ToString_float(float aValue)
{
	return ToString_T<float>(aValue);
}


template<typename T>
class collectedType : public IXMLDataBound, private apl::Uncopyable
{
public:
	~collectedType<T>();
	std::vector<T*> collection;
	std::string name;
	collectedType(std::string aS);
	size_t size();
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
};
template<typename T>
collectedType<T> :: ~collectedType()
{
	for (size_t i = 0; i < collection.size(); i++ ) {
		delete collection[i];
	}
}

template <typename T>
collectedType<T>::collectedType(std::string aS)
{
	name = aS;

};

template <typename T>
size_t collectedType<T>::size()
{
	return collection.size();
};

template <typename T>
void collectedType<T>::toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid)
{	
	for(auto pItem: collection) {		
		pItem->toXml(pParent, aCreateNode, aIgnoreValid);		
	}
};

template <typename T>
void collectedType<T>::fromXml(TiXmlNode* pNode)
{
	TiXmlElement* node = pNode->FirstChildElement(name);
	for( ; node != 0 ; node = node->NextSiblingElement(name)) {
		T* item = new T();
		item->fromXml(node);
		collection.push_back(item);
	}
};

template <>
void collectedType<string>::toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);

template <>
void collectedType<string>::fromXml(TiXmlNode* pNode);


void loadXmlInto(const std::string& arFileName, IXMLDataBound* sps);

// throws on error
void WriteXmlToFile(IXMLDataBound* aXml, std::string aFileName);

template <typename T>
T* LoadXML(const std::string& arFileName)
{
	T* pCfg = new T();
	try {
		loadXmlInto(arFileName, pCfg);
		return pCfg;
	}
	catch(apl::Exception ex) {
		std::cout << "Error loading config file: " << std::endl;
		std::cout << ex.GetErrorString() << std::endl;
		throw ex;
	}
}

#endif
