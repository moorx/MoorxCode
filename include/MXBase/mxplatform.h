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

#ifndef MXBASE_MXPLATFORM_H_
#define MXBASE_MXPLATFORM_H_

#if defined(_WIN32)
  #define MX_PLATFORM_WINDOWS
  #define MX_ALIGNMENT_DEFAULT 16
  #if !defined(_WIN64)
    #define MX_PLATFORM_WIN32
    #define MX_PROCESSOR_X86
  #elif defined(_WIN64)
    #define MX_PLATFORM_WIN64
    #define MX_PROCESSOR_X64
  #else
    #error Unknown Windows platform.
  #endif

#elif defined(__APPLE__) && defined(__MACH__)
  #define MX_PLATFORM_MACOS
  #define MX_ALIGNMENT_DEFAULT 16
  #if defined(__i386__)
    #define MX_PROCESSOR_X86
  #elif defined(__amd64__)
    #define MX_PROCESSOR_X64
  #else
    #error Unknown MacOS platform.
  #endif

#else
  #error Unknown platform.
  #error Unknown processor.
#endif

#endif  // MXBASE_MXPLATFORM_H_
