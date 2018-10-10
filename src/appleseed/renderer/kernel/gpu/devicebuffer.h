
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

#ifndef APPLESEED_RENDERER_KERNEL_GPU_DEVICEBUFFER_H
#define APPLESEED_RENDERER_KERNEL_GPU_DEVICEBUFFER_H

// appleseed.renderer headers.
#include "renderer/kernel/gpu/deviceptr.h"

// appleseed.foundation headers.
#include "foundation/core/concepts/noncopyable.h"
#include "foundation/core/exceptions/exceptioncudaerror.h"

// CUDA headers.
#include "cuda.h"

// Standard headers.
#include <cassert>

namespace renderer
{

template <typename T>
class DeviceBuffer
  : foundation::NonCopyable
{
  public:
    explicit DeviceBuffer(const size_t size)
      : m_ptr(0)
      , m_size(0)
    {
        reset(size);
    }

    ~DeviceBuffer()
    {
        if (m_ptr)
            cuMemFree(&m_ptr);
    }

    void reset(const size_t size = 0)
    {
        assert(size != 0);

        if (m_ptr)
            foundation::check_cuda_error(cuMemFree(&m_ptr));

        m_ptr = 0;
        m_size = 0;

        if (m_size != 0)
        {
            foundation::check_cuda_error(
                cuMemAlloc(&m_ptr, size * sizeof(T)));

            m_size = size;
        }
    }

    size_t size() const
    {
        return m_size;
    }

    DevicePtr<T> get()
    {
        return DevicePtr<T>(reinterpret_cast<T*>(m_ptr));
    }

  private:
    CUdeviceptr     m_ptr;
    const size_t    m_size;
};

}       // namespace renderer

#endif  // !APPLESEED_RENDERER_KERNEL_GPU_DEVICEBUFFER_H
