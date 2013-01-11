/* ******************************
	GENERATED CONTENT DO NOT ALTER!
*********************************
*/
#include "APLXML_DNP.h"
using namespace std;
namespace APLXML_DNP {
void Analog_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Analog",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Analog",pEm != 0);
	Index = FromString_int(pEm, pEm->Attribute("Index"));
	Name = FromString_string(pEm, pEm->Attribute("Name"));
	ClassGroup = FromString_int(pEm, pEm->Attribute("ClassGroup"));
	Deadband = FromString_int(pEm, pEm->Attribute("Deadband"));
	valid=true;
};
void Analog_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Analog");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Index", ToString_int(Index));
	pEm->SetAttribute("Name", ToString_string(Name));
	pEm->SetAttribute("ClassGroup", ToString_int(ClassGroup));
	pEm->SetAttribute("Deadband", ToString_int(Deadband));
};

AnalogData_t::AnalogData_t():
		Analog("Analog"), AnalogVector(Analog.collection){};
void AnalogData_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("AnalogData",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("AnalogData",pEm != 0);
	Analog.fromXml(pNode);
	valid=true;
};
void AnalogData_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(Analog.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("AnalogData");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	Analog.toXml(pEm, true, aIgnoreValid);
};

void AppLayer_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("AppLayer",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("AppLayer",pEm != 0);
	TimeoutMS = FromString_int(pEm, pEm->Attribute("TimeoutMS"));
	MaxFragSize = FromString_int(pEm, pEm->Attribute("MaxFragSize"));
	NumRetries = FromString_int(pEm, pEm->Attribute("NumRetries"));
	valid=true;
};
void AppLayer_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("AppLayer");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("TimeoutMS", ToString_int(TimeoutMS));
	pEm->SetAttribute("MaxFragSize", ToString_int(MaxFragSize));
	pEm->SetAttribute("NumRetries", ToString_int(NumRetries));
};

void Binary_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Binary",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Binary",pEm != 0);
	Index = FromString_int(pEm, pEm->Attribute("Index"));
	Name = FromString_string(pEm, pEm->Attribute("Name"));
	ClassGroup = FromString_int(pEm, pEm->Attribute("ClassGroup"));
	valid=true;
};
void Binary_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Binary");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Index", ToString_int(Index));
	pEm->SetAttribute("Name", ToString_string(Name));
	pEm->SetAttribute("ClassGroup", ToString_int(ClassGroup));
};

BinaryData_t::BinaryData_t():
		Binary("Binary"), BinaryVector(Binary.collection){};
void BinaryData_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("BinaryData",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("BinaryData",pEm != 0);
	Binary.fromXml(pNode);
	valid=true;
};
void BinaryData_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(Binary.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("BinaryData");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	Binary.toXml(pEm, true, aIgnoreValid);
};

void Control_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Control",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Control",pEm != 0);
	Index = FromString_int(pEm, pEm->Attribute("Index"));
	Name = FromString_string(pEm, pEm->Attribute("Name"));
	ControlMode = FromString_string(pEm, pEm->Attribute("ControlMode"));
	SelectTimeoutMS = FromString_int(pEm, pEm->Attribute("SelectTimeoutMS"));
	valid=true;
};
void Control_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Control");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Index", ToString_int(Index));
	pEm->SetAttribute("Name", ToString_string(Name));
	pEm->SetAttribute("ControlMode", ToString_string(ControlMode));
	pEm->SetAttribute("SelectTimeoutMS", ToString_int(SelectTimeoutMS));
};

ControlData_t::ControlData_t():
		Control("Control"), ControlVector(Control.collection){};
void ControlData_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("ControlData",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("ControlData",pEm != 0);
	Control.fromXml(pNode);
	valid=true;
};
void ControlData_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(Control.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("ControlData");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	Control.toXml(pEm, true, aIgnoreValid);
};

