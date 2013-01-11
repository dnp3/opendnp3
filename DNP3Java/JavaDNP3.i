%module(directors="1") javadnp3
%{
/* Includes the header in the wrapper code */

#include <APL/IPhysicalLayerObserver.h>
#include <DNP3/StackManager.h>

using namespace apl;
using namespace apl::dnp;

%}

%pragma(java) jniclasscode=%{
  static {
    if(System.getProperty("org.totalgrid.reef.protocol.dnp3.nostaticload") == null) System.loadLibrary("dnp3java");    
  }
%}

%include <std_string.i>
%include <std_vector.i>

%template(VectorOfString) std::vector<std::string>;

%feature("director"); //generate directors for all classes that have virtual methods

%apply short { boost::uint8_t };
%apply char { boost::int8_t };
%apply int { boost::uint16_t };
%apply short { boost::int16_t };
%apply int { boost::int32_t };
%apply long long { boost::uint32_t };
%apply long long { boost::int64_t };
%apply long long { boost::uint64_t };

%include "APL/Types.h"
%include "APL/INotifier.h"
%include "APL/LogTypes.h"
%include "APL/LogEntry.h"
%include "APL/LogBase.h"
%include "APL/PhysicalLayerStates.h"
%include "APL/IPhysicalLayerObserver.h"
%include "APL/PhysLayerSettings.h"
%include "APL/SerialTypes.h"
%include "APL/QualityMasks.h"
%include "APL/CommandTypes.h"

%include "APL/BaseDataTypes.h"
%template(DoublePoint) apl::TypedDataPoint<double>;
%template(UnsignedPoint) apl::TypedDataPoint<boost::uint32_t>;
%include "APL/DataTypes.h"

%include "APL/ITransactable.h"
%include "APL/DataInterfaces.h"
%include "APL/CommandInterfaces.h"

%include "DNP3/PointClass.h"
%include "DNP3/LinkConfig.h"
%include "DNP3/AppConfig.h"
%include "DNP3/MasterConfigTypes.h"
%include "DNP3/IStackObserver.h"
%include "DNP3/ClassMask.h"

%template(VectorOfExceptionScan) std::vector<apl::dnp::ExceptionScan>;
%include "DNP3/MasterConfig.h"
%include "DNP3/SlaveConfig.h"

%include "DNP3/DeviceTemplateTypes.h"
%template(VectorOfEventPointRecord) std::vector<apl::dnp::EventPointRecord>;
%template(VectorOfDeadbandPointRecord) std::vector<apl::dnp::DeadbandPointRecord>;
%template(VectorOfControlRecord) std::vector<apl::dnp::ControlRecord>;
%template(VectorOfPointRecord) std::vector<apl::dnp::PointRecord>;
%include "DNP3/DeviceTemplate.h"

%include "DNP3/MasterStackConfig.h"
%include "DNP3/SlaveStackConfig.h"


%include <exception.i>
// provide an exception handler for the stack manager
%exception {
	try {
		$action
	}
	catch (apl::Exception ex) {
		SWIG_exception(SWIG_ValueError, ex.what());
	}
	catch (std::exception ex) {
		SWIG_exception(SWIG_ValueError, ex.what());
	}
}


%include "DNP3/StackManager.h"

