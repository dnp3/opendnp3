/*
 * Copyright 2013-2020 Automatak, LLC
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
#ifndef OPENDNP3_SERIALIZER_H
#define OPENDNP3_SERIALIZER_H

#include <ser4cpp/container/SequenceTypes.h>

namespace opendnp3
{

template<class T> class Serializer
{
public:
    using read_func_t = bool (*)(ser4cpp::rseq_t& buffer, T& output);
    using write_func_t = bool (*)(const T& value, ser4cpp::wseq_t& buffer);

    Serializer() = default;

    Serializer(size_t size, read_func_t read_func, write_func_t write_func)
        : size(size), read_func(read_func), write_func(write_func)
    {
    }

    /**
     * @return The size (in bytes) required for every call to read/write
     */
    size_t get_size() const
    {
        return size;
    }

    /**
     * reads the value and advances the read buffer
     */
    bool read(ser4cpp::rseq_t& buffer, T& output) const
    {
        return (*read_func)(buffer, output);
    }

    /**
     * writes the value and advances the write buffer
     */
    bool write(const T& value, ser4cpp::wseq_t& buffer) const
    {
        return (*write_func)(value, buffer);
    }

private:
    size_t size = 0;
    read_func_t read_func = nullptr;
    write_func_t write_func = nullptr;
};
} // namespace opendnp3

#endif