void ControlStatus_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("ControlStatus",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("ControlStatus",pEm != 0);
	Index = FromString_int(pEm, pEm->Attribute("Index"));
	Name = FromString_string(pEm, pEm->Attribute("Name"));
	valid=true;
};
void ControlStatus_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("ControlStatus");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Index", ToString_int(Index));
	pEm->SetAttribute("Name", ToString_string(Name));
};

ControlStatusData_t::ControlStatusData_t():
		ControlStatus("ControlStatus"), ControlStatusVector(ControlStatus.collection){};
void ControlStatusData_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("ControlStatusData",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("ControlStatusData",pEm != 0);
	ControlStatus.fromXml(pNode);
	valid=true;
};
void ControlStatusData_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(ControlStatus.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("ControlStatusData");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	ControlStatus.toXml(pEm, true, aIgnoreValid);
};

void Counter_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Counter",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Counter",pEm != 0);
	Index = FromString_int(pEm, pEm->Attribute("Index"));
	ClassGroup = FromString_int(pEm, pEm->Attribute("ClassGroup"));
	Name = FromString_string(pEm, pEm->Attribute("Name"));
	valid=true;
};
void Counter_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Counter");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Index", ToString_int(Index));
	pEm->SetAttribute("ClassGroup", ToString_int(ClassGroup));
	pEm->SetAttribute("Name", ToString_string(Name));
};

CounterData_t::CounterData_t():
		Counter("Counter"), CounterVector(Counter.collection){};
void CounterData_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("CounterData",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("CounterData",pEm != 0);
	Counter.fromXml(pNode);
	valid=true;
};
void CounterData_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(Counter.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("CounterData");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	Counter.toXml(pEm, true, aIgnoreValid);
};

void ExceptionScan_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("ExceptionScan",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("ExceptionScan",pEm != 0);
	Class1 = FromString_bool(pEm, pEm->Attribute("Class1"));
	Class2 = FromString_bool(pEm, pEm->Attribute("Class2"));
	Class3 = FromString_bool(pEm, pEm->Attribute("Class3"));
	PeriodMS = FromString_int(pEm, pEm->Attribute("PeriodMS"));
	valid=true;
};
void ExceptionScan_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("ExceptionScan");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Class1", ToString_bool(Class1));
	pEm->SetAttribute("Class2", ToString_bool(Class2));
	pEm->SetAttribute("Class3", ToString_bool(Class3));
	pEm->SetAttribute("PeriodMS", ToString_int(PeriodMS));
};

void GrpVar_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("GrpVar",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("GrpVar",pEm != 0);
	Grp = FromString_int(pEm, pEm->Attribute("Grp"));
	Var = FromString_int(pEm, pEm->Attribute("Var"));
	valid=true;
};
void GrpVar_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("GrpVar");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Grp", ToString_int(Grp));
	pEm->SetAttribute("Var", ToString_int(Var));
};

void LinkLayer_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("LinkLayer",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("LinkLayer",pEm != 0);
	IsMaster = FromString_bool(pEm, pEm->Attribute("IsMaster"));
	LocalAddress = FromString_int(pEm, pEm->Attribute("LocalAddress"));
	RemoteAddress = FromString_int(pEm, pEm->Attribute("RemoteAddress"));
	UseConfirmations = FromString_bool(pEm, pEm->Attribute("UseConfirmations"));
	AckTimeoutMS = FromString_int(pEm, pEm->Attribute("AckTimeoutMS"));
	NumRetries = FromString_int(pEm, pEm->Attribute("NumRetries"));
	valid=true;
};
void LinkLayer_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("LinkLayer");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("IsMaster", ToString_bool(IsMaster));
	pEm->SetAttribute("LocalAddress", ToString_int(LocalAddress));
	pEm->SetAttribute("RemoteAddress", ToString_int(RemoteAddress));
	pEm->SetAttribute("UseConfirmations", ToString_bool(UseConfirmations));
	pEm->SetAttribute("AckTimeoutMS", ToString_int(AckTimeoutMS));
	pEm->SetAttribute("NumRetries", ToString_int(NumRetries));
};

