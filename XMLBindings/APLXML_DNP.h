/* ******************************
	GENERATED CONTENT DO NOT ALTER!
*********************************
*/
#ifndef _APLXML_DNP_H_
#define _APLXML_DNP_H_
#include <APLXML/tinybinding.h>
using namespace std;
namespace APLXML_DNP {
class Analog_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int Index;
	string Name;
	int ClassGroup;
	int Deadband;
};
#ifdef SWIG
}
%template(Analog_c) std::vector<APLXML_DNP::Analog_t*>;
namespace APLXML_DNP{
#endif
class AnalogData_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	AnalogData_t();

#ifdef SWIG
%immutable AnalogVector;
#endif
	private: collectedType < Analog_t > Analog;
	public: vector < Analog_t* >& AnalogVector;
};
class AppLayer_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int TimeoutMS;
	int MaxFragSize;
	int NumRetries;
};
class Binary_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int Index;
	string Name;
	int ClassGroup;
};
#ifdef SWIG
}
%template(Binary_c) std::vector<APLXML_DNP::Binary_t*>;
namespace APLXML_DNP{
#endif
class BinaryData_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	BinaryData_t();

#ifdef SWIG
%immutable BinaryVector;
#endif
	private: collectedType < Binary_t > Binary;
	public: vector < Binary_t* >& BinaryVector;
};
class Control_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int Index;
	string Name;
	string ControlMode;
	int SelectTimeoutMS;
};
#ifdef SWIG
}
%template(Control_c) std::vector<APLXML_DNP::Control_t*>;
namespace APLXML_DNP{
#endif
class ControlData_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	ControlData_t();

#ifdef SWIG
%immutable ControlVector;
#endif
	private: collectedType < Control_t > Control;
	public: vector < Control_t* >& ControlVector;
};
class ControlStatus_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int Index;
	string Name;
};
#ifdef SWIG
}
%template(ControlStatus_c) std::vector<APLXML_DNP::ControlStatus_t*>;
namespace APLXML_DNP{
#endif
class ControlStatusData_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	ControlStatusData_t();

#ifdef SWIG
%immutable ControlStatusVector;
#endif
	private: collectedType < ControlStatus_t > ControlStatus;
	public: vector < ControlStatus_t* >& ControlStatusVector;
};
class Counter_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int Index;
	int ClassGroup;
	string Name;
};
#ifdef SWIG
}
%template(Counter_c) std::vector<APLXML_DNP::Counter_t*>;
namespace APLXML_DNP{
#endif
class CounterData_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	CounterData_t();

#ifdef SWIG
%immutable CounterVector;
#endif
	private: collectedType < Counter_t > Counter;
	public: vector < Counter_t* >& CounterVector;
};
class ExceptionScan_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	bool Class1;
	bool Class2;
	bool Class3;
	int PeriodMS;
};
class GrpVar_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int Grp;
	int Var;
};
class LinkLayer_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	bool IsMaster;
	int LocalAddress;
	int RemoteAddress;
	bool UseConfirmations;
	int AckTimeoutMS;
	int NumRetries;
};
class MasterSettings_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	bool AllowTimeSync;
	int TaskRetryMS;
	int IntegrityPeriodMS;
	bool UseNonStandardVtoTransferCode;
};
#ifdef SWIG
}
%template(ExceptionScan_c) std::vector<APLXML_DNP::ExceptionScan_t*>;
namespace APLXML_DNP{
#endif
class ScanList_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	ScanList_t();

#ifdef SWIG
%immutable ExceptionScanVector;
#endif
	private: collectedType < ExceptionScan_t > ExceptionScan;
	public: vector < ExceptionScan_t* >& ExceptionScanVector;
};
class Setpoint_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int Index;
	string Name;
	string ControlMode;
	int SelectTimeoutMS;
};
#ifdef SWIG
}
%template(Setpoint_c) std::vector<APLXML_DNP::Setpoint_t*>;
namespace APLXML_DNP{
#endif
class SetpointData_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	SetpointData_t();

#ifdef SWIG
%immutable SetpointVector;
#endif
	private: collectedType < Setpoint_t > Setpoint;
	public: vector < Setpoint_t* >& SetpointVector;
};
class SetpointStatus_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int Index;
	string Name;
};
#ifdef SWIG
}
%template(SetpointStatus_c) std::vector<APLXML_DNP::SetpointStatus_t*>;
namespace APLXML_DNP{
#endif
class SetpointStatusData_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	SetpointStatusData_t();

#ifdef SWIG
%immutable SetpointStatusVector;
#endif
	private: collectedType < SetpointStatus_t > SetpointStatus;
	public: vector < SetpointStatus_t* >& SetpointStatusVector;
};
class SetpointStatusGrpVar_t : public APLXML_DNP::GrpVar_t{
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
};
class Stack_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
#ifdef SWIG
%immutable LinkLayer;
#endif
	LinkLayer_t LinkLayer;
