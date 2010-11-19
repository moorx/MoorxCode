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

#include "MXCore/raw_memory.h"

#include <stdlib.h>

namespace mxcore {

// Allocates size + alignment bytes and increases the returned address to the
// next suitable one. The offset to the actual address is prepended in the
// unused memory space before the actual pointer. Memory allocated with
// AllocateAligned has to be freed with FreeAligned!
void* AllocateAligned(const size_t size, const size_t alignment) {
  uintptr_t raw_address = reinterpret_cast<uintptr_t>(
      malloc(size + alignment));

  uintptr_t mask = alignment - 1;
  size_t misalignment = raw_address & mask;
  size_t adjustment = alignment - misalignment;
  uintptr_t aligned_address = raw_address + adjustment;

  size_t* adjustment_pointer = reinterpret_cast<size_t*>(
      aligned_address - sizeof(size_t));
  *adjustment_pointer = adjustment;

  return reinterpret_cast<void*>(aligned_address);
}

// Retrieves the alignment adjustment and properly frees the allocated memory.
void FreeAligned(void* memory) {
  uintptr_t aligned_address = reinterpret_cast<uintptr_t>(memory);
  size_t* adjustment_pointer = reinterpret_cast<size_t*>(
      aligned_address - sizeof(size_t));
  size_t adjustment = *adjustment_pointer;
  free(reinterpret_cast<void*>(aligned_address - adjustment));
}

}  // namespace mxcore