void MasterSettings_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("MasterSettings",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("MasterSettings",pEm != 0);
	AllowTimeSync = FromString_bool(pEm, pEm->Attribute("AllowTimeSync"));
	TaskRetryMS = FromString_int(pEm, pEm->Attribute("TaskRetryMS"));
	IntegrityPeriodMS = FromString_int(pEm, pEm->Attribute("IntegrityPeriodMS"));
	UseNonStandardVtoTransferCode = FromString_bool(pEm, pEm->Attribute("UseNonStandardVtoTransferCode"));
	valid=true;
};
void MasterSettings_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("MasterSettings");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("AllowTimeSync", ToString_bool(AllowTimeSync));
	pEm->SetAttribute("TaskRetryMS", ToString_int(TaskRetryMS));
	pEm->SetAttribute("IntegrityPeriodMS", ToString_int(IntegrityPeriodMS));
	pEm->SetAttribute("UseNonStandardVtoTransferCode", ToString_bool(UseNonStandardVtoTransferCode));
};

ScanList_t::ScanList_t():
		ExceptionScan("ExceptionScan"), ExceptionScanVector(ExceptionScan.collection){};
void ScanList_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("ScanList",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("ScanList",pEm != 0);
	ExceptionScan.fromXml(pNode);
	valid=true;
};
void ScanList_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(ExceptionScan.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("ScanList");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	ExceptionScan.toXml(pEm, true, aIgnoreValid);
};

void Setpoint_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Setpoint",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Setpoint",pEm != 0);
	Index = FromString_int(pEm, pEm->Attribute("Index"));
	Name = FromString_string(pEm, pEm->Attribute("Name"));
	ControlMode = FromString_string(pEm, pEm->Attribute("ControlMode"));
	SelectTimeoutMS = FromString_int(pEm, pEm->Attribute("SelectTimeoutMS"));
	valid=true;
};
void Setpoint_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Setpoint");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Index", ToString_int(Index));
	pEm->SetAttribute("Name", ToString_string(Name));
	pEm->SetAttribute("ControlMode", ToString_string(ControlMode));
	pEm->SetAttribute("SelectTimeoutMS", ToString_int(SelectTimeoutMS));
};

SetpointData_t::SetpointData_t():
		Setpoint("Setpoint"), SetpointVector(Setpoint.collection){};
void SetpointData_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("SetpointData",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("SetpointData",pEm != 0);
	Setpoint.fromXml(pNode);
	valid=true;
};
void SetpointData_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(Setpoint.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("SetpointData");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	Setpoint.toXml(pEm, true, aIgnoreValid);
};

void SetpointStatus_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("SetpointStatus",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("SetpointStatus",pEm != 0);
	Index = FromString_int(pEm, pEm->Attribute("Index"));
	Name = FromString_string(pEm, pEm->Attribute("Name"));
	valid=true;
};
void SetpointStatus_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("SetpointStatus");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("Index", ToString_int(Index));
	pEm->SetAttribute("Name", ToString_string(Name));
};

SetpointStatusData_t::SetpointStatusData_t():
		SetpointStatus("SetpointStatus"), SetpointStatusVector(SetpointStatus.collection){};
void SetpointStatusData_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("SetpointStatusData",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("SetpointStatusData",pEm != 0);
	SetpointStatus.fromXml(pNode);
	valid=true;
};
void SetpointStatusData_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(SetpointStatus.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("SetpointStatusData");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	SetpointStatus.toXml(pEm, true, aIgnoreValid);
};

