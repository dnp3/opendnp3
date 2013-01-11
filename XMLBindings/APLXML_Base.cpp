/* ******************************
	GENERATED CONTENT DO NOT ALTER!
*********************************
*/
#include "APLXML_Base.h"
using namespace std;
namespace APLXML_Base {
BaudRateEnum FromString_BaudRateEnum(TiXmlNode* apParent, const char* aValue) {
	if(aValue != NULL && strcmp(aValue,"1200") == 0) return BAUDRATE_1200;
	if(aValue != NULL && strcmp(aValue,"1800") == 0) return BAUDRATE_1800;
	if(aValue != NULL && strcmp(aValue,"2400") == 0) return BAUDRATE_2400;
	if(aValue != NULL && strcmp(aValue,"4800") == 0) return BAUDRATE_4800;
	if(aValue != NULL && strcmp(aValue,"9600") == 0) return BAUDRATE_9600;
	if(aValue != NULL && strcmp(aValue,"19200") == 0) return BAUDRATE_19200;
	if(aValue != NULL && strcmp(aValue,"38400") == 0) return BAUDRATE_38400;
	if(aValue != NULL && strcmp(aValue,"57600") == 0) return BAUDRATE_57600;
	if(aValue != NULL && strcmp(aValue,"115200") == 0) return BAUDRATE_115200;
	if(aValue != NULL && strcmp(aValue,"230400") == 0) return BAUDRATE_230400;
	std::ostringstream oss;
	oss << "String " << (aValue==NULL?"null":aValue) << " isnt valid for BaudRateEnum at " << apParent->Row() << ":" << apParent->Column();
	if(IXMLDataBound::msExceptOnFailure){throw apl::Exception(LOCATION, oss.str());}
	else{std::cout << oss.str() << std::endl; return BAUDRATE_1200;}
};
string ToString_BaudRateEnum(BaudRateEnum aValue) {
	switch(aValue){
	case BAUDRATE_1200: return "1200";
	case BAUDRATE_1800: return "1800";
	case BAUDRATE_2400: return "2400";
	case BAUDRATE_4800: return "4800";
	case BAUDRATE_9600: return "9600";
	case BAUDRATE_19200: return "19200";
	case BAUDRATE_38400: return "38400";
	case BAUDRATE_57600: return "57600";
	case BAUDRATE_115200: return "115200";
	case BAUDRATE_230400: return "230400";
	}
	std::ostringstream oss;
	oss << "Bad Enum value " << aValue << " for type BaudRateEnum";
	throw apl::Exception(LOCATION, oss.str());
};

DBitsEnum FromString_DBitsEnum(TiXmlNode* apParent, const char* aValue) {
	if(aValue != NULL && strcmp(aValue,"DATABITS_7") == 0) return DATABITS_7;
	if(aValue != NULL && strcmp(aValue,"DATABITS_8") == 0) return DATABITS_8;
	std::ostringstream oss;
	oss << "String " << (aValue==NULL?"null":aValue) << " isnt valid for DBitsEnum at " << apParent->Row() << ":" << apParent->Column();
	if(IXMLDataBound::msExceptOnFailure){throw apl::Exception(LOCATION, oss.str());}
	else{std::cout << oss.str() << std::endl; return DATABITS_7;}
};
string ToString_DBitsEnum(DBitsEnum aValue) {
	switch(aValue){
	case DATABITS_7: return "DATABITS_7";
	case DATABITS_8: return "DATABITS_8";
	}
	std::ostringstream oss;
	oss << "Bad Enum value " << aValue << " for type DBitsEnum";
	throw apl::Exception(LOCATION, oss.str());
};

FlowControlEnum FromString_FlowControlEnum(TiXmlNode* apParent, const char* aValue) {
	if(aValue != NULL && strcmp(aValue,"FLOW_NONE") == 0) return FLOW_NONE;
	if(aValue != NULL && strcmp(aValue,"FLOW_HARDWARE") == 0) return FLOW_HARDWARE;
	if(aValue != NULL && strcmp(aValue,"FLOW_XONXOFF") == 0) return FLOW_XONXOFF;
	std::ostringstream oss;
	oss << "String " << (aValue==NULL?"null":aValue) << " isnt valid for FlowControlEnum at " << apParent->Row() << ":" << apParent->Column();
	if(IXMLDataBound::msExceptOnFailure){throw apl::Exception(LOCATION, oss.str());}
	else{std::cout << oss.str() << std::endl; return FLOW_NONE;}
};
string ToString_FlowControlEnum(FlowControlEnum aValue) {
	switch(aValue){
	case FLOW_NONE: return "FLOW_NONE";
	case FLOW_HARDWARE: return "FLOW_HARDWARE";
	case FLOW_XONXOFF: return "FLOW_XONXOFF";
	}
	std::ostringstream oss;
	oss << "Bad Enum value " << aValue << " for type FlowControlEnum";
	throw apl::Exception(LOCATION, oss.str());
};

LogLevelEnum FromString_LogLevelEnum(TiXmlNode* apParent, const char* aValue) {
	if(aValue != NULL && strcmp(aValue,"LOG_DEBUG") == 0) return LOG_DEBUG;
	if(aValue != NULL && strcmp(aValue,"LOG_COMM") == 0) return LOG_COMM;
	if(aValue != NULL && strcmp(aValue,"LOG_INTERPRET") == 0) return LOG_INTERPRET;
	if(aValue != NULL && strcmp(aValue,"LOG_INFO") == 0) return LOG_INFO;
	if(aValue != NULL && strcmp(aValue,"LOG_WARNING") == 0) return LOG_WARNING;
	if(aValue != NULL && strcmp(aValue,"LOG_ERROR") == 0) return LOG_ERROR;
	if(aValue != NULL && strcmp(aValue,"LOG_EVENT") == 0) return LOG_EVENT;
	std::ostringstream oss;
	oss << "String " << (aValue==NULL?"null":aValue) << " isnt valid for LogLevelEnum at " << apParent->Row() << ":" << apParent->Column();
	if(IXMLDataBound::msExceptOnFailure){throw apl::Exception(LOCATION, oss.str());}
	else{std::cout << oss.str() << std::endl; return LOG_DEBUG;}
};
string ToString_LogLevelEnum(LogLevelEnum aValue) {
	switch(aValue){
	case LOG_DEBUG: return "LOG_DEBUG";
	case LOG_COMM: return "LOG_COMM";
	case LOG_INTERPRET: return "LOG_INTERPRET";
	case LOG_INFO: return "LOG_INFO";
	case LOG_WARNING: return "LOG_WARNING";
	case LOG_ERROR: return "LOG_ERROR";
	case LOG_EVENT: return "LOG_EVENT";
	}
	std::ostringstream oss;
	oss << "Bad Enum value " << aValue << " for type LogLevelEnum";
	throw apl::Exception(LOCATION, oss.str());
};

ParityEnum FromString_ParityEnum(TiXmlNode* apParent, const char* aValue) {
	if(aValue != NULL && strcmp(aValue,"PARITY_NONE") == 0) return PARITY_NONE;
	if(aValue != NULL && strcmp(aValue,"PARITY_EVEN") == 0) return PARITY_EVEN;
	if(aValue != NULL && strcmp(aValue,"PARITY_ODD") == 0) return PARITY_ODD;
	std::ostringstream oss;
	oss << "String " << (aValue==NULL?"null":aValue) << " isnt valid for ParityEnum at " << apParent->Row() << ":" << apParent->Column();
	if(IXMLDataBound::msExceptOnFailure){throw apl::Exception(LOCATION, oss.str());}
	else{std::cout << oss.str() << std::endl; return PARITY_NONE;}
};
string ToString_ParityEnum(ParityEnum aValue) {
	switch(aValue){
	case PARITY_NONE: return "PARITY_NONE";
	case PARITY_EVEN: return "PARITY_EVEN";
	case PARITY_ODD: return "PARITY_ODD";
	}
	std::ostringstream oss;
	oss << "Bad Enum value " << aValue << " for type ParityEnum";
	throw apl::Exception(LOCATION, oss.str());
};

void PhysicalLayerDescriptor_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("PhysicalLayerDescriptor",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("PhysicalLayerDescriptor",pEm != 0);
	Name = FromString_string(pEm, pEm->Attribute("Name"));
	OpenRetryMS = FromString_int(pEm, pEm->Attribute("OpenRetryMS"));
	valid=true;
};
void PhysicalLayerDescriptor_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("PhysicalLayerDescriptor");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Name", ToString_string(Name));
	pEm->SetAttribute("OpenRetryMS", ToString_int(OpenRetryMS));
};

