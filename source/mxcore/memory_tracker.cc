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

#include "mxcore/memory_tracker.h"

namespace mxcore {

namespace internal {

Allocation::Allocation(void* pointer, const char8_t* file, const uint32_t line,
                       const size_t size)
    : pointer_(pointer),
      file_(file),
      line_(line),
      size_(size) {}

}  // namespace internal

size_t MemoryTracker::bytes_allocated_ = 0;
MemoryTracker::AllocationMap MemoryTracker::allocations_;

void* MemoryTracker::Add(const internal::Allocation& allocation) {
  AllocationTuple item(allocation.pointer_, allocation);
  allocations_.insert(item);
  bytes_allocated_ += allocation.size_;
  return allocation.pointer_;
}

bool MemoryTracker::Remove(void* pointer) {
   AllocationIterator item = allocations_.find(pointer);

  if (item != allocations_.end()) {
    bytes_allocated_ -= item->second.size_;
    allocations_.erase(item);
    return true;
  }

  printf("Tried to delete illegal pointer @ 0x%p\n", pointer);
  return false;
}

void MemoryTracker::Report() {
  printf("*** MEMORY REPORT ***\n");

  AllocationIterator item;
  AllocationIterator end = allocations_.end();

  for (item = allocations_.begin(); item != end; ++item) {
    printf("%s at line %d @ 0x%p\n", item->second.file_, item->second.line_,
           item->second.pointer_);
  }

  printf("*** MEMORY REPORT ***\n");
}

}  // namespace mxcore