void SetpointStatusGrpVar_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("SetpointStatusGrpVar",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("SetpointStatusGrpVar",pEm != 0);
	this->APLXML_DNP::GrpVar_t::fromXml(pNode);
	valid=true;
};
void SetpointStatusGrpVar_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("SetpointStatusGrpVar");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	this->APLXML_DNP::GrpVar_t::toXml(pEm, false, aIgnoreValid);
};

void Stack_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Stack",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Stack",pEm != 0);
	LinkLayer.fromXml(pNode->FirstChildElement("LinkLayer"));
	AppLayer.fromXml(pNode->FirstChildElement("AppLayer"));
	valid=true;
};
void Stack_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Stack");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	LinkLayer.toXml(pEm, true, aIgnoreValid);
	AppLayer.toXml(pEm, true, aIgnoreValid);
};

void TimeIINTask_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("TimeIINTask",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("TimeIINTask",pEm != 0);
	DoTask = FromString_bool(pEm, pEm->Attribute("DoTask"));
	PeriodMS = FromString_int(pEm, pEm->Attribute("PeriodMS"));
	valid=true;
};
void TimeIINTask_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("TimeIINTask");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("DoTask", ToString_bool(DoTask));
	pEm->SetAttribute("PeriodMS", ToString_int(PeriodMS));
};

void Unsol_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Unsol",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Unsol",pEm != 0);
	DoTask = FromString_bool(pEm, pEm->Attribute("DoTask"));
	Enable = FromString_bool(pEm, pEm->Attribute("Enable"));
	Class1 = FromString_bool(pEm, pEm->Attribute("Class1"));
	Class2 = FromString_bool(pEm, pEm->Attribute("Class2"));
	Class3 = FromString_bool(pEm, pEm->Attribute("Class3"));
	valid=true;
};
void Unsol_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Unsol");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("DoTask", ToString_bool(DoTask));
	pEm->SetAttribute("Enable", ToString_bool(Enable));
	pEm->SetAttribute("Class1", ToString_bool(Class1));
	pEm->SetAttribute("Class2", ToString_bool(Class2));
	pEm->SetAttribute("Class3", ToString_bool(Class3));
};

void UnsolDefaults_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("UnsolDefaults",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("UnsolDefaults",pEm != 0);
	DoClass1 = FromString_bool(pEm, pEm->Attribute("DoClass1"));
	DoClass2 = FromString_bool(pEm, pEm->Attribute("DoClass2"));
	DoClass3 = FromString_bool(pEm, pEm->Attribute("DoClass3"));
	PackDelayMS = FromString_int(pEm, pEm->Attribute("PackDelayMS"));
	RetryMS = FromString_int(pEm, pEm->Attribute("RetryMS"));
	valid=true;
};
void UnsolDefaults_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("UnsolDefaults");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("DoClass1", ToString_bool(DoClass1));
	pEm->SetAttribute("DoClass2", ToString_bool(DoClass2));
	pEm->SetAttribute("DoClass3", ToString_bool(DoClass3));
	pEm->SetAttribute("PackDelayMS", ToString_int(PackDelayMS));
	pEm->SetAttribute("RetryMS", ToString_int(RetryMS));
};

void VtoPort_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("VtoPort",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("VtoPort",pEm != 0);
	PhysicalLayer = FromString_string(pEm, pEm->Attribute("PhysicalLayer"));
	Index = FromString_int(pEm, pEm->Attribute("Index"));
	StartLocal = FromString_bool(pEm, pEm->Attribute("StartLocal"));
	OpenRetry = FromString_int(pEm, pEm->Attribute("OpenRetry"));
	valid=true;
};
void VtoPort_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("VtoPort");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("PhysicalLayer", ToString_string(PhysicalLayer));
	pEm->SetAttribute("Index", ToString_int(Index));
	pEm->SetAttribute("StartLocal", ToString_bool(StartLocal));
	pEm->SetAttribute("OpenRetry", ToString_int(OpenRetry));
};

VtoPorts_t::VtoPorts_t():
		VtoPort("VtoPort"), VtoPortVector(VtoPort.collection){};
