
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
#include "ptxcache.h"

// Standard headers.
#include <fstream>
#include <sstream>

using namespace foundation;
using namespace std;

namespace bfs = boost::filesystem;

namespace renderer
{

//
// ExceptionCannotLoadPTX class implementation.
//

ExceptionCannotLoadPTX::ExceptionCannotLoadPTX(const char* path)
{
    string err("Cannot load PTX file ");
    err += path;
    err += ".";
    set_what(err.c_str());
}


//
// PTXCache implementation.
//

PTXCache::PTXCache(const char* ptx_path)
  : m_ptx_path(ptx_path)
{
}

namespace
{

struct ModuleDeleter
{
    void operator()(CUmodule* mod) const
    {
        cuModuleUnload(*mod);
        delete mod;
    }
};

}

std::shared_ptr<CUmodule> PTXCache::create_cuda_module(
    const char*     ptx_filename)
{
    CUmodule module;
    CUresult result =
        cuModuleLoadDataEx(
            &module,
            get_ptx_code(ptx_filename).c_str(),
            0,
            nullptr,
            nullptr);

    if (result != CUDA_SUCCESS)
        throw ExceptionCUDAError(result);

    return std::shared_ptr<CUmodule>(new CUmodule(module), ModuleDeleter());
}

optix::Program PTXCache::create_optix_program(
    optix::Context& context,
    const char*     ptx_filename,
    const char*     program_name)
{
    return context->createProgramFromPTXString(
        get_ptx_code(ptx_filename),
        program_name);
}

const std::string& PTXCache::get_ptx_code(const char* filename)
{
    map<string, string>::const_iterator it = m_ptx_code.find(filename);

    if (it == m_ptx_code.end())
    {
        bfs::path p(m_ptx_path / filename);
        const char* filepath = p.string().c_str();
        it =
            m_ptx_code.insert(make_pair(filename, read_source_file(filepath))).first;
    }

    return it->second;
}

string PTXCache::read_source_file(const char* filepath)
{
    ifstream file(filepath);

    if (file.good())
    {
        stringstream source_buffer;
        source_buffer << file.rdbuf();
        return source_buffer.str();
    }

    throw ExceptionCannotLoadPTX(filepath);
    return string();
}

}   // namespace renderer
