
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

#ifndef APPLESEED_FOUNDATION_CORE_EXCEPTIONS_EXCEPTIONCUDAERROR_H
#define APPLESEED_FOUNDATION_CORE_EXCEPTIONS_EXCEPTIONCUDAERROR_H

// appleseed.foundation headers.
#include "foundation/core/exceptions/exception.h"

// CUDA headers.
#include "cuda.h"

// Standard headers.
#include <string>

namespace foundation
{

//
// Exception thrown when a CUDA call fails.
//

class ExceptionCUDAError
  : public Exception
{
  public:
    // Constructor.
    explicit ExceptionCUDAError(CUresult error);
};


//
// ExceptionCUDAError class implementation.
//

inline ExceptionCUDAError::ExceptionCUDAError(CUresult error)
{
    std::string err;
    const char* error_string;

    if (cuGetErrorString(error, &error_string) == CUDA_SUCCESS)
    {
        err += "CUDA error: ";
        err += error_string;
        err += ".";
    }
    else
        err += "Unknown CUDA error.";

    set_what(err.c_str());
}

inline void check_cuda_error(const CUresult error)
{
    if (error != CUDA_SUCCESS)
        throw ExceptionCUDAError(error);
}

}       // namespace foundation

#endif  // !APPLESEED_FOUNDATION_CORE_EXCEPTIONS_EXCEPTIONCUDAERROR_H
