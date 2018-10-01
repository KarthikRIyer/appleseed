
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

// Interface header.
#include "optixcontext.h"

// appleseed.renderer headers.
#include "renderer/global/globallogger.h"
#include "renderer/kernel/gpu/ptxcache.h"

namespace renderer
{

//
// OptixContext implementation.
//

struct OptixContext::Impl
{
    PTXCache            m_ptx_cache;

    optix::Context      m_context;
    optix::Group        m_scene;
    optix::Acceleration m_scene_accel;

    explicit Impl(const int device_number, const char* ptx_dir)
      : m_ptx_cache(ptx_dir)
    {
        m_context = optix::Context::create();

        // Pick a device.
        if (device_number >= 0)
        {
            for (unsigned int i = 0, e = m_context->getDeviceCount(); i < e; ++i)
            {
                int cuda_device_ordinal;
                m_context->getDeviceAttribute(
                    static_cast<int>(i),
                    RT_DEVICE_ATTRIBUTE_CUDA_DEVICE_ORDINAL,
                    sizeof(int),
                    &cuda_device_ordinal);

                if (cuda_device_ordinal == device_number)
                {
                    m_context->setDevices(&i, &i + 1);
                    break;
                }
            }
        }

        m_context->setEntryPointCount(1);
        m_context->setStackSize(400);

        // Init ray types.
        m_context->setRayTypeCount(1);

        // Create the scene root.
        m_scene = m_context->createGroup();
        m_scene_accel = m_context->createAcceleration("Trbvh");
        m_scene->setAcceleration(m_scene_accel);
        m_context["scene"]->set(m_scene);

    #ifndef NDEBUG
        m_context->setUsageReportCallback(&Impl::usage_report_callback, 3, nullptr);

        m_context->setPrintEnabled(true);
        m_context->setPrintBufferSize(4096);

        m_context->setExceptionEnabled(RT_EXCEPTION_ALL, true);
    #else
        //m_context->setUsageReportCallback(&Impl::usage_report_callback, 1, nullptr);
        //m_context->setExceptionEnabled(RT_EXCEPTION_ALL, false);
    #endif
    }

    ~Impl()
    {
        m_scene_accel->destroy();
        m_scene->destroy();
        m_context->destroy();
    }

    static void usage_report_callback(int /* verbosity */, const char* tag, const char* message, void* /* user_data */)
    {
        RENDERER_LOG_DEBUG("%s: %s", tag, message);
    }
};

OptixContext::OptixContext(const char* ptx_dir)
  : impl(new Impl(-1, ptx_dir))
{
}

OptixContext::OptixContext(const int device_number, const char* ptx_dir)
  : impl(new Impl(device_number, ptx_dir))
{
}

OptixContext::~OptixContext()
{
    delete impl;
}

optix::Group OptixContext::get_scene()
{
    return impl->m_scene;
}

optix::Transform OptixContext::create_transform()
{
    return impl->m_context->createTransform();
}

optix::GeometryGroup OptixContext::create_geometry_group()
{
    return impl->m_context->createGeometryGroup();
}

optix::Geometry OptixContext::create_geometry()
{
    return impl->m_context->createGeometry();
}

optix::GeometryInstance OptixContext::create_geometry_instance()
{
    return impl->m_context->createGeometryInstance();
}

optix::Acceleration OptixContext::create_acceleration(const char *builder)
{
    return impl->m_context->createAcceleration(builder);
}

optix::Material OptixContext::create_material()
{
    return impl->m_context->createMaterial();
}

optix::Program OptixContext::create_program_from_ptx_file(const char* filename, const char* program)
{
    return impl->m_ptx_cache.create_optix_program(impl->m_context, filename, program);
}

optix::Buffer OptixContext::create_buffer(unsigned int type, RTformat format, RTsize width)
{
    return impl->m_context->createBuffer(type, format, width);
}

optix::Buffer OptixContext::create_buffer(unsigned int type, RTformat format, RTsize width, RTsize height)
{
    return impl->m_context->createBuffer(type, format, width, height);
}

optix::Handle<optix::VariableObj> OptixContext::operator[](const std::string& variable_name)
{
    return impl->m_context[variable_name];
}

optix::Handle<optix::VariableObj> OptixContext::operator[](const char* variable_name)
{
    return impl->m_context[variable_name];
}

void OptixContext::validate()
{
    impl->m_context->validate();
}

}   // namespace renderer
