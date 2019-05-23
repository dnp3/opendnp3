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
#ifndef OPENPAL_ARRAY_H
#define OPENPAL_ARRAY_H

#include "ArrayView.h"

#include "openpal/Configure.h"
#include "openpal/util/Uncopyable.h"

#include <assert.h>

namespace openpal
{

/**
 * Template type for a dynamically allocated array
 */
template<class T, class W> class Array : public HasSize<W>
{

public:
    Array(W size) : HasSize<W>(size), buffer(new T[size]()) {}

    Array() : HasSize<W>(0), buffer(nullptr) {}

    Array(const Array& copy) : HasSize<W>(copy.Size()), buffer(new T[copy.Size()])
    {
        for (W i = 0; i < copy.Size(); ++i)
            buffer[i] = copy.buffer[i];
    }

    virtual ~Array()
    {
        delete[] buffer;
    }

    ArrayView<T, W> ToView() const
    {
        return ArrayView<T, W>(buffer, this->size);
    }

    inline bool Contains(W index) const
    {
        return index < this->size;
    }

    inline T& operator[](W index)
    {
        assert(index < this->size);
        return buffer[index];
    }

    const T& operator[](W index) const
    {
        assert(index < this->size);
        return buffer[index];
    }

protected:
    T* buffer;

private:
    Array& operator=(const Array&) = delete;
};

} // namespace openpal

#endif
