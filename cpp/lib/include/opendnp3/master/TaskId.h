/*
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
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
#ifndef OPENDNP3_TASKID_H
#define OPENDNP3_TASKID_H

namespace opendnp3
{

/**
 * Interface that represents a running master.
 */
class TaskId
{
public:
    static TaskId Defined(int id)
    {
        return TaskId(id, true);
    }
    static TaskId Undefined()
    {
        return TaskId(-1, false);
    }

    int GetId() const
    {
        return id;
    }
    bool IsDefined() const
    {
        return isDefined;
    }

private:
    TaskId() = delete;

    TaskId(int id_, bool isDefined_) : id(id_), isDefined(isDefined_) {}

    int id;
    bool isDefined;
};

} // namespace opendnp3

#endif
