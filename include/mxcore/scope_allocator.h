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

#ifndef MXCORE_SCOPE_ALLOCATOR_H_
#define MXCORE_SCOPE_ALLOCATOR_H_

#include <new>
#include "mxcore/mxtypes.h"
#include "mxcore/scope_stack.h"

namespace mxcore {

// An STL allocator that takes memory from a scope stack. Never deallocates
// memory, but constructs and destructs allocated objects. Note that this might
// not be 100% standards conform.
template <class T>
class scope_allocator {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  scope_allocator(ScopeStack& scope) : scope_(scope) {}
  scope_allocator(const scope_allocator& other) : scope_(other.scope()) {}
  ~scope_allocator() {}

  template <class U>
  scope_allocator(const scope_allocator<U>&) {}

  template <class U>
  struct rebind { typedef scope_allocator<U> other; };

  pointer address(reference r) const { return &r; }
  const_pointer address(const_reference r) const { return &r; }
  size_type max_size() const { return scope_.size() / sizeof(value_type); }
  
  pointer allocate(size_type n, const void* q = NULL) {
    return reinterpret_cast<pointer>(scope_.NewRaw(n * sizeof(value_type)));
  }
  
  void construct(pointer p, const value_type& v) { new(p) value_type(v); }
  void destroy(pointer p) { p->~value_type(); }
  void deallocate(pointer p, size_type t) {}

  ScopeStack& scope() const { return scope_; }
  
 private:
  scope_allocator() {}
  ScopeStack& scope_;
};

// To be more standards-conforming, we need this specialization.
template <>
class scope_allocator<void>
{
 public:
  typedef void value_type;
  typedef void* pointer;
  typedef const void* const_pointer;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  template <class U>
  scope_allocator(const scope_allocator<U>&) {}

  template <class U>
  struct rebind { typedef scope_allocator<U> other; };
};

template <class T1, class T2>
bool operator==(const scope_allocator<T1>& a, const scope_allocator<T2>& b) {
  return a.scope() == b.scope();
}

template <class T1, class T2>
bool operator!=(const scope_allocator<T1>& a, const scope_allocator<T2>& b) {
  return !(a == b);
}

}  // namespace mxcore

#endif  // MXCORE_SCOPE_ALLOCATOR_H_
