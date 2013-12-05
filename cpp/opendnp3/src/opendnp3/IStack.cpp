#include <opendnp3/IStack.h>


using namespace openpal;

namespace opendnp3
{

IStack::IStack(Logger& arLogger, std::function<void (bool)> aEnableDisableFunc) : 
	mLogger(arLogger),
	mEnableDisableFunc(aEnableDisableFunc)
{


}

IStack::~IStack()
{

}


void IStack::Enable()
{
	mEnableDisableFunc(true);
}

	
void IStack::Disable()
{
	mEnableDisableFunc(false);
}

}

