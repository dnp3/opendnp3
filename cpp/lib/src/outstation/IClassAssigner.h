/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
#ifndef OPENDNP3_ICLASSASSIGNER_H
#define OPENDNP3_ICLASSASSIGNER_H

#include "app/GroupVariationRecord.h"
#include "app/Range.h"

#include "opendnp3/gen/AssignClassType.h"
#include "opendnp3/gen/PointClass.h"

namespace opendnp3
{

/**
 * An interface used to process assign class requests
 */
class IClassAssigner
{
public:
    /**
     *	@return the full range for the actual type, an invalid range if the type doesn't exist
     */
    virtual Range AssignClassToAll(AssignClassType type, PointClass clazz) = 0;

    /**
     *	@return the portion of the requested range that is valid, an invalid range if the type doesn't exist
     */
    virtual Range AssignClassToRange(AssignClassType type, PointClass clazz, const Range& range) = 0;
};

} // namespace opendnp3

#endif
