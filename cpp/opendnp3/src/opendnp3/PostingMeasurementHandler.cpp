#include "PostingMeasurementHandler.h"

#include <openpal/IExecutor.h>
#include <openpal/MoveOnCopy.h>

using namespace openpal;

namespace opendnp3
{

PostingMeasurementHandler::PostingMeasurementHandler(IMeasurementHandler* apProxy, openpal::IExecutor* apExecutor) : 
	mpProxy(apProxy),
	mpExecutor(apExecutor),
	Load([this](MeasurementUpdate& update) {
		move_on_copy<MeasurementUpdate> wrapper(std::move(update));
		mpExecutor->Post([this, wrapper](){ mpProxy->Load(wrapper.Value()); });
	})	
{

	
}

}

