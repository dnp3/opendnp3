/* ******************************
	GENERATED CONTENT DO NOT ALTER!
*********************************
*/
#include "APLXML_STS.h"
using namespace std;
namespace APLXML_STS {
void SlaveTestSet_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("SlaveTestSet",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("SlaveTestSet",pEm != 0);
	LogFile = FromString_string(pEm, pEm->Attribute("LogFile"));
	PhysicalLayer = FromString_string(pEm, pEm->Attribute("PhysicalLayer"));
	Remote = FromString_bool(pEm, pEm->Attribute("Remote"));
	RemotePort = FromString_int(pEm, pEm->Attribute("RemotePort"));
	LinkCommandStatus = FromString_bool(pEm, pEm->Attribute("LinkCommandStatus"));
	StartOnline = FromString_bool(pEm, pEm->Attribute("StartOnline"));
	Log.fromXml(pNode->FirstChildElement("Log"));
	Slave.fromXml(pNode->FirstChildElement("Slave"));
	DeviceTemplate.fromXml(pNode->FirstChildElement("DeviceTemplate"));
	PhysicalLayerList.fromXml(pNode->FirstChildElement("PhysicalLayerList"));
	valid=true;
};
void SlaveTestSet_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("SlaveTestSet");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("LogFile", ToString_string(LogFile));
	pEm->SetAttribute("PhysicalLayer", ToString_string(PhysicalLayer));
	pEm->SetAttribute("Remote", ToString_bool(Remote));
	pEm->SetAttribute("RemotePort", ToString_int(RemotePort));
	pEm->SetAttribute("LinkCommandStatus", ToString_bool(LinkCommandStatus));
	pEm->SetAttribute("StartOnline", ToString_bool(StartOnline));
	Log.toXml(pEm, true, aIgnoreValid);
	Slave.toXml(pEm, true, aIgnoreValid);
	DeviceTemplate.toXml(pEm, true, aIgnoreValid);
	PhysicalLayerList.toXml(pEm, true, aIgnoreValid);
};

}
