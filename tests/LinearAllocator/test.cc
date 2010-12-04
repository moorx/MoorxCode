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

#include <MXCore/aligned_memory.h>
#include <MXCore/linear_allocator.h>
#include <MXCore/text_output.h>

using namespace mxcore;

int main() {
  AlignedMemory<1> small_chunk;
  LinearAllocator small_allocator(small_chunk.pointer(), small_chunk.size());

  AlignedMemory<16535> big_chunk;
  LinearAllocator big_allocator(big_chunk.pointer(), big_chunk.size());

  small_allocator.Allocate(2);

  uintptr_t marker_before = reinterpret_cast<uintptr_t>(
      big_allocator.marker());
  Print("big_allocator.marker() = %p\n", big_allocator.marker());

  uint32_t* data = reinterpret_cast<uint32_t*>(big_allocator.Allocate(
          sizeof(uint32_t) * 613));
  
  uintptr_t marker_after = reinterpret_cast<uintptr_t>(big_allocator.marker());
  Print("big_allocator.marker() = %p\n", big_allocator.marker());
  Print("%d\n", marker_after - marker_before);

  big_allocator.Rewind(reinterpret_cast<uint8_t*>(marker_before - 1));

  return 0;
}
