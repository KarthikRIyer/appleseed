
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
#include "optixrendererservices.h"

namespace renderer
{

//
// OptixRendererServices class implementation.
//

OptixRendererServices::OptixRendererServices()
  : OSL::RendererServices()
{
}

int OptixRendererServices::supports(OIIO::string_view feature) const
{
    if (feature == "OptiX")
        return true;

    return false;
}

bool OptixRendererServices::get_matrix(
    OSL::ShaderGlobals*         sg,
    OSL::Matrix44&              result,
    OSL::TransformationPtr      xform,
    float                       time)
{
    return false;
}

bool OptixRendererServices::get_matrix(
    OSL::ShaderGlobals*         sg,
    OSL::Matrix44&              result,
    OSL::TransformationPtr      xform)
{
    return false;
}

bool OptixRendererServices::get_matrix(
    OSL::ShaderGlobals*         sg,
    OSL::Matrix44&              result,
    OIIO::ustring               from,
    float                       time)
{
    return false;
}

bool OptixRendererServices::get_matrix(
    OSL::ShaderGlobals*         sg,
    OSL::Matrix44&              result,
    OIIO::ustring               from)
{
    return false;
}

bool OptixRendererServices::get_attribute(
    OSL::ShaderGlobals*         sg,
    bool                        derivatives,
    OIIO::ustring               object,
    OIIO::TypeDesc              type,
    OIIO::ustring               name,
    void*                       val)
{
    return false;
}

bool OptixRendererServices::get_array_attribute(
    OSL::ShaderGlobals*         sg,
    bool                        derivatives,
    OIIO::ustring               object,
    OIIO::TypeDesc              type,
    OIIO::ustring               name,
    int                         index,
    void*                       val)
{
    return false;
}

bool OptixRendererServices::get_userdata(
    bool                        derivatives,
    OIIO::ustring               name,
    OIIO::TypeDesc              type,
    OSL::ShaderGlobals*         sg,
    void*                       val)
{
    return false;
}

}   // namespace renderer
