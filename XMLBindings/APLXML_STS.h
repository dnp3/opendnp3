/* ******************************
	GENERATED CONTENT DO NOT ALTER!
*********************************
*/
#ifndef _APLXML_STS_H_
#define _APLXML_STS_H_
#include <APLXML/tinybinding.h>
#include <XMLBindings/APLXML_Base.h>
#include <XMLBindings/APLXML_DNP.h>
using namespace std;
namespace APLXML_STS {
class SlaveTestSet_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	string LogFile;
	string PhysicalLayer;
	bool Remote;
	int RemotePort;
	bool LinkCommandStatus;
	bool StartOnline;
#ifdef SWIG
%immutable Log;
#endif
	APLXML_Base::Log_t Log;
#ifdef SWIG
%immutable Slave;
#endif
	APLXML_DNP::Slave_t Slave;
#ifdef SWIG
%immutable DeviceTemplate;
#endif
	APLXML_DNP::DeviceTemplate_t DeviceTemplate;
#ifdef SWIG
%immutable PhysicalLayerList;
#endif
	APLXML_Base::PhysicalLayerList_t PhysicalLayerList;
};
}
#endif

