
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
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

// appleseed.renderer headers.

// appleseed.renderer headers.
#include "renderer/kernel/gpu/kernels/ray.cuh"
#include "renderer/kernel/gpu/kernels/shadingpoint.cuh"

// OptiX headers.
#include <optix_world.h>

using namespace renderer;
using namespace foundation;
using namespace optix;


//
// Launch variables.
//

rtDeclareVariable(unsigned int, launch_index, rtLaunchIndex, );


//
// Scene variables.
//

rtDeclareVariable(rtObject, scene, , );


//
// Ray buffer.
//

rtBuffer<gpu::Ray, 1> rays;


//
// Outputs.
//

rtBuffer<gpu::ShadingPoint, 1> shading_points;


//
// Intersection programs.
//

RT_PROGRAM void raycast()
{
    gpu::ShadingPoint ray_shading_point;
    ray_shading_point.m_flags = 0;
    // todo: init ray shading point here...

    gpu::Ray ray = rays[launch_index];

    rtTrace(
        scene,
        optix::make_Ray(
            make_float3(ray.m_org.x, ray.m_org.y, ray.m_org.z),
            make_float3(ray.m_dir.x, ray.m_dir.y, ray.m_dir.z),
            0, // OptiX ray type.
            ray.m_tmin,
            ray.m_tmax),
            ray_shading_point);

    // Copy ray payload to output buffer.
    shading_points[launch_index] = ray_shading_point;
}
