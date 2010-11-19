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

#ifndef MXCORE_LINEAR_ALLOCATOR_H_
#define MXCORE_LINEAR_ALLOCATOR_H_

#include "MXBase/mxbase.h"
#include "MXCore/mxassert.h"

namespace mxcore {

// Linear memory allocator. Takes a given chunk of memory and increments a marker
// pointer that indicates the next free memory address upon every allocation.
// Memory isn't freed specifically, instead the Rewind() function is used to
// roll back the marker to a new memory address.
// The base address has to be aligned for proper operation. See raw_memory.h for
// aligned allocation functions.
class LinearAllocator {
 public:
  MX_FORCE_INLINE LinearAllocator(void* base, const size_t size, 
                                  size_t alignment = MX_ALIGNMENT_DEFAULT) {
    base_ = marker_ = reinterpret_cast<uint8_t*>(base);
    end_ = base_ + size;
    alignment_ = alignment;
  }

  // Ensures proper memory alignment. Increases size to the next multiple of
  // alignment.
  static MX_FORCE_INLINE size_t AlignSize(
      const size_t size, 
      const size_t alignment) {
    return (size + (alignment - 1)) & ~(alignment - 1);
  }

  // Allocates size bytes from the memory pool.
  MX_FORCE_INLINE void* Allocate(const size_t size) {
    uint8_t* result = marker_;
    size_t aligned_size = AlignSize(size, alignment_);
    marker_ += aligned_size;
    MX_ASSERT(marker_ <= end_);
    return result;
  }

  // Resets the marker to an arbitrary position within the pool's boundaries.
  MX_FORCE_INLINE void Rewind(void* to) {
    MX_ASSERT((to >= base_) && (to <= end_));
    marker_ = reinterpret_cast<uint8_t*>(to);
  }

  MX_FORCE_INLINE size_t alignment() const {
    return alignment_;
  }

  MX_FORCE_INLINE void* marker() const {
    return marker_;
  }

 private:
  uint8_t* base_;
  uint8_t* marker_;
  uint8_t* end_;
  size_t alignment_;
};

}  // namespace mxcore

#endif  // MXCORE_LINEAR_ALLOCATOR_H_
