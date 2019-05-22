#include <openpal/container/Buffer.h>
#include <openpal/logging/Logger.h>

#include <opendnp3/LogLevels.h>

#include <asiodnp3/ConsoleLogger.h>

#include <dnp3decode/Decoder.h>

using namespace std;
using namespace openpal;
using namespace opendnp3;
using namespace asiodnp3;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size)
{
    RSlice buffer(Data, Size);
    openpal::Logger logger(ConsoleLogger::Create(), "decoder", LogFilters(0));
    IDecoderCallbacks callback;
    Decoder decoder(callback, logger);

    decoder.DecodeLPDU(buffer);

    return 0;
}
