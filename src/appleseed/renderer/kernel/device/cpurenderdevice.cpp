
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
#include "cpurenderdevice.h"

// appleseed.renderer headers.
#include "renderer/modeling/project/project.h"
#include "renderer/modeling/scene/scene.h"

// Standard headers.
#include <string>

using namespace foundation;
using namespace std;

namespace renderer
{

CPURenderDevice::CPURenderDevice(
    Project&                project,
    const ParamArray&       params,
    IRendererController*    renderer_controller)
  : RenderDeviceBase(project, params, renderer_controller)
{
}

CPURenderDevice::~CPURenderDevice()
{
}

bool CPURenderDevice::initialize(
    TextureStore&           texture_store,
    IAbortSwitch&           abort_switch)
{
    return false;
}

void CPURenderDevice::build_or_update_bvh()
{
#ifdef APPLESEED_WITH_EMBREE
    m_project.set_use_embree(
        m_params.get_optional<bool>("use_embree", false));
#endif

    m_project.update_trace_context();
}

IRendererController::Status CPURenderDevice::render_frame(
    ITileCallbackFactory*   tile_callback_factory,
    TextureStore&           texture_store,
    IAbortSwitch&           abort_switch)
{
    return IRendererController::AbortRendering;
}

}
