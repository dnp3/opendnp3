#include <asiopal/UTCTimeSource.h>

#include <chrono>

namespace asiopal
{

UTCTimeSource UTCTimeSource::mInstance;

openpal::IUTCTimeSource* UTCTimeSource::Inst()
{
	return &mInstance;
}

openpal::UTCTimestamp UTCTimeSource::Now()
{
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return openpal::UTCTimestamp(time);
}

}