StopBitsEnum FromString_StopBitsEnum(TiXmlNode* apParent, const char* aValue) {
	if(aValue != NULL && strcmp(aValue,"STOPBITS_0") == 0) return STOPBITS_0;
	if(aValue != NULL && strcmp(aValue,"STOPBITS_1") == 0) return STOPBITS_1;
	if(aValue != NULL && strcmp(aValue,"STOPBITS_2") == 0) return STOPBITS_2;
	std::ostringstream oss;
	oss << "String " << (aValue==NULL?"null":aValue) << " isnt valid for StopBitsEnum at " << apParent->Row() << ":" << apParent->Column();
	if(IXMLDataBound::msExceptOnFailure){throw apl::Exception(LOCATION, oss.str());}
	else{std::cout << oss.str() << std::endl; return STOPBITS_0;}
};
string ToString_StopBitsEnum(StopBitsEnum aValue) {
	switch(aValue){
	case STOPBITS_0: return "STOPBITS_0";
	case STOPBITS_1: return "STOPBITS_1";
	case STOPBITS_2: return "STOPBITS_2";
	}
	std::ostringstream oss;
	oss << "Bad Enum value " << aValue << " for type StopBitsEnum";
	throw apl::Exception(LOCATION, oss.str());
};

void TCPClient_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("TCPClient",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("TCPClient",pEm != 0);
	this->APLXML_Base::PhysicalLayerDescriptor_t::fromXml(pNode);
	Address = FromString_string(pEm, pEm->Attribute("Address"));
	Port = FromString_int(pEm, pEm->Attribute("Port"));
	valid=true;
};
void TCPClient_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("TCPClient");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	this->APLXML_Base::PhysicalLayerDescriptor_t::toXml(pEm, false, aIgnoreValid);
	pEm->SetAttribute("Address", ToString_string(Address));
	pEm->SetAttribute("Port", ToString_int(Port));
};

