
//
// This source file is part of appleseed.
// Visit https://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2018 Esteban Tovagliari, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// Interface header.
#include "cudadevice.h"

// appleseed.foundation headers.
#include "foundation/core/exceptions/exceptioncudaerror.h"

// Standard headers.
#include <algorithm>
#include <cassert>

using namespace foundation;

namespace renderer
{

CUDADevice::CUDADevice(const int device_number)
  : m_device_number(device_number)
  , m_context(nullptr)
{
}

CUDADevice::~CUDADevice()
{
    if (m_context)
        cuCtxDestroy(m_context);
}

CUDADevice::CUDADevice(CUDADevice&& other)
  : m_device_number(other.m_device_number)
  , m_context(other.m_context)
{
    other.m_context = nullptr;
}

int CUDADevice::device_number() const
{
    return m_device_number;
}

void CUDADevice::create_context()
{
    assert(m_context == nullptr);
    check_cuda_error(cuCtxCreate(&m_context, 0, m_device_number));
}

CUcontext CUDADevice::context() const
{
    return m_context;
}

void CUDADevice::set_context_current() const
{
    assert(m_context);
    cuCtxSetCurrent(m_context);
}

CUDADeviceList::CUDADeviceList()
{
    int device_count;
    check_cuda_error(cuDeviceGetCount(&device_count));
    m_devices.reserve(static_cast<size_t>(device_count));

    for (int i = 0; i < device_count; ++i)
        m_devices.emplace_back(i);
}

bool CUDADeviceList::empty() const
{
    return m_devices.empty();
}

std::size_t CUDADeviceList::size() const
{
    return m_devices.size();
}

CUDADevice& CUDADeviceList::get_device(const size_t index)
{
    assert(index < size());
    return m_devices[index];
}

const CUDADevice& CUDADeviceList::get_device(const size_t index) const
{
    assert(index < size());
    return m_devices[index];
}

}
