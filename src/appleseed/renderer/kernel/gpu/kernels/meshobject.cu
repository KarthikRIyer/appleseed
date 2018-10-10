
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
#include "renderer/kernel/gpu/kernels/shadingpoint.cuh"

// OptiX headers.
#include <optix_world.h>

using namespace optix;
using namespace foundation;
using namespace renderer::gpu;

//
// Inputs.
//

rtBuffer<float3> vertices;
rtBuffer<int3>   face_indices;

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );


//
// Attributes.
//

rtDeclareVariable(ShadingPoint, shading_point_attr, attribute shading_point, );


//
// Intersect.
//

RT_PROGRAM void intersect(int primitive_index)
{
    // Fetch triangle vertices.
    const int3 indices = face_indices[primitive_index];
    const float3 p0 = vertices[indices.x];
    const float3 p1 = vertices[indices.y];
    const float3 p2 = vertices[indices.z];

    // Intersect ray with triangle.
    float3 n;
    float t, beta, gamma;

    if (intersect_triangle(ray, p0, p1, p2, n, t, beta, gamma))
    {
        if (rtPotentialIntersection(t))
        {
            ShadingPoint shading_point;
            shading_point.m_flags = 1;
            shading_point.m_distance = t;
            //shading_point.geometric_normal = normalize(n);
            // ...

            shading_point_attr = shading_point;
            rtReportIntersection(0);
        }
    }
}


//
// Bounds.
//

RT_PROGRAM void bounds(int primitive_index, float result[6])
{
    const int3 indices = face_indices[primitive_index];

    const float3 v0 = vertices[indices.x];
    const float3 v1 = vertices[indices.y];
    const float3 v2 = vertices[indices.z];
    const float  area = length(cross(v1 - v0, v2 - v0));

    optix::Aabb* aabb = (optix::Aabb*)result;

    if(area > 0.0f && !isinf(area))
    {
        aabb->m_min = fminf(fminf(v0, v1), v2);
        aabb->m_max = fmaxf(fmaxf(v0, v1), v2);
    }
    else
        aabb->invalidate();
}

