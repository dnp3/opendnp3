/* ******************************
	GENERATED CONTENT DO NOT ALTER!
*********************************
*/
#include "APLXML_MTS.h"
using namespace std;
namespace APLXML_MTS {
void MasterTestSet_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("MasterTestSet",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("MasterTestSet",pEm != 0);
	PhysicalLayer = FromString_string(pEm, pEm->Attribute("PhysicalLayer"));
	LogFile = FromString_string(pEm, pEm->Attribute("LogFile"));
	Log.fromXml(pNode->FirstChildElement("Log"));
	Master.fromXml(pNode->FirstChildElement("Master"));
	PhysicalLayerList.fromXml(pNode->FirstChildElement("PhysicalLayerList"));
	valid=true;
};
void MasterTestSet_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("MasterTestSet");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("PhysicalLayer", ToString_string(PhysicalLayer));
	pEm->SetAttribute("LogFile", ToString_string(LogFile));
	Log.toXml(pEm, true, aIgnoreValid);
	Master.toXml(pEm, true, aIgnoreValid);
	PhysicalLayerList.toXml(pEm, true, aIgnoreValid);
};

}
