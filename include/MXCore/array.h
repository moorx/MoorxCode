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

#ifndef MXCORE_ARRAY_H_
#define MXCORE_ARRAY_H_

#include "MXBase/mxbase.h"
#include "MXCore/mxassert.h"
#include "MXCore/linear_allocator.h"
#include "MXCore/scope_stack.h"

namespace mxcore {

// Allocates a raw piece of memory big enough to hold the given amount of items.
// Does not construct objects or care for calling destructors.
struct RawArrayTraits {
  template <class T>
  static T* AllocateArray(ScopeStack& scope, const size_t size) {
    return reinterpret_cast<T*>(scope.AllocateRaw(size * sizeof(T)));
  }

  template <class T>
  static size_t CalcStepSize(ScopeStack& scope) {
    return sizeof(T);
  }
};

// Default array behavior. This allocates array items at aligned addresses
// without finalizers.
struct ObjectArrayTraits {
  template <class T>
  static T* AllocateArray(ScopeStack& scope, const size_t size) {
    T* first = scope.AllocateObject<T>();
    
    for (size_t i = 0; i < size; ++i) {
      scope.AllocateObject<T>();
    }

    return first;
  }

  template <class T>
  static size_t CalcStepSize(ScopeStack& scope) {
    size_t item_size = LinearAllocator::AlignSize(sizeof(T),
                                                  scope.alignment());
    return item_size;
  }
};

// Makes an array allocate and construct objects with finalizers. If the scope
// containing the array falls out of scope, all contained items are destructed.
struct FinalizerArrayTraits {
  template <class T>
  static T* AllocateArray(ScopeStack& scope, const size_t size) {
    T* first = scope.AllocateWithFinalizer<T>();
    
    for (size_t i = 0; i < size; ++i) {
      scope.AllocateWithFinalizer<T>();
    }

    return first;
  }

  template <class T>
  static size_t CalcStepSize(ScopeStack& scope) {
    size_t finalizer_size = LinearAllocator::AlignSize(sizeof(Finalizer),
                                                       scope.alignment());
    size_t item_size = LinearAllocator::AlignSize(sizeof(T),
                                                  scope.alignment());
    return finalizer_size + item_size;
  }
};

// Fixed size array of a given type. Type allocation and alignment can be
// controlled using array traits.
template <class MemberType, class Traits = ObjectArrayTraits>
class Array {
 public:
  Array(ScopeStack& scope, const size_t size) : size_(size), scope_(scope) {
    items_ = Traits::template AllocateArray<MemberType>(scope_, size_);
    stepsize_ = Traits::template CalcStepSize<MemberType>(scope_);
  }

  MX_FORCE_INLINE MemberType& operator[](const int32_t index) {
    MX_ASSERT(index < size_);
    return GetItem(index, stepsize_);
  }

  MX_FORCE_INLINE const MemberType& operator[](const int32_t index) const {
    MX_ASSERT(index < size_);
    return GetItem(index, stepsize_);
  }

  MX_FORCE_INLINE size_t size() const {
    return size_;
  }

 private:
  // Returns the array item at the given index. We can't just jump to a offset
  // because we might need to skip alignment padding and finalizer objects.
  MemberType& GetItem(const int32_t index, const size_t stepsize_bytes) {
    uintptr_t base_address = reinterpret_cast<uintptr_t>(items_);
    uintptr_t item_address = base_address + (index * stepsize_bytes);
    MemberType* item = reinterpret_cast<MemberType*>(item_address);
    return *item;
  }

  size_t size_;
  size_t stepsize_;
  ScopeStack& scope_;
  MemberType* items_;
};

}  // namespace mxcore

#endif  // MXCORE_ARRAY_H_
