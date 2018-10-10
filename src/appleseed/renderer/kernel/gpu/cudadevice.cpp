
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

// appleseed.renderer headers.
#include "renderer/global/globallogger.h"

// appleseed.foundation headers.
#include "foundation/core/exceptions/exceptioncudaerror.h"

// Standard headers.
#include <cassert>

using namespace foundation;

namespace renderer
{

CUDADevice::CUDADevice(const CUdevice device_number)
  : m_device_number(device_number)
{
    char device_name[256];
    cuDeviceGetName(device_name, 256, m_device_number);
    m_name = device_name;

    cuDeviceComputeCapability(
        &m_compute_capability.first,
        &m_compute_capability.second,
        m_device_number);

    cuDeviceGetAttribute(&m_compute_mode, CU_DEVICE_ATTRIBUTE_COMPUTE_MODE, m_device_number);

    cuDeviceTotalMem(&m_total_mem, m_device_number);

    cuDeviceGetAttribute(&m_max_threads_per_block, CU_DEVICE_ATTRIBUTE_MAX_THREADS_PER_BLOCK, m_device_number);

    cuDeviceGetAttribute(&m_max_block_dim_x, CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_X, m_device_number);
    cuDeviceGetAttribute(&m_max_block_dim_y, CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Y, m_device_number);
    cuDeviceGetAttribute(&m_max_block_dim_z, CU_DEVICE_ATTRIBUTE_MAX_BLOCK_DIM_Z, m_device_number);

    cuDeviceGetAttribute(&m_max_grid_dim_x, CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_X, m_device_number);
    cuDeviceGetAttribute(&m_max_grid_dim_y, CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Y, m_device_number);
    cuDeviceGetAttribute(&m_max_grid_dim_z, CU_DEVICE_ATTRIBUTE_MAX_GRID_DIM_Z, m_device_number);

    cuDeviceGetAttribute(&m_max_registers, CU_DEVICE_ATTRIBUTE_MAX_REGISTERS_PER_BLOCK, m_device_number);
}

namespace
{

bool is_device_supported(const CUDADevice& device)
{
    if (device.m_compute_mode == CU_COMPUTEMODE_PROHIBITED)
        return false;

    // todo: check more things here...

    return true;
}

}

CUDADeviceList::CUDADeviceList()
{
    int device_count;
    check_cuda_error(cuDeviceGetCount(&device_count));
    m_devices.reserve(static_cast<size_t>(device_count));
    m_contexts.resize(static_cast<size_t>(device_count), nullptr);

    for (int i = 0; i < device_count; ++i)
    {
        CUDADevice device(i);

        if (is_device_supported(device))
            m_devices.push_back(device);
    }
}

CUDADeviceList::~CUDADeviceList()
{
    for (size_t i = 0, e = m_contexts.size(); i < e; ++i)
    {
        if (m_contexts[i] != nullptr)
            cuDevicePrimaryCtxRelease(static_cast<CUdevice>(i));
    }
}

CUDADeviceList& CUDADeviceList::instance()
{
    static CUDADeviceList x;
    return x;
}

bool CUDADeviceList::empty() const
{
    return m_devices.empty();
}

std::size_t CUDADeviceList::size() const
{
    return m_devices.size();
}

const CUDADevice& CUDADeviceList::get_device(const size_t index) const
{
    assert(index < size());
    return m_devices[index];
}

const CUDADevice& CUDADeviceList::pick_best_device() const
{
    assert(size() > 0);

    // for now, we pick device #0.
    // todo: do something better here...
    const size_t device_number = 0;
    const CUDADevice& device = m_devices[device_number];

    // Create a primary context for the device if needed.
    if (m_contexts[device_number] == nullptr)
    {
        const CUdevice dev = static_cast<CUdevice>(device_number);
        RENDERER_LOG_DEBUG("Creating CUDA primary context for device %d", dev);

        CUcontext ctx;
        check_cuda_error(cuDevicePrimaryCtxRetain(&ctx, device_number));

        // todo: set context flags here.
        // cuDevicePrimaryCtxSetFlags(...);
        // ...

        check_cuda_error(cuCtxPushCurrent(ctx));
        m_contexts[device_number] = ctx;
    }

    return device;
}

}
