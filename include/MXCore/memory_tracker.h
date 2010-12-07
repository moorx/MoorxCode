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

#ifndef MXCORE_MEMORY_TRACKER_H_
#define MXCORE_MEMORY_TRACKER_H_

#include <assert.h>
#include <tr1/unordered_map>
#include "MXCore/mxtypes.h"

namespace mxcore {

namespace internal {

// Stores information about an allocation.
struct Allocation {
  Allocation(void* pointer, const char8_t* file, const uint32_t line,
             const size_t size);

  void* pointer_;
  const char8_t* file_;
  const uint32_t line_;
  const size_t size_;
};

}  // namespace internal

// Stores all allocations and allows to generate a report of allocated memory.
// Use the macros below instead of new or malloc to enable tracking. Memory
// tracking is only active if _DEBUG is defined.
class MemoryTracker {
 public:
  static void* Add(const internal::Allocation& allocation);
  static bool Remove(void* pointer);
  static void Report();

  static size_t bytes_allocated() { return bytes_allocated_; }
  
  template <class T>
  static T* Delete(T* pointer) {
    delete pointer;
    return pointer;
  }

  template <class T>
  static T* DeleteArray(T* array) {
    delete[] array;
    return array;
  }

  template <class T>
  static T* Free(T* pointer) {
    free(pointer);
    return pointer;
  }

 private:
  typedef std::tr1::unordered_map<void*, internal::Allocation> AllocationMap;
  typedef std::tr1::unordered_map<void*,
          internal::Allocation>::value_type AllocationTuple;
  typedef std::tr1::unordered_map<void*,
          internal::Allocation>::iterator AllocationIterator;

  static size_t bytes_allocated_;
  static AllocationMap allocations_;
};

}  // namespace mxcore

#ifdef _DEBUG
  #define mxnew(type, constructor) reinterpret_cast< type *>( \
      mxcore::MemoryTracker::Add(mxcore::internal::Allocation(new type constructor, \
                                                              __FILE__, \
                                                              __LINE__, \
                                                              sizeof( type ))))
  #define mxdelete(pointer) { if (!mxcore::MemoryTracker::Remove( \
              mxcore::MemoryTracker::Delete((pointer)))) assert(false && #pointer); }

  #define mxnew_array(type, size) reinterpret_cast< type *>( \
      mxcore::MemoryTracker::Add(mxcore::internal::Allocation(new type [ size ], \
                                                              __FILE__, \
                                                              __LINE__, \
                                                              sizeof( type ) * (size))))
  #define mxdelete_array(pointer) { if (!mxcore::MemoryTracker::Remove( \
              mxcore::MemoryTracker::DeleteArray((pointer)))) assert(false && #pointer); }
  
  #define mxalloc(size) mxcore::MemoryTracker::Add(mxcore::internal::Allocation( \
          malloc((size)), __FILE__, __LINE__, (size)))
  #define mxfree(pointer) { if (!mxcore::MemoryTracker::Remove( \
              mxcore::MemoryTracker::Free((pointer)))) assert(false && #pointer); }
#else
  #define mxnew(type, constructor) new (type)((constructor))
  #define mxdelete(pointer) delete (pointer)

  #define mxnew_array(type, size) new (type)[(size)]
  #define mxdelete_array(pointer) delete[] (pointer)

  #define mxalloc(size) malloc((size))
  #define mxfree(pointer) free((pointer))
#endif

#endif  // MXCORE_MEMORY_TRACKER_H_
