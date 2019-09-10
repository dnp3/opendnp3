/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPENDNP3_IMASTEROPERATIONS_H
#define OPENDNP3_IMASTEROPERATIONS_H

#include "opendnp3/StackStatistics.h"
#include "opendnp3/util/TimeDuration.h"
#include "opendnp3/app/ClassField.h"
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/gen/FunctionCode.h"
#include "opendnp3/gen/RestartType.h"
#include "opendnp3/master/HeaderTypes.h"
#include "opendnp3/master/ICommandProcessor.h"
#include "opendnp3/master/IMasterScan.h"
#include "opendnp3/master/RestartOperationResult.h"
#include "opendnp3/master/ISOEHandler.h"
#include "opendnp3/master/TaskConfig.h"

#include <log4cpp/LogLevels.h>

#include <memory>
#include <string>
#include <vector>

namespace opendnp3
{

/**
 * All the operations that the user can perform on a running master
 */
class IMasterOperations : public ICommandProcessor
{
public:
    virtual ~IMasterOperations() {}

    /**
     *  @param filters Adjust the filters to this value
     */
    virtual void SetLogFilters(const log4cpp::LogLevels& filters) = 0;

    /**
     * Add a recurring user-defined scan from a vector of headers
     * @ return A proxy class used to manipulate the scan
     */
    virtual std::shared_ptr<IMasterScan> AddScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                 TimeDuration period,
                                                 const std::vector<Header>& headers,
                                                 const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Add a scan that requests all objects using qualifier code 0x06
     * @ return A proxy class used to manipulate the scan
     */
    virtual std::shared_ptr<IMasterScan> AddAllObjectsScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                           GroupVariationID gvId,
                                                           TimeDuration period,
                                                           const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Add a class-based scan to the master
     * @return A proxy class used to manipulate the scan
     */
    virtual std::shared_ptr<IMasterScan> AddClassScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                      const ClassField& field,
                                                      TimeDuration period,
                                                      const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Add a start/stop (range) scan to the master
     * @return A proxy class used to manipulate the scan
     */
    virtual std::shared_ptr<IMasterScan> AddRangeScan(std::shared_ptr<ISOEHandler> soe_handler,
                                                      GroupVariationID gvId,
                                                      uint16_t start,
                                                      uint16_t stop,
                                                      TimeDuration period,
                                                      const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Initiate a single user defined scan via a vector of headers
     */
    virtual void Scan(std::shared_ptr<ISOEHandler> soe_handler,
                      const std::vector<Header>& headers,
                      const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Initiate a single scan that requests all objects (0x06 qualifier code) for a certain group and variation
     */
    virtual void ScanAllObjects(std::shared_ptr<ISOEHandler> soe_handler,
                                GroupVariationID gvId,
                                const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Initiate a single class-based scan
     */
    virtual void ScanClasses(std::shared_ptr<ISOEHandler> soe_handler,
                             const ClassField& field,
                             const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Initiate a single start/stop (range) scan
     */
    virtual void ScanRange(std::shared_ptr<ISOEHandler> soe_handler,
                           GroupVariationID gvId,
                           uint16_t start,
                           uint16_t stop,
                           const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Write a time and interval object to a specific index
     */
    virtual void Write(const TimeAndInterval& value, uint16_t index, const TaskConfig& config = TaskConfig::Default())
        = 0;

    /**
     * Perform a cold or warm restart and get back the time-to-complete value
     */
    virtual void Restart(RestartType op,
                         const RestartOperationCallbackT& callback,
                         TaskConfig config = TaskConfig::Default())
        = 0;

    /**
     * Perform any operation that requires just a function code
     */
    virtual void PerformFunction(const std::string& name,
                                 FunctionCode func,
                                 const std::vector<Header>& headers,
                                 const TaskConfig& config = TaskConfig::Default())
        = 0;
};

} // namespace opendnp3

#endif
