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

namespace internal {

// Base for all array classes.
template <class MemberType>
class ArrayBase {
 public:
  ArrayBase(ScopeStack& scope) : scope_(scope), items_(NULL) {
  }

  MX_FORCE_INLINE MemberType& GetItem(const int32_t index) {
    return items_[index];
  }

 protected:
  ScopeStack& scope_;
  MemberType* items_;
};

// Base class for array implementations that store their members sparsely in
// memory (with gaps between actual items) due to alignment or prepended
// finalizers. Derived classes have to initialize stepsize_ explicitly as this
// constructor sets it to the illegal value -1.
template <class MemberType>
class SparseArrayBase : public ArrayBase<MemberType> {
 public:
  SparseArrayBase(ScopeStack& scope)
      : ArrayBase<MemberType>(scope),
        stepsize_(-1) {}

  // Returns the item at the given index, using the step size to determine the
  // proper memory address.
  MX_FORCE_INLINE MemberType& GetItem(const int32_t index) {
    uintptr_t base_address = reinterpret_cast<uintptr_t>(this->items_);
    uintptr_t item_address = base_address + (index * stepsize_);
    return *(reinterpret_cast<MemberType*>(item_address));
  }

 protected:
  size_t stepsize_;
};

}  // namespace internal

// Array behavior for objects that do not require any teardown logic. Each
// object is aligned to the scope's boundaries.
template <class MemberType>
class ObjectArrayTraits : public internal::SparseArrayBase<MemberType> {
 public:
  ObjectArrayTraits(ScopeStack& scope, const size_t size)
      : internal::SparseArrayBase<MemberType>(scope) {
    MemberType* item;

    for (size_t i = 0; i < size; ++i) {
      item = this->scope_.template AllocateObject<MemberType>();
      if (i == 0) {
        this->items_ = item;
      }
    }

    this->stepsize_ = LinearAllocator::AlignSize(sizeof(MemberType),
                                                 this->scope_.alignment());
  }
};

// Array implementation for storing objects with finalizers. When getting and
// setting items, proper alignment and offset are used. Finalizers as well as
// objects are aligned, meaning that lots of memory is wasted for small objects.
template <class MemberType>
class FinalizerArrayTraits : public internal::SparseArrayBase<MemberType> {
 public:
  FinalizerArrayTraits(ScopeStack& scope, const size_t size)
      : internal::SparseArrayBase<MemberType>(scope) {
    MemberType* item;

    for (size_t i = 0; i < size; ++i) {
      // .template is needed to treat the following name as a dependent template
      // name.
      item = this->scope_.template AllocateWithFinalizer<MemberType>();
      if (i == 0) {
        this->items_ = item;
      }
    }

    size_t finalizer_size = LinearAllocator::AlignSize(sizeof(Finalizer), 
                                                       this->scope_.alignment());
    size_t item_size = LinearAllocator::AlignSize(sizeof(MemberType),
                                                  this->scope_.alignment());
    this->stepsize_ = finalizer_size + item_size;
  }
};

// Basic fixed-size array. The internal behavior of the array is defined by a
// set of array traits that may be exchanged for different use cases.
template <class MemberType, 
          class ArrayTraits = FinalizerArrayTraits<MemberType> >
class Array {
 public:
  Array(ScopeStack& scope, const size_t size) : size_(size), traits_(scope, size) {
  }

  MX_FORCE_INLINE MemberType& operator[](const int32_t index) {
    MX_ASSERT(index < size_);
    return traits_.GetItem(index);
  }

  MX_FORCE_INLINE const MemberType& operator[](const int32_t index) const {
    MX_ASSERT(index < size_);
    return traits_.GetItem(index);
  }

  MX_FORCE_INLINE size_t size() const {
    return size_;
  }

 private:
  size_t size_;
  ArrayTraits traits_;
};

}  // namespace mxcore

#endif  // MXCORE_ARRAY_H_
