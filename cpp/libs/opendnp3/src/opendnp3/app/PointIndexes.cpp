/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include "PointIndexes.h"

#include <openpal/util/Limits.h>

namespace opendnp3
{
    const uint32_t PointRange::MIN(openpal::MinValue<uint16_t>());
    const uint32_t PointRange::MAX(openpal::MaxValue<uint16_t>());
    const uint32_t PointRange::OUTOFRANGE(openpal::MaxValue<uint32_t>());
    PointRange PointRange::FULLRANGE = { PointRange::MIN, PointRange::MAX, PointRange::MIN };
    PointRange PointRange::EMPTYRANGE = { PointRange::MAX, PointRange::MIN, PointRange::MIN };

    const PointIndexes PointIndexes::FULLINDEXES(openpal::Indexable<PointRange, uint32_t>(&PointRange::FULLRANGE,1));
    const PointIndexes PointIndexes::EMPTYINDEXES(openpal::Indexable<PointRange, uint32_t>(&PointRange::EMPTYRANGE,0));
}
