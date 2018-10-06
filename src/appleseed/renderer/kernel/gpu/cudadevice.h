
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

#ifndef APPLESEED_RENDERER_KERNEL_GPU_CUDADEVICE_H
#define APPLESEED_RENDERER_KERNEL_GPU_CUDADEVICE_H

// appleseed.foundation headers.
#include "foundation/core/concepts/noncopyable.h"

// CUDA headers.
#include "cuda.h"

// Standard headers.
#include <vector>

namespace renderer
{

class CUDADevice
  : foundation::NonCopyable
{
  public:
    explicit CUDADevice(const int device_number);

    ~CUDADevice();

    CUDADevice(CUDADevice&& other);

    int device_number() const;

    void create_context();

    CUcontext context() const;

    void set_context_current() const;

  private:
    int         m_device_number;
    CUcontext   m_context;
};

class CUDADeviceList
  : foundation::NonCopyable
{
  public:
    static CUDADeviceList& instance();

    bool empty() const;

    std::size_t size() const;

    CUDADevice& get_device(const size_t index);
    const CUDADevice& get_device(const size_t index) const;

  private:
    std::vector<CUDADevice> m_devices;

    CUDADeviceList();
};

}       // namespace renderer

#endif  // !APPLESEED_RENDERER_KERNEL_GPU_CUDADEVICE_H