void VtoPorts_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("VtoPorts",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("VtoPorts",pEm != 0);
	VtoPort.fromXml(pNode);
	valid=true;
};
void VtoPorts_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(VtoPort.size() == 0)return;
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("VtoPorts");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	VtoPort.toXml(pEm, true, aIgnoreValid);
};

void AnalogGrpVar_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("AnalogGrpVar",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("AnalogGrpVar",pEm != 0);
	this->APLXML_DNP::GrpVar_t::fromXml(pNode);
	valid=true;
};
void AnalogGrpVar_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("AnalogGrpVar");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	this->APLXML_DNP::GrpVar_t::toXml(pEm, false, aIgnoreValid);
};

void BinaryGrpVar_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("BinaryGrpVar",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("BinaryGrpVar",pEm != 0);
	this->APLXML_DNP::GrpVar_t::fromXml(pNode);
	valid=true;
};
void BinaryGrpVar_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("BinaryGrpVar");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	this->APLXML_DNP::GrpVar_t::toXml(pEm, false, aIgnoreValid);
};

void CounterGrpVar_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("CounterGrpVar",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("CounterGrpVar",pEm != 0);
	this->APLXML_DNP::GrpVar_t::fromXml(pNode);
	valid=true;
};
void CounterGrpVar_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("CounterGrpVar");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	this->APLXML_DNP::GrpVar_t::toXml(pEm, false, aIgnoreValid);
};

void DeviceTemplate_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("DeviceTemplate",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("DeviceTemplate",pEm != 0);
	TemplateName = FromString_string(pEm, pEm->Attribute("TemplateName"));
	BinaryData.fromXml(pNode->FirstChildElement("BinaryData"));
	AnalogData.fromXml(pNode->FirstChildElement("AnalogData"));
	CounterData.fromXml(pNode->FirstChildElement("CounterData"));
	SetpointStatusData.fromXml(pNode->FirstChildElement("SetpointStatusData"));
	ControlStatusData.fromXml(pNode->FirstChildElement("ControlStatusData"));
	SetpointData.fromXml(pNode->FirstChildElement("SetpointData"));
	ControlData.fromXml(pNode->FirstChildElement("ControlData"));
	valid=true;
};
void DeviceTemplate_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("DeviceTemplate");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("TemplateName", ToString_string(TemplateName));
	BinaryData.toXml(pEm, true, aIgnoreValid);
	AnalogData.toXml(pEm, true, aIgnoreValid);
	CounterData.toXml(pEm, true, aIgnoreValid);
	SetpointStatusData.toXml(pEm, true, aIgnoreValid);
	ControlStatusData.toXml(pEm, true, aIgnoreValid);
	SetpointData.toXml(pEm, true, aIgnoreValid);
	ControlData.toXml(pEm, true, aIgnoreValid);
};

void EventRsp_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("EventRsp",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("EventRsp",pEm != 0);
	BinaryGrpVar.fromXml(pNode->FirstChildElement("BinaryGrpVar"));
	AnalogGrpVar.fromXml(pNode->FirstChildElement("AnalogGrpVar"));
	CounterGrpVar.fromXml(pNode->FirstChildElement("CounterGrpVar"));
	valid=true;
};
void EventRsp_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("EventRsp");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	BinaryGrpVar.toXml(pEm, true, aIgnoreValid);
	AnalogGrpVar.toXml(pEm, true, aIgnoreValid);
	CounterGrpVar.toXml(pEm, true, aIgnoreValid);
};

