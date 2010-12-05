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

#ifndef MXCORE_ALIGNED_MEMORY_H_
#define MXCORE_ALIGNED_MEMORY_H_

#include <stdlib.h>
#include "MXCore/mxtypes.h"
#include "MXCore/mxassert.h"

namespace mxcore {

// A chunk of aligned memory.
template <const size_t kAlignment>
class AlignedMemory {
 public:
  AlignedMemory(const size_t size);
  ~AlignedMemory();

  size_t size() const { return size_; }
  size_t alignment() const { return kAlignment; }
  void* pointer() const { return pointer_; }

 private:
  AlignedMemory(const AlignedMemory<kAlignment>& other) {}
  AlignedMemory& operator=(const AlignedMemory<kAlignment>& other) {}
  void* pointer_;
  const size_t size_;
};

template <const size_t kAlignment>
AlignedMemory<kAlignment>::AlignedMemory(const size_t size) : size_(size) {
  uintptr_t raw_address = reinterpret_cast<uintptr_t>(malloc(size_ + kAlignment));

  uintptr_t mask = kAlignment - 1;
  size_t misalignment = raw_address & mask;
  size_t adjustment = kAlignment - misalignment;
  uintptr_t aligned_address = raw_address + adjustment;

  MxAssert(kAlignment >= sizeof(adjustment));
  size_t* adjustment_pointer = reinterpret_cast<size_t*>(
      aligned_address - sizeof(adjustment));
  *adjustment_pointer = adjustment;

  pointer_ = reinterpret_cast<void*>(aligned_address);
}

template <const size_t kAlignment>
AlignedMemory<kAlignment>::~AlignedMemory() {
  uintptr_t aligned_address = reinterpret_cast<uintptr_t>(pointer_);
  size_t* adjustment_pointer = reinterpret_cast<size_t*>(
      aligned_address - sizeof(size_t));
  size_t adjustment = *adjustment_pointer;
  free(reinterpret_cast<void*>(aligned_address - adjustment));
}

}  // namespace mxcore

#endif  // MXCORE_ALIGNED_MEMORY_H_
