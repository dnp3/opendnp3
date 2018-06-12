#include "OutstationTestObject.h"

#include <dnp3mocks/APDUHexBuilders.h>
#include <testlib/HexConversions.h>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace testlib;


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    RSlice buffer(Data, Size);
    OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();
    t.SendToOutstation(buffer);
    return 0;
}
