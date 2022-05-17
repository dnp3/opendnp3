
#ifndef OPENDNP3_NULLLOGHANDLER_H
#define OPENDNP3_NULLLOGHANDLER_H

#include "opendnp3/logging/Logger.h"

class NullLogHandler : public opendnp3::ILogHandler
{
    void log(opendnp3::ModuleId module,
                     const char* id,
                     opendnp3::LogLevel level,
                     char const* location,
                     char const* message) override {}
};

class NullLogger
{
public:
    opendnp3::Logger get_logger() const {
        return opendnp3::Logger(this->handler, opendnp3::ModuleId(0), "logger", opendnp3::LogLevels(~0));
    }

private:
    std::shared_ptr<opendnp3::ILogHandler> handler;
};

#endif // OPENDNP3_NULLLOGHANDLER_H
