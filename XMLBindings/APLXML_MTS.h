/* ******************************
	GENERATED CONTENT DO NOT ALTER!
*********************************
*/
#ifndef _APLXML_MTS_H_
#define _APLXML_MTS_H_
#include <APLXML/tinybinding.h>
#include <XMLBindings/APLXML_Base.h>
#include <XMLBindings/APLXML_DNP.h>
using namespace std;
namespace APLXML_MTS {
class MasterTestSet_t : public IXMLDataBound {
public:
	void toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);
	void fromXml(TiXmlNode* pNode);
	string PhysicalLayer;
	string LogFile;
#ifdef SWIG
%immutable Log;
#endif
	APLXML_Base::Log_t Log;
#ifdef SWIG
%immutable Master;
#endif
	APLXML_DNP::Master_t Master;
#ifdef SWIG
%immutable PhysicalLayerList;
#endif
	APLXML_Base::PhysicalLayerList_t PhysicalLayerList;
};
}
#endif