#ifdef SWIG
%immutable AppLayer;
#endif
	AppLayer_t AppLayer;
};
class TimeIINTask_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	bool DoTask;
	int PeriodMS;
};
class Unsol_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	bool DoTask;
	bool Enable;
	bool Class1;
	bool Class2;
	bool Class3;
};
class UnsolDefaults_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	bool DoClass1;
	bool DoClass2;
	bool DoClass3;
	int PackDelayMS;
	int RetryMS;
};
class VtoPort_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	string PhysicalLayer;
	int Index;
	bool StartLocal;
	int OpenRetry;
};
#ifdef SWIG
}
%template(VtoPort_c) std::vector<APLXML_DNP::VtoPort_t*>;
namespace APLXML_DNP{
#endif
class VtoPorts_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	VtoPorts_t();

#ifdef SWIG
%immutable VtoPortVector;
#endif
	private: collectedType < VtoPort_t > VtoPort;
	public: vector < VtoPort_t* >& VtoPortVector;
};
class AnalogGrpVar_t : public APLXML_DNP::GrpVar_t{
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
};
class BinaryGrpVar_t : public APLXML_DNP::GrpVar_t{
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
};
class CounterGrpVar_t : public APLXML_DNP::GrpVar_t{
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
};
class DeviceTemplate_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	string TemplateName;
#ifdef SWIG
%immutable BinaryData;
#endif
	BinaryData_t BinaryData;
#ifdef SWIG
%immutable AnalogData;
#endif
	AnalogData_t AnalogData;
#ifdef SWIG
%immutable CounterData;
#endif
	CounterData_t CounterData;
#ifdef SWIG
%immutable SetpointStatusData;
#endif
	SetpointStatusData_t SetpointStatusData;
#ifdef SWIG
%immutable ControlStatusData;
#endif
	ControlStatusData_t ControlStatusData;
#ifdef SWIG
%immutable SetpointData;
#endif
	SetpointData_t SetpointData;
#ifdef SWIG
%immutable ControlData;
#endif
	ControlData_t ControlData;
};
class EventRsp_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
#ifdef SWIG
%immutable BinaryGrpVar;
#endif
	BinaryGrpVar_t BinaryGrpVar;
#ifdef SWIG
%immutable AnalogGrpVar;
#endif
	AnalogGrpVar_t AnalogGrpVar;
#ifdef SWIG
%immutable CounterGrpVar;
#endif
	CounterGrpVar_t CounterGrpVar;
};
class Master_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
#ifdef SWIG
%immutable Stack;
#endif
	Stack_t Stack;
#ifdef SWIG
%immutable MasterSettings;
#endif
	MasterSettings_t MasterSettings;
#ifdef SWIG
%immutable ScanList;
#endif
	ScanList_t ScanList;
#ifdef SWIG
%immutable Unsol;
#endif
	Unsol_t Unsol;
#ifdef SWIG
%immutable VtoPorts;
#endif
	VtoPorts_t VtoPorts;
};
class StaticRsp_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
#ifdef SWIG
%immutable BinaryGrpVar;
#endif
	BinaryGrpVar_t BinaryGrpVar;
#ifdef SWIG
%immutable AnalogGrpVar;
#endif
	AnalogGrpVar_t AnalogGrpVar;
#ifdef SWIG
%immutable CounterGrpVar;
#endif
	CounterGrpVar_t CounterGrpVar;
#ifdef SWIG
%immutable SetpointStatusGrpVar;
#endif
	SetpointStatusGrpVar_t SetpointStatusGrpVar;
};
class SlaveConfig_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	int MaxBinaryEvents;
	int MaxAnalogEvents;
	int MaxCounterEvents;
#ifdef SWIG
%immutable StaticRsp;
#endif
	StaticRsp_t StaticRsp;
#ifdef SWIG
%immutable EventRsp;
#endif
	EventRsp_t EventRsp;
#ifdef SWIG
%immutable TimeIINTask;
#endif
	TimeIINTask_t TimeIINTask;
#ifdef SWIG
%immutable UnsolDefaults;
#endif
	UnsolDefaults_t UnsolDefaults;
};
class Slave_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
#ifdef SWIG
%immutable Stack;
#endif
	Stack_t Stack;
#ifdef SWIG
%immutable SlaveConfig;
#endif
	SlaveConfig_t SlaveConfig;
#ifdef SWIG
%immutable VtoPorts;
#endif
	VtoPorts_t VtoPorts;
};
}
#endif

