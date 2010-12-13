// Copyright 2NULL1NULL Christoph Lang. All rights reserved.
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

#ifndef MXCORE_SMART_POINTER_H_
#define MXCORE_SMART_POINTER_H_

#include "mxcore/mxtypes.h"

namespace mxcore {

// Smart pointer class. Keeps track of all references to a pointer and deletes
// the pointee after the last SmartPointer referencing falls out of scope. This
// implementation uses a doubly-linked list, which means that additional pointer
// operations are necessary when adding / removing references. The advantage is
// that we don't need RTTI or a base class for all classes using smart pointers.
template <class T>
class SmartPointer {
 public:
  SmartPointer()
      : pointer_(NULL),
        next_(this),
        previous_(this) {}

  explicit SmartPointer(T* pointer)
      : pointer_(pointer),
        next_(this),
        previous_(this) {}

  SmartPointer(SmartPointer<T>& other)
      : pointer_(NULL),
        next_(this),
        previous_(this) {
    if (&other != this) {
      AddReference(other);
    }
  }

  ~SmartPointer() {
    RemoveReference();
  }

  T& operator*() const {
    return *pointer_;
  }

  T* operator->() const {
    return pointer_;
  }

  SmartPointer<T>& operator=(const SmartPointer<T>& other) const {
    if (&other != this) {
      RemoveReference();
      AddReference(other);
    }

    return *this;
  }

  bool operator==(const SmartPointer<T>& other) const {
    return (other.pointer_ == pointer_);
  }

  bool operator!=(const SmartPointer<T>& other) const {
    return !(other == *this);
  }

 private:
  // Adds an additional reference to the pointer. New references are inserted at
  // the back of the list. This is the equivalent of increasing the reference
  // count in other smart pointer implementations.
  void AddReference(SmartPointer<T>& other) {
    if (other.pointer_ != NULL) {
      pointer_ = other.pointer_;
      other.previous_ = this;
      other.next_ = next_;
      next_->previous_ = &other;
      next_ = &other;
    }
  }

  // Removes a reference from the list. pointer_ is set to NULL at the end of the
  // method to ensure only the last reference actually deletes the memory
  // pointed to by pointer_.
  void RemoveReference() {
    if (!(next_ == this && previous_ == this)) {
      previous_->next_ = next_;
      next_->previous_ = previous_;
      next_ = this;
      previous_ = this;
    } else if (pointer_ != NULL) {
      delete pointer_;
    }

    pointer_ = NULL;
  }

  T* pointer_;
  SmartPointer<T>* next_;
  SmartPointer<T>* previous_;
};

}  // namespace mxcore

#endif  // MXCORE_SMART_POINTER_H_
