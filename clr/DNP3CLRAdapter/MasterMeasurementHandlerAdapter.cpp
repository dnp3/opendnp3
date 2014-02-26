#include "SOEHandlerAdapter.h"


#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{

SOEHandlerAdapter::SOEHandlerAdapter(DNP3::Interface::IMeasurementHandler^ aProxy) : proxy(aProxy)
{}


}
}
