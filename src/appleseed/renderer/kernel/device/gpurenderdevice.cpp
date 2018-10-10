
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
#include "gpurenderdevice.h"

// appleseed.renderer headers.
#include "renderer/kernel/intersection/optixtracecontext.h"
#include "renderer/kernel/rendering/iframerenderer.h"
#include "renderer/modeling/project/project.h"

// appleseed.foundation headers.
#include "foundation/core/exceptions/exceptioncudaerror.h"

// OptiX headers.
#include "optixu/optixpp.h"

// CUDA headers.
#include "cuda.h"

using namespace foundation;

namespace renderer
{
namespace
{
    bool g_cuda_initialized = false;
}

GPURenderDevice::GPURenderDevice(
    Project&                project,
    const ParamArray&       params,
    const char*             ptx_dir,
    IRendererController*    renderer_controller)
  : RenderDeviceBase(project, params, renderer_controller)
  , m_ptx_dir(ptx_dir)
{
    // Init CUDA if needed.
    if (!g_cuda_initialized)
    {
        RENDERER_LOG_DEBUG("Initializing CUDA");
        check_cuda_error(cuInit(0));
        g_cuda_initialized = true;
    }

    CUDADeviceList& device_list = CUDADeviceList::instance();

    if (device_list.empty())
        RENDERER_LOG_ERROR("No GPU devices found");

    m_cuda_device = &device_list.pick_best_device();
}

GPURenderDevice::~GPURenderDevice()
{
}

bool GPURenderDevice::initialize(
    TextureStore&           texture_store,
    IAbortSwitch&           abort_switch)
{
    return true;
}

bool GPURenderDevice::build_or_update_bvh()
{
    try
    {
        m_project.update_optix_trace_context(m_cuda_device->m_device_number, m_ptx_dir);
    }
    catch(const optix::Exception& e)
    {
        RENDERER_LOG_ERROR("Error while updating OptiX context: %s\n", e.what());
        return false;
    }

    return true;
}

IRendererController::Status GPURenderDevice::render_frame(
    ITileCallbackFactory*   tile_callback_factory,
    TextureStore&           texture_store,
    IAbortSwitch&           abort_switch)
{
    try
    {
        // ...
    }
    catch(const optix::Exception& e)
    {
        RENDERER_LOG_ERROR("OptiX error: %s\n", e.what());
    }
    catch(const ExceptionCUDAError& e)
    {
        RENDERER_LOG_ERROR("CUDA error: %s\n", e.what());
    }
    catch(const Exception& e)
    {
        RENDERER_LOG_ERROR("Exception: %s\n", e.what());
    }

    return IRendererController::AbortRendering;
}

void GPURenderDevice::print_settings() const
{
    // todo: implement me...
}

}