void Master_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Master",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Master",pEm != 0);
	Stack.fromXml(pNode->FirstChildElement("Stack"));
	MasterSettings.fromXml(pNode->FirstChildElement("MasterSettings"));
	ScanList.fromXml(pNode->FirstChildElement("ScanList"));
	Unsol.fromXml(pNode->FirstChildElement("Unsol"));
	VtoPorts.fromXml(pNode->FirstChildElement("VtoPorts"));
	valid=true;
};
void Master_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Master");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	Stack.toXml(pEm, true, aIgnoreValid);
	MasterSettings.toXml(pEm, true, aIgnoreValid);
	ScanList.toXml(pEm, true, aIgnoreValid);
	Unsol.toXml(pEm, true, aIgnoreValid);
	VtoPorts.toXml(pEm, true, aIgnoreValid);
};

void StaticRsp_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("StaticRsp",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("StaticRsp",pEm != 0);
	BinaryGrpVar.fromXml(pNode->FirstChildElement("BinaryGrpVar"));
	AnalogGrpVar.fromXml(pNode->FirstChildElement("AnalogGrpVar"));
	CounterGrpVar.fromXml(pNode->FirstChildElement("CounterGrpVar"));
	SetpointStatusGrpVar.fromXml(pNode->FirstChildElement("SetpointStatusGrpVar"));
	valid=true;
};
void StaticRsp_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("StaticRsp");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	BinaryGrpVar.toXml(pEm, true, aIgnoreValid);
	AnalogGrpVar.toXml(pEm, true, aIgnoreValid);
	CounterGrpVar.toXml(pEm, true, aIgnoreValid);
	SetpointStatusGrpVar.toXml(pEm, true, aIgnoreValid);
};

void SlaveConfig_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("SlaveConfig",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("SlaveConfig",pEm != 0);
	MaxBinaryEvents = FromString_int(pEm, pEm->Attribute("MaxBinaryEvents"));
	MaxAnalogEvents = FromString_int(pEm, pEm->Attribute("MaxAnalogEvents"));
	MaxCounterEvents = FromString_int(pEm, pEm->Attribute("MaxCounterEvents"));
	StaticRsp.fromXml(pNode->FirstChildElement("StaticRsp"));
	EventRsp.fromXml(pNode->FirstChildElement("EventRsp"));
	TimeIINTask.fromXml(pNode->FirstChildElement("TimeIINTask"));
	UnsolDefaults.fromXml(pNode->FirstChildElement("UnsolDefaults"));
	valid=true;
};
void SlaveConfig_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("SlaveConfig");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	pEm->SetAttribute("MaxBinaryEvents", ToString_int(MaxBinaryEvents));
	pEm->SetAttribute("MaxAnalogEvents", ToString_int(MaxAnalogEvents));
	pEm->SetAttribute("MaxCounterEvents", ToString_int(MaxCounterEvents));
	StaticRsp.toXml(pEm, true, aIgnoreValid);
	EventRsp.toXml(pEm, true, aIgnoreValid);
	TimeIINTask.toXml(pEm, true, aIgnoreValid);
	UnsolDefaults.toXml(pEm, true, aIgnoreValid);
};

void Slave_t :: fromXml(TiXmlNode* pNode){
	if(pNode == NULL)return;
	XML_CHECK("Slave",pNode->Type() == TiXmlNode::ELEMENT);
	TiXmlElement* pEm = pNode->ToElement();
	XML_CHECK("Slave",pEm != 0);
	Stack.fromXml(pNode->FirstChildElement("Stack"));
	SlaveConfig.fromXml(pNode->FirstChildElement("SlaveConfig"));
	VtoPorts.fromXml(pNode->FirstChildElement("VtoPorts"));
	valid=true;
};
void Slave_t :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){
	if(!aIgnoreValid && !valid) return;
	TiXmlElement * pEm;
	if(aCreateNode){
		pEm = new TiXmlElement("Slave");
		pParent->LinkEndChild(pEm);
	}else{
		pEm = pParent->ToElement();
	}
	Stack.toXml(pEm, true, aIgnoreValid);
	SlaveConfig.toXml(pEm, true, aIgnoreValid);
	VtoPorts.toXml(pEm, true, aIgnoreValid);
};

}
