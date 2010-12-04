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

#ifndef MXCORE_SCOPE_STACK_H_
#define MXCORE_SCOPE_STACK_H_

#include <new>
#include "MXBase/mxbase.h"
#include "MXCore/linear_allocator.h"

namespace mxcore {

// A finalizer is prepended to every object allocated from the memory pool.
// Before rewinding allocated memory, ScopeStack iterates over all finalizers,
// calling destructors and teardown functions.
struct Finalizer {
  void (*function_)(void* data);
  Finalizer* next;
};

// Wrapper for calling destructors.
template <class T>
void CallDestructor(void* data) {
  reinterpret_cast<T*>(data)->~T();
}

// Allocates memory from a linear allocator for POD types as well as objects. As
// soon as it falls out of scope, ScopeStack automatically calls object
// destructors and rewinds the underlying allocator. Instances of ScopeStack
// have to live on the stack!
class ScopeStack {
 public:
  explicit MX_FORCE_INLINE ScopeStack(LinearAllocator& allocator)
      : allocator_(allocator),
        base_(allocator.marker()),
        finalizer_chain_(NULL) {
  }

  ~ScopeStack() {
    for (Finalizer* finalizer = finalizer_chain_; finalizer != NULL;
         finalizer = finalizer->next) {
      (*finalizer->function_)(GetObjectFromFinalizer(finalizer));
    }
    allocator_.Rewind(base_);
  }

  // Allocates and constructs an object and adds a call to the desctructor to
  // the front of the finalizer chain.
  template <class T>
  MX_FORCE_INLINE T* NewWithFinalizer() {
    Finalizer* finalizer = AllocateWithFinalizer(sizeof(T));
    T* result = new(GetObjectFromFinalizer(finalizer)) T;

    finalizer->function_ = &CallDestructor<T>;
    finalizer->next = finalizer_chain_;
    finalizer_chain_ = finalizer;

    return result;
  }

  // Allocates memory and constructs an object but doesn't add it to the
  // finalizer list. Use this for structs and objects that don't need to clean
  // up.
  template <class T>
  MX_FORCE_INLINE T* NewObject() const {
    return new(allocator_.Allocate(sizeof(T))) T;
  }

  // Allocate a chunk of raw memory from the underlying linear allocator.
  MX_FORCE_INLINE void* NewRaw(const size_t size) const {
    return allocator_.Allocate(size);
  }

  MX_FORCE_INLINE size_t alignment() const {
    return allocator_.alignment();
  }

 private:
  // Given a pointer to a finalizer, calculates the offset to the actual object
  // and returns it.
  MX_FORCE_INLINE void* GetObjectFromFinalizer(Finalizer* finalizer) const {
    return reinterpret_cast<uint8_t*>(finalizer) + LinearAllocator::AlignSize(
        sizeof(Finalizer), allocator_.alignment());
  }

  // To ensure a proper teardown, a Finalizer object is prepended to the actual
  // object. Allocates enough memory for object and finalizer and guarantees
  // proper alignment of both.
  MX_FORCE_INLINE Finalizer* AllocateWithFinalizer(const size_t size) const {
    return reinterpret_cast<Finalizer*>(allocator_.Allocate(
            size + LinearAllocator::AlignSize(sizeof(Finalizer), 
                                              allocator_.alignment())));
  }

  LinearAllocator& allocator_;
  void* base_;
  Finalizer* finalizer_chain_;
};

}  // namespace mxcore

#endif  // MXCORE_SCOPE_STACK_H_
