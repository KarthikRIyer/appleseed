
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

#ifndef APPLESEED_RENDERER_KERNEL_RENDERING_OPTIX_OPTIXRENDERERSERVICES_H
#define APPLESEED_RENDERER_KERNEL_RENDERING_OPTIX_OPTIXRENDERERSERVICES_H

// appleseed.foundation headers.
#include "foundation/platform/compiler.h"

// OSL headers.
#include "foundation/platform/_beginoslheaders.h"
#include "OSL/rendererservices.h"
#include "foundation/platform/_endoslheaders.h"

namespace renderer
{

//
// appleseed's OSL OptixRendererServices.
//

class OptixRendererServices
  : public OSL::RendererServices
{
  public:
    OptixRendererServices();

    int supports(OIIO::string_view feature) const override;

    bool get_matrix(
        OSL::ShaderGlobals*         sg,
        OSL::Matrix44&              result,
        OSL::TransformationPtr      xform,
        float                       time) override;

    bool get_matrix(
        OSL::ShaderGlobals*         sg,
        OSL::Matrix44&              result,
        OSL::TransformationPtr      xform) override;

    bool get_matrix(
        OSL::ShaderGlobals*         sg,
        OSL::Matrix44&              result,
        OIIO::ustring               from,
        float                       time) override;

    bool get_matrix(
        OSL::ShaderGlobals*         sg,
        OSL::Matrix44&              result,
        OIIO::ustring               from) override;

    bool get_attribute(
        OSL::ShaderGlobals*         sg,
        bool                        derivatives,
        OIIO::ustring               object,
        OIIO::TypeDesc              type,
        OIIO::ustring               name,
        void*                       val) override;

    bool get_array_attribute(
        OSL::ShaderGlobals*         sg,
        bool                        derivatives,
        OIIO::ustring               object,
        OIIO::TypeDesc              type,
        OIIO::ustring               name,
        int                         index,
        void*                       val) override;

    bool get_userdata(
        bool                        derivatives,
        OIIO::ustring               name,
        OIIO::TypeDesc              type,
        OSL::ShaderGlobals*         sg,
        void*                       val) override;
};

}       // namespace renderer

#endif  // !APPLESEED_RENDERER_KERNEL_RENDERING_OPTIX_OPTIXRENDERERSERVICES_H
