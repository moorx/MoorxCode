// Copyright 2010 Christoph Lang. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Christoph Lang nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef MXBASE_MXBASE_H_
#define MXBASE_MXBASE_H_

#include "mxcompiler.h"
#include "mxplatform.h"

#ifndef MXBASE_VERSION
  #define MXBASE_VERSION_STRING "0.00.00"
  #define MXBASE_VERSION_MAJOR 0
  #define MXBASE_VERSION_MINOR 0
  #define MXBASE_VERSION_PATCH 0
#endif

// Defines NULL, size_t
#if defined(MX_COMPILER_GNUC) || defined(MX_COMPILER_MSVC)
  #include <stddef.h>
#endif

// Include standard integer and float types.
#if defined(MX_COMPILER_GNUC) || defined(MX_COMPILER_MSVC_10)

// This macro is required for standard printf format specifiers.
  #ifndef __STDC_FORMAT_MACROS
    #define __STDC_FORMAT_MACROS
  #endif

  #include <inttypes.h>
  #include <stdint.h>
  #include <float.h>
  #include <math.h>

  typedef char char8_t;
#endif

#if defined(_DEBUG) || defined(MX_DEBUG)
  #undef MX_DEBUG
  #define MX_DEBUG 1
#else
  #undef MX_DEBUG
  #define MX_DEBUG 0
#endif

#endif  // MXBASE_MXBASE_H_