void TCPServer_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("TCPServer",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("TCPServer",pEm != 0);
	this->APLXML_Base::PhysicalLayerDescriptor_t::fromXml(pNode);
	Endpoint = FromString_string(pEm, pEm->Attribute("Endpoint"));
	Port = FromString_int(pEm, pEm->Attribute("Port"));
	valid=true;
};
void TCPServer_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("TCPServer");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	this->APLXML_Base::PhysicalLayerDescriptor_t::toXml(pEm, false, aIgnoreValid);
	pEm->SetAttribute("Endpoint", ToString_string(Endpoint));
	pEm->SetAttribute("Port", ToString_int(Port));
};

void Log_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Log",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Log",pEm != 0);
	Filter = FromString_LogLevelEnum(pEm, pEm->Attribute("Filter"));
	valid=true;
};
void Log_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Log");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Filter", ToString_LogLevelEnum(Filter));
};

void Serial_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Serial",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Serial",pEm != 0);
	this->APLXML_Base::PhysicalLayerDescriptor_t::fromXml(pNode);
	Device = FromString_string(pEm, pEm->Attribute("Device"));
	BaudRate = FromString_BaudRateEnum(pEm, pEm->Attribute("BaudRate"));
	Parity = FromString_ParityEnum(pEm, pEm->Attribute("Parity"));
	DBits = FromString_DBitsEnum(pEm, pEm->Attribute("DBits"));
	StopBits = FromString_StopBitsEnum(pEm, pEm->Attribute("StopBits"));
	FlowControl = FromString_FlowControlEnum(pEm, pEm->Attribute("FlowControl"));
	valid=true;
};
void Serial_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Serial");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	this->APLXML_Base::PhysicalLayerDescriptor_t::toXml(pEm, false, aIgnoreValid);
	pEm->SetAttribute("Device", ToString_string(Device));
	pEm->SetAttribute("BaudRate", ToString_BaudRateEnum(BaudRate));
	pEm->SetAttribute("Parity", ToString_ParityEnum(Parity));
	pEm->SetAttribute("DBits", ToString_DBitsEnum(DBits));
	pEm->SetAttribute("StopBits", ToString_StopBitsEnum(StopBits));
	pEm->SetAttribute("FlowControl", ToString_FlowControlEnum(FlowControl));
};

PhysicalLayerList_t::PhysicalLayerList_t():
		TCPServer("TCPServer"), TCPServerVector(TCPServer.collection),
		TCPClient("TCPClient"), TCPClientVector(TCPClient.collection),
		Serial("Serial"), SerialVector(Serial.collection){};
void PhysicalLayerList_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("PhysicalLayerList",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("PhysicalLayerList",pEm != 0);
	TCPServer.fromXml(pNode);
	TCPClient.fromXml(pNode);
	Serial.fromXml(pNode);
	valid=true;
};
void PhysicalLayerList_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(TCPServer.size() == 0 && TCPClient.size() == 0 && Serial.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("PhysicalLayerList");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	TCPServer.toXml(pEm, true, aIgnoreValid);
	TCPClient.toXml(pEm, true, aIgnoreValid);
	Serial.toXml(pEm, true, aIgnoreValid);
};

}
